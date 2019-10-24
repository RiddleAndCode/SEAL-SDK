

// http://ww1.microchip.com/downloads/en/DeviceDoc/ATAES132A-Data-Sheet-40002023A.pdf
// Read datasheet for more information


#include "sec_elem_abstr.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"


#define PRINT_AUTHENTICATION_LOG()   (i > 4 ? (i == 5 ? printf("\tAuthenticating memory slot %d with key in slot (1) \n",i) \
: (i == 6 ? printf("\tAuthenticating memory slot %d with key in slot (2) \n",i) :\
 printf("\tAuthenticating memory slot %d with key in slot (0) \n",i)) ):\
  printf("\tMemory slot %d doesn't require authentication\n",i))



void test_write_data();


int main(void) {


  TEST_ASSERT_EQUAL(SE_SUCCESS,se_secure_storage_init());
  TEST_ASSERT_EQUAL(SE_SUCCESS,se_secure_storage_personalize(false));

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

for (size_t i = 0; i < ZONE; i++)
{
  printf("\n\n\tRunning the READ/WRITE tests for memory slot (%d-%d)\n",i,ZONE-1);
  PRINT_AUTHENTICATION_LOG();
  printf("\tWriting %d bytes of data\n\n",SIZE);

  TEST_ASSERT_EQUAL(SE_SUCCESS,se_authenticate(i));
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
}
