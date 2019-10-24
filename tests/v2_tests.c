#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "types.h"
#include "sec_elem_abstr.h"
#include "uECC.h"
#include "unity.h"

void test_wipe(void);
void test_generate(void);
void test_store_key_pair(void);
static void signTest(bool storeTest);
void test_store(void);

static int seconds;

#define EX_APP_VERSION "1.00:1.01"
#define KEY_PAIR_NUM 4
#define DELAY_TEST
#define MAX_CHUNK_LENGTH_LINK 256

/*******************************************************************************
 **   Main Function  main()
 *******************************************************************************/
int main(int argc, char **argv) {

    if(argc > 2){
    printf("\n\t\tUsage : sudo ./sea_test X\n\t\tX is the number of seconds to test the program with. \n\t\tExiting ...\n\n");
    exit(EXIT_FAILURE);
    }
    if(argv[1] == NULL)
    seconds = 0;
    else
    seconds = atoi(argv[1]);

    printf( "SecureElement v2 HostLibrary light application (Rev %s)\r\n",
    EX_APP_VERSION);
    printf("**********************************************\r\n");
    printf( "Connect to SecureElement v2-SM. Chunksize at link layer = %d.\r\n", MAX_CHUNK_LENGTH_LINK);
    UNITY_BEGIN();

    RUN_TEST(test_store);
    RUN_TEST(test_wipe);
    RUN_TEST(test_store_key_pair);
    RUN_TEST(test_wipe);
    RUN_TEST(test_generate);

     return UNITY_END();
}

void test_wipe(void)
{
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    printf("\n\r\t... Running Wipe Device Test ...\n\n\r");
    size_t i;
    for(i =0 ;i < KEY_PAIR_NUM ;i++)
    {
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_wipe_device(i));

        printf("\tKeypair at index %d has been deleted\n\r",i);
    }
    printf("\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());
    printf("\t\t\n Closing I2C Communications Succeeded\r\n\n");

}

void test_generate(void)
{
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    printf("\n\r\t... Running Generate Key Pairs Test ...\n\n");
    int i;
    for(i = 0;i < KEY_PAIR_NUM ;i++)
    {
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_generate_keypair(i));
    }

    if(seconds){
    sleep(seconds);
    se_init(RESUME_SCI2C);
    }

    signTest(false);
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());
    printf("\t\t\n Closing I2C Communications Succeeded\r\n\n");


}
void test_store_key_pair(void)
{
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    printf("\n\r\t... Running Store External Key Pairs Test ...\n\r\n ");
    uint8_t pubKey[64];
    uint8_t uncompPubKey[65];
    uint8_t privKey[32];
    int i = 0;
    for(i = 0;i < KEY_PAIR_NUM ;i++)
    {
        TEST_ASSERT_EQUAL(1,uECC_make_key(pubKey,privKey,uECC_secp256r1()));

        uncompPubKey[0] = 0x04;
        memcpy(uncompPubKey+1,pubKey,sizeof(pubKey));
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_save_key_pair(i,uncompPubKey,sizeof(uncompPubKey),privKey,sizeof(privKey)));

    }

    if(seconds){
    sleep(seconds);
    se_init(RESUME_SCI2C);
    }

    signTest(true);
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());
    printf("\t\t\n Closing I2C Communications Succeeded\r\n\n");


}
void test_store(void)
{
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    printf("\n\r\t... Running Store Data Test ...\n\r\n ");
    uint8_t tst[256];
    uint16_t tstLen = 128;
    uint8_t rsp[256] = {0};
    uint16_t rspLen = 256;
    int j;
    for(j=0;j<128;j++)
    {
        tst[j] = 0xf0;
    }
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_write_data(0,tst,tstLen));

    for(j=0;j<128;j++)
    {
        tst[j] = 0x88;
    }
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_write_data(128,tst,tstLen));

    if(seconds){
    sleep(seconds);
    se_init(RESUME_SCI2C);
    }

    rspLen = 256;

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_read_data(0,rsp,rspLen));

    memset(tst,0xf0,128);
    memset(tst+128,0x88,128);

    TEST_ASSERT_EQUAL(0,memcmp(rsp,tst,256));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());
    printf("\t\t\n Closing I2C Communications Succeeded\r\n\n");


}
typedef struct {
    uint8_t pub[(64 << 1)+1+8]; //!< Public key stored as byte array + 8 for wrapping
    uint16_t pubLen;          //!< Length of pub (expressed as number of byte)
} sea_pubkey_t;

static void signTest(bool storeTest)
{
    printf("\n\r\t... Running Signing & Verification Test ...\n\r\n");
    uint8_t str[] = {"HELLO WORLD !"};
    uint8_t sha[32] = {0};
    uint16_t shaLen =32;
    sea_pubkey_t dodo[4];

    dodo[0].pubLen = sizeof(dodo->pub);
    dodo[1].pubLen = sizeof(dodo->pub);
    dodo[2].pubLen = sizeof(dodo->pub);
    dodo[3].pubLen = sizeof(dodo->pub);
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_sha256(str,sizeof(str),sha,&shaLen));
    size_t i;
    uint8_t sign[256] = {0};
    uint16_t signLen = 256;
    uint8_t rawSign[64] = {0};
    #ifdef PRINT_LOG
    printf("HASH IS : ");
    for(i=0;i<shaLen;i++)
    {
        printf("%02x ",sha[i]);
    }
    puts("");
    #endif
    for(i=0;i<KEY_PAIR_NUM;i++)
    {
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_sign_raw(i,sha,shaLen,sign,&signLen,rawSign));

        #ifdef PRINT_LOG
        puts("Sign Succesfull");
        printf("Signature size is : %d\n\r",signLen1);
        printf("SIGNATURE IS : ");
        for(i=0;i<signLen1;i++)
        {
            printf("%02x ",sign1[i]);
        }
        puts("");
        printf("RAW SIGNATURE IS : ");
        for(i=0;i<64;i++)
        {
            printf("%02x ",rawSign[i]);
        }
        puts("");
        #endif
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(i,dodo[i].pub,&(dodo[i].pubLen)));
        TEST_ASSERT_NOT_EQUAL(0,uECC_verify(dodo[i].pub + 1, sha, shaLen, rawSign, uECC_secp256r1()));

        if(storeTest)
        {
            TEST_ASSERT_EQUAL(SE_VERIFY_SUCCESS,se_verify_external(i,dodo[i].pub,dodo[i].pubLen,sha,shaLen,sign,signLen));
        }
        else
        {
            TEST_ASSERT_EQUAL(SE_VERIFY_SUCCESS,se_verify(i,sha,shaLen,sign,signLen));

        }
        signLen = 256;

    }
}