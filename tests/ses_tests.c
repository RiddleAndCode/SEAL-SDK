

// http://ww1.microchip.com/downloads/en/DeviceDoc/ATAES132A-Data-Sheet-40002023A.pdf
// Read datasheet for more information


#include "sec_elem_abstr.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"

uint8_t test_pin_0[]={'R','I','D','D','L','E','&','C','O','D','E','R','O','C','K','S','!'};
uint8_t test_pin_1[]={'C','+','+',' ','I','S',' ','N','O','T',' ','C'};
uint8_t test_pin_2[]={'P','E','E','R','T','O','P','E','E','R','E','L','E','C','T','R','O','N','I','C','K','C','A','S','H','S','Y','S','T','E','M'};


#define PRINT_AUTHENTICATION_LOG()   (i > 4 ? (i == 5 ? printf("\tAuthenticating memory slot %d with key in slot (0) \n",i) \
: (i == 6 ? printf("\tAuthenticating memory slot %d with key in slot (1) \n",i) :\
 printf("\tAuthenticating memory slot %d with key in slot (2) \n",i)) ):\
  printf("\tMemory slot %d doesn't require authentication\n",i))



void test_write_data();


int main(void) {


  TEST_ASSERT_EQUAL(SE_SUCCESS,se_secure_storage_init());
  TEST_ASSERT_EQUAL(SE_SUCCESS,se_secure_storage_personalize(false,test_pin_0,test_pin_1,test_pin_2));

  UNITY_BEGIN();
  RUN_TEST(test_write_data);


  TEST_ASSERT_EQUAL(SE_SUCCESS,se_secure_storage_close());

  return UNITY_END();
}


void test_write_data()
{
  #define SIZE 32
  #define ZONE 16
  uint8_t data[SIZE] = {0};
  uint8_t write_buffer[SIZE]= {0};
  memset(write_buffer,0xBE,SIZE/4);
  memset(write_buffer+(SIZE/4),0xEF,SIZE/4);
  memset(write_buffer+(SIZE/2),0xCA,SIZE/4);
  memset(write_buffer+(3*SIZE/4),0xFE,SIZE/4);
  uint8_t* key_ptr = NULL;
for (size_t i = 0; i < ZONE; i++)
{
  printf("\n\n\tRunning the READ/WRITE tests for memory slot (%d-%d)\n",i,ZONE-1);
  PRINT_AUTHENTICATION_LOG();
  printf("\tWriting %d bytes of data\n\n",SIZE);
  if(i == 5)
    key_ptr = test_pin_0;
  else if(i == 6)
    key_ptr = test_pin_1;
  else if(i > 6  && i < 16)
    key_ptr = test_pin_2;
  TEST_ASSERT_EQUAL(SE_SUCCESS,se_authenticate(i,key_ptr));
  TEST_ASSERT_EQUAL(SE_SUCCESS, se_secure_store(i,write_buffer,SIZE)) ;

  TEST_ASSERT_EQUAL(SE_SUCCESS,se_secure_read(i,data,SIZE)) ;
  printf("[ ");
  for (size_t i = 0; i < SIZE; i++)
  {
    printf("%02x ",data[i]);
  }
  printf(" ]\n");


  TEST_ASSERT_EQUAL(memcmp(data, write_buffer, SIZE), 0);

  memset(data,0,SIZE);
}
printf("\n\n\tAttemting to authenticate slot 5 with the Key number 1, Should Fail ... \n");
TEST_ASSERT_NOT_EQUAL(SE_SUCCESS,se_authenticate(5,test_pin_1));
printf("\n\n\tAuthenticating slot 5 with the Key number 0, Should Pass ... \n");
TEST_ASSERT_EQUAL(SE_SUCCESS,se_authenticate(5,test_pin_0));
printf("\n\n\tAttemting to write to slot 6 when chip is authenticated for slot 5 , Should Fail ... \n");
TEST_ASSERT_NOT_EQUAL(SE_SUCCESS, se_secure_store(6,write_buffer,SIZE)) ;
printf("\n\n\tAttemting to authenticate slot 6 with the Key number 2, Should Fail ... \n");
TEST_ASSERT_NOT_EQUAL(SE_SUCCESS,se_authenticate(6,test_pin_2));
printf("\n\n\tAttemting to authenticate slot 10 with the Key number 0, Should Fail ... \n");
TEST_ASSERT_NOT_EQUAL(SE_SUCCESS,se_authenticate(10,test_pin_0));

}
