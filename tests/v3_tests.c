#include "sec_elem_abstr.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"

#define KEY_PAIR_NUM 3

void test_verify();
void test_generate();
void store_test();


int main()
{



    UNITY_BEGIN();
    RUN_TEST(test_generate);
    RUN_TEST(test_verify);
    RUN_TEST(store_test);

    return UNITY_END();

}

void test_generate()
{
    printf("\n\r\t... Running Generate Key Pairs Test ...\n\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS, se_init(0));

    for (size_t i = 0; i < KEY_PAIR_NUM; i++)
    {
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_generate_keypair(i));
    }

    TEST_ASSERT_EQUAL(SE_SUCCESS, se_close());
}

void test_verify()
{
    printf("\n\r\t... Running Get Public Key -> Hashing -> Signing -> Verifying Tests ...\n\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS, se_init(0));

    uint8_t publicKeys[3][68] = {{0}};
    uint16_t publicKeyLen[3]  = {sizeof(publicKeys[0]),sizeof(publicKeys[1]),sizeof(publicKeys[2])} ;
    uint8_t digest[32] = {0};
    uint16_t digestLen = sizeof(digest);
    uint8_t signatures [3][70]={{0}};     //To store the signture generated
    uint16_t signatureLen[3] ={sizeof(signatures[0]),sizeof(signatures[1]),sizeof(signatures[2])} ;
    uint8_t input[] = {"Hello world!"};

    for (size_t i = 0; i < KEY_PAIR_NUM; i++)
    {
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(i,*(publicKeys+i),publicKeyLen+i));
    }

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_sha256(input,sizeof(input),digest,&digestLen));

    for (size_t i = 0; i < KEY_PAIR_NUM; i++)
    {
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_sign(i,digest,digestLen,*(signatures+i),signatureLen+i));
    }

    for (size_t i = 0; i < KEY_PAIR_NUM; i++)
    {
        TEST_ASSERT_EQUAL(SE_SUCCESS,se_verify_external(0,*(publicKeys+i),publicKeyLen[i],digest,digestLen,*(signatures+i),signatureLen[i]));
    }

    TEST_ASSERT_EQUAL(SE_SUCCESS, se_close());

}

void store_test()
{
    printf("\n\r\t... Running Random Number Generation -> Store Tests ...\n\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS, se_init(0));

    uint8_t writeBuf[256] = {0};
    uint8_t readBuf[256] = {0};
    uint8_t randBuf[64] = {0};
    uint16_t randLen = sizeof(randBuf);
    memset(writeBuf,0xBE,256);

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_write_data(0,writeBuf,sizeof(writeBuf)));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_read_data(0,readBuf,sizeof(readBuf)));
    TEST_ASSERT_EQUAL(0,memcmp(writeBuf,readBuf,256));

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_random(randBuf,randLen));

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_write_data(randLen,randBuf,randLen));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_read_data(randLen,readBuf,randLen));
    TEST_ASSERT_EQUAL(0,memcmp(randBuf,readBuf,64));

    TEST_ASSERT_EQUAL(SE_SUCCESS, se_close());

}