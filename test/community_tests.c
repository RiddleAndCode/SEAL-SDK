#include "sec_elem_abstr.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "unity.h"

void test_save_pubkey();
void test_random();
void test_generate();
void test_read_pubkey();
void test_verify();
void test_store();
void test_save_pubkey();

uint8_t publicKeys[3][64]=  {{0}};


int main()
{
    UNITY_BEGIN();


    RUN_TEST(test_random);
    RUN_TEST(test_generate);
    RUN_TEST(test_read_pubkey);
    RUN_TEST(test_save_pubkey);
    RUN_TEST(test_store);
    RUN_TEST(test_verify);

    return UNITY_END();
}




void test_random()
{
    printf("\n\r\t... Running Random Generation Test ...\n\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    uint8_t rnd[2][32]= {{0}};
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_random(rnd[0],0));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_random(rnd[1],0));
    TEST_ASSERT_NOT_EQUAL(0,memcmp(rnd[0],rnd[1],32));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());

}


void test_generate()
{
    printf("\n\r\t... Running Generate Key-Pair Test ...\n\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_generate_keypair(0));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_generate_keypair(2));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_generate_keypair(7));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());
}

void test_read_pubkey()
{
    printf("\n\r\t... Running Read Public Key Test ...\n\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(0,publicKeys[0],NULL));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(2,publicKeys[1],NULL));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(7,publicKeys[2],NULL));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());
}
void test_verify()
{
    uint8_t input[] = "Hello world!!";
    uint8_t sha[32] = {0};
    uint16_t shaLen = 32;
    uint8_t signature[3][64] =  {{0}};
    printf("\n\r\t... Running Verify Test ...\n\n\r");
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_sha256(input,sizeof(input),sha,&shaLen));

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_sign(0,sha,0,signature[0],0));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_sign(2,sha,0,signature[1],0));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_sign(7,sha,0,signature[2],0));

    TEST_ASSERT_EQUAL(SE_VERIFY_SUCCESS,se_verify_external(0,publicKeys[0],0,sha,0,signature[0],0));
    TEST_ASSERT_EQUAL(SE_VERIFY_SUCCESS,se_verify_external(0,publicKeys[1],0,sha,0,signature[1],0));
    TEST_ASSERT_EQUAL(SE_VERIFY_SUCCESS,se_verify_external(0,publicKeys[2],0,sha,0,signature[2],0));

    TEST_ASSERT_EQUAL(SE_VERIFY_FAIL,se_verify_external(0,publicKeys[0],0,sha+1,0,signature[0],0));
    TEST_ASSERT_EQUAL(SE_VERIFY_FAIL,se_verify_external(0,publicKeys[1],0,sha+1,0,signature[1],0));
    TEST_ASSERT_EQUAL(SE_VERIFY_FAIL,se_verify_external(0,publicKeys[2],0,sha+1,0,signature[2],0));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());

}

void test_store()
{
    printf("\n\r\t... Running Store Test ...\n\n\r");
    uint8_t write_data[72] = {0};
    uint8_t read_data[72] = {0};
    memset(write_data,'x',72);
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_write_data(0,write_data,72));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_read_data(0,read_data,72));
    TEST_ASSERT_EQUAL(0,memcmp(write_data,read_data,72));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());

}
void test_save_pubkey()
{
    printf("\n\r\t... Running Store Public Key Test ...\n\n\r");
    uint8_t PUB[64] = {0};
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_init(ESTABLISH_SCI2C));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_save_key_pair(10,publicKeys[0],0,0,0));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_save_key_pair(11,publicKeys[0],0,0,0));
    TEST_ASSERT_EQUAL(SE_SUCCESS,se_save_key_pair(12,publicKeys[0],0,0,0));

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(10,PUB,0));
    TEST_ASSERT_EQUAL(0,memcmp(PUB,publicKeys[0],64));
    memset(PUB,0,64);

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(11,PUB,0));
    TEST_ASSERT_EQUAL(0,memcmp(PUB,publicKeys[0],64));
    memset(PUB,0,64);

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_get_pubkey(12,PUB,0));
    TEST_ASSERT_EQUAL(0,memcmp(PUB,publicKeys[0],64));

    TEST_ASSERT_EQUAL(SE_SUCCESS,se_close());


}