#include "sec_elem_abstr.h"
#include <Arduino.h>



uint8_t testText[] = "\t\tSmart cities combine the Internet of Things with almost every aspect of life,and with \n"
                     "\trecent developments in mind,smart cities will become a reality in the near future. Investments by \n"
                     "\tBill Gates near Phoenix, Arizona, NEOM in the Middle East or development plans for Hangzhou in China, show a \n"
                     "\tuniversal interest all over the world.While numerous examples from every aspect of life, including education,\n"
                     "\thealth and safety can be cited, particularly the concept of smart mobility and energy as well as smart retail & logistics\n"
                     "\tcome to mind when thinking of the combination of IoT and digital twins. With an ever-increasing urban population, congestion\n"
                     "\tand pollution become a major concern for both residents and governments and therefore demand innovative technical solutions.\n"
                     "\tThe City of Los Angeles is working on an automated traffic surveillance and control system, which is scheduled for 2018\n"
                     "\tand uses IT components in roads and vehicles that provide accurate data with which to anticipate anticipate and reduce traffic\n"
                     "\tcongestion and pollution. Smart parking, on the other hand, focuses on real-time information provided by drivers in regards\n"
                     "\tto available parking spots, which are represented as digital twins.An example of a smart parking solution can be found in Barcelona,\n"
                     "\twhere a fully operational system is also scheduled for 2018.For both scenarios, one can visualize how digital twins, perhaps first as\n"
                     "\ttraffic lights, then as street segments and finally as cars and parking spots, sufficiently accurately replicate the real-world\n"
                     "\tenvironment. Digital twins emerge from digital objects and allow status updates e.g. on location, utilization or temperature in near real-time.\n"
                     "\tThis kind of transparency is highly beneficial to emerging IoT ecosystems which typically focus on increasing efficiency and optimization.\n";
                     
uint8_t test_pin_0[]={'R','I','D','D','L','E','&','C','O','D','E','R','O','C','K','S','!'};
uint8_t test_pin_1[]={'C','+','+',' ','I','S',' ','N','O','T',' ','C'};
uint8_t test_pin_2[]={'P','E','E','R','T','O','P','E','E','R','E','L','E','C','T','R','O','N','I','C','K','C','A','S','H','S','Y','S','T','E','M'};
uint8_t public_key[10][64]=  {{0}};
uint8_t sha[32] = {0};


#if VALGRIND
/* Call after secret data is written, before first use */
#define MARK_SECRET_DATA(addr, len) VALGRIND_MAKE_MEM_UNDEFINED(addr, len)
/* Call before secret data is freed or to mark non-secret data (public keys or
   signatures) */
#define UNMARK_SECRET_DATA(addr, len) VALGRIND_MAKE_MEM_DEFINED(addr, len)
#else
#define MARK_SECRET_DATA(addr, len)
#define UNMARK_SECRET_DATA(addr, len)
#endif

#define FROMHEX_MAXLEN 512

#define VERSION_PUBLIC 0x0488b21e
#define VERSION_PRIVATE 0x0488ade4

#define DECRED_VERSION_PUBLIC 0x02fda926
#define DECRED_VERSION_PRIVATE 0x02fda4e8





void setup() {

  Serial.begin(57600);
  Serial.println("\n\n\t     ... ...................................... ... \n\n");
  Serial.println("\n\n\t     ... SECURE - ELEMENT TEST SUITE for nRF-DK ... \n\n");

  uint8_t ret = -1 ;
  
  do
  {
      ret = initialize_device(true);
  
  }
  while(ret != 0);

}

void loop() {


  test_secure_store();
  test_store();
  random_tests();
  generate_tests();
  hash_test();
  verify_test();
  

  

  delay(500);

}




void hexify(const uint8_t *hex, unsigned int len) {
  int i;

  for (i = 0; i < len; i++)  {
    static char tmp[4] = {};
    sprintf(tmp, "%02X", hex[i]);
    Serial.write(tmp);
  }

  Serial.println();
}

const uint8_t *fromhex(const char *str) {
  static uint8_t buf[FROMHEX_MAXLEN];
  size_t len = strlen(str) / 2;
  if (len > FROMHEX_MAXLEN) len = FROMHEX_MAXLEN;
  for (size_t i = 0; i < len; i++) {
    uint8_t c = 0;
    if (str[i * 2] >= '0' && str[i * 2] <= '9') c += (str[i * 2] - '0') << 4;
    if ((str[i * 2] & ~0x20) >= 'A' && (str[i * 2] & ~0x20) <= 'F')
      c += (10 + (str[i * 2] & ~0x20) - 'A') << 4;
    if (str[i * 2 + 1] >= '0' && str[i * 2 + 1] <= '9')
      c += (str[i * 2 + 1] - '0');
    if ((str[i * 2 + 1] & ~0x20) >= 'A' && (str[i * 2 + 1] & ~0x20) <= 'F')
      c += (10 + (str[i * 2 + 1] & ~0x20) - 'A');
    buf[i] = c;
  }
  return buf;
}
void test_store()
{
    Serial.println("\n\n\t     ... Running Storage Tests ... \n\n");

    #define STORE_SLOT_1 10
    #define STORE_SLOT_2 (STORE_SLOT_1 + 1)
    #define STORE_SLOT_3 (STORE_SLOT_2 + 1)
    #define STORE_SLOT_4 (STORE_SLOT_3 + 1)
    #define STORE_SLOT_5 (STORE_SLOT_4 + 1)
    #define STORE_SLOT_6 (STORE_SLOT_5 + 1)
    #define SLOT_SIZE 72
    uint8_t write_data[SLOT_SIZE] = {0};
    uint8_t read_data[SLOT_SIZE] = {0};
    
    memset(write_data,0xD0,SLOT_SIZE);
    
    se_write_data(STORE_SLOT_1,write_data,SLOT_SIZE);
    se_read_data(STORE_SLOT_1,read_data,SLOT_SIZE);
    
    for (size_t i = 0; i < SLOT_SIZE; i++)
    {
      Serial.print(read_data[i], HEX);
      Serial.print("-");

    }
    
    Serial.println(""); 
    
    if(memcmp(write_data,read_data,SLOT_SIZE) == 0)
      Serial.println("\n\n\t      Store & Read Success for slot 10  \n\n");
    else
      Serial.println("\n\n\t      Store & Read Fail for slot 10  \n\n");

      

    memset(read_data,0,SLOT_SIZE);
    
    se_write_data(STORE_SLOT_2,write_data,SLOT_SIZE);
    se_read_data(STORE_SLOT_2,read_data,SLOT_SIZE);
    for (size_t i = 0; i < SLOT_SIZE; i++)
    {
      Serial.print(read_data[i], HEX);
      Serial.print("-");
  
    }
    
    Serial.println("");
    
    if(memcmp(write_data,read_data,SLOT_SIZE) == 0)
      Serial.println("\n\n\t      Store & Read Success for slot 11 \n\n");
    else
      Serial.println("\n\n\t      Store & Read Fail for slot 11  \n\n");
    memset(read_data,0,SLOT_SIZE);


    

    se_write_data(STORE_SLOT_3,write_data,SLOT_SIZE);
    se_read_data(STORE_SLOT_3,read_data,SLOT_SIZE);
    for (size_t i = 0; i < SLOT_SIZE; i++)
    {
      Serial.print(read_data[i], HEX);
      Serial.print("-");
  
    }
    
    Serial.println("");
    if(memcmp(write_data,read_data,SLOT_SIZE) == 0)
      Serial.println("\n\n\t      Store & Read Success for slot 12  \n\n");
    else
      Serial.println("\n\n\t      Store & Read Fail for slot 12  \n\n");
    memset(read_data,0,SLOT_SIZE);



    se_write_data(STORE_SLOT_4,write_data,SLOT_SIZE);
    se_read_data(STORE_SLOT_4,read_data,SLOT_SIZE);
    for (size_t i = 0; i < SLOT_SIZE; i++)
    {
      Serial.print(read_data[i], HEX);
      Serial.print("-");
  
    }
    
    Serial.println("");
    if(memcmp(write_data,read_data,SLOT_SIZE) == 0)
      Serial.println("\n\n\t      Store & Read Success for slot 13  \n\n");
    else
      Serial.println("\n\n\t      Store & Read Fail for slot 13  \n\n");
    memset(read_data,0,SLOT_SIZE);



    se_write_data(STORE_SLOT_5,write_data,SLOT_SIZE);
    se_read_data(STORE_SLOT_5,read_data,SLOT_SIZE);
    for (size_t i = 0; i < SLOT_SIZE; i++)
    {
      Serial.print(read_data[i], HEX);
      Serial.print("-");
  
    }
    
    Serial.println("");
    if(memcmp(write_data,read_data,SLOT_SIZE) == 0)
      Serial.println("\n\n\t      Store & Read Success for slot 14  \n\n");
    else
      Serial.println("\n\n\t      Store & Read Fail for slot 14  \n\n");
    memset(read_data,0,SLOT_SIZE);

    se_write_data(STORE_SLOT_6,write_data,SLOT_SIZE);
    se_read_data(STORE_SLOT_6,read_data,SLOT_SIZE);
    for (size_t i = 0; i < SLOT_SIZE; i++)
    {
      Serial.print(read_data[i], HEX);
      Serial.print("-");
  
    }
    
    Serial.println("");
    if(memcmp(write_data,read_data,SLOT_SIZE) == 0)
      Serial.println("\n\n\t      Store & Read Success for slot 15 \n\n");
    else
      Serial.println("\n\n\t      Store & Read Fail for slot 15  \n\n");

    Serial.println("\n\n");
    Serial.println("------------------------------------------------------------------------------------------------------------------");
    
  
}
void verify_test()
{
  Serial.println("\n\n\t     ... Running Signing & Verifying Tests ... \n\n");
  uint8_t signature[64] = {0};
  SE_STATUS rv;
  Serial.println("Signing the digest of the modified text with Device Identity at slot 0...");
  se_sign(0, sha, 0, signature, 0);
  Serial.println("Signature is :");
  hexify(signature, sizeof signature);

  Serial.println("\n\n");
  Serial.println("Verifying the signature with the public key at slot 0 ...");

  if ((rv = se_verify_external(0, public_key[0], 0, sha, 0, signature, 0)) == SE_SUCCESS)
    Serial.println("Verify Success");
  else
  {
    Serial.println("Verify Failed");
    Serial.println(rv, HEX);

  }
  Serial.println("Verifying the signature with the public key at slot 1 to observe failure ...");

  if (se_verify_external(0, public_key[1], 0, sha, 0, signature, 0) == SE_SUCCESS) // This should fail as we you a different pubkey to verify
    Serial.println("Verify Success");
  else
    Serial.println("Verify Failed ");
  Serial.println("\n\n");
  Serial.println("------------------------------------------------------------------------------------------------------------------");

  
   
}

void hash_test()
{
  Serial.println("\n\n\t     ... Using SHA-256 on input text ... \n\n");
  size_t text_size = strlen((const char*)testText);
  testText[2] = 'S';
  SE_STATUS rv;
  uint16_t shaLen = 32;
  Serial.println("\n\n\t\t     ... Input text is  ... \n\n");
  Serial.println((char*)testText);
  Serial.println("\t\tsha256 of the input text is :\n");
  se_get_sha256(testText, sizeof(testText), sha, &shaLen);
  Serial.print("\t\t");
  hexify(sha, sizeof sha);
  Serial.println("\n\n\n\t     ... Swapping First letter 'S' with lower case 's'  ... \n\n");
  testText[2] = 's';

  Serial.println("\n\n\t\t     ... Modified Input text is  ... \n\n");
  Serial.println((char*)testText);
  Serial.println("\t\tsha256 of the input text is :\n");
  se_get_sha256(testText, sizeof(testText), sha, &shaLen);
  Serial.print("\t\t");
  hexify(sha, sizeof sha);
  Serial.println("\n\n");  
  Serial.println("------------------------------------------------------------------------------------------------------------------");

}

void generate_tests()
{
  Serial.println("\n\n\t     ... Running ECC Keypair Generation Tests ... \n\n");

  SE_STATUS rv;
  
  #define NUMBER_OF_KEYS 10


  for (size_t i = 1; i < NUMBER_OF_KEYS; i++)
  {
    se_generate_keypair(i);
  }

  for (size_t i = 0; i < NUMBER_OF_KEYS; i++)
  {
    se_get_pubkey(i,public_key[i],NULL);
  }
  for (size_t i = 0; i < NUMBER_OF_KEYS; i++)
  {
    char temp[48]= {0};

    sprintf(temp,"Public Key in slot %d is :",i);
    Serial.write(temp);
    hexify(public_key[i], sizeof public_key[i]);

  }

  Serial.println("\n\n");  
  Serial.println("------------------------------------------------------------------------------------------------------------------");
  
  
}

void random_tests()
{
  
  Serial.println("\n\n\t     ... Running Random- Generation Tests ... \n\n");

  for (size_t i = 0; i < 5; i++) 
  
  {
  
    uint8_t rnd_num[32] = {0};
    SE_STATUS rnd_status = SE_COM_FAIL;
    rnd_status = se_get_random(rnd_num, 32);
    if (rnd_status == SE_SUCCESS) {
      Serial.println("Random number is :");
      hexify(rnd_num, sizeof rnd_num);
  
    } else {
      Serial.print(F("Random number generation failed:"));
      Serial.println(rnd_status, HEX);
    }
  
  }
  Serial.println("\n\n");
  Serial.println("------------------------------------------------------------------------------------------------------------------");

  
}


void test_secure_store()
{
  
  Serial.println("\n\n\t     ... Running Secure-Storage Tests ... \n\n");
  #define SIZE 48
  #define ZONE 16

  uint8_t data[SIZE] = {0};
  uint8_t write_buffer[SIZE] = {0};
  memset(write_buffer, 0xBE, SIZE / 4);
  memset(write_buffer + (SIZE / 4), 0xEF, SIZE / 4);
  memset(write_buffer + (SIZE / 2), 0xCA, SIZE / 4);
  memset(write_buffer + (3 * SIZE / 4), 0xFE, SIZE / 4);
  SE_STATUS ret = SE_COM_FAIL;
  uint8_t* key_ptr = NULL;
  char temp[48] = {0}; 
  for (size_t i = 0; i < ZONE; i++)
  {
    sprintf(temp,"\nWriting to & Reading from slot %d\n",i);
    Serial.write(temp);
    
    ret = SE_COM_FAIL;

    if(i == 5)
    {
        key_ptr = test_pin_0;  
    }
    
    else if(i == 6)
    {
        key_ptr = test_pin_1;
    }
      
    else if(i > 6  && i < 16)
    
    key_ptr = test_pin_2;
    
    ret = se_authenticate(i,key_ptr);

    if (ret == SE_SUCCESS)
      Serial.println("Authenticate Success");
    else
      Serial.println("Authenticate Failed");

    ret = SE_COM_FAIL;
    ret = se_secure_store(i, write_buffer, SIZE);

    if (ret == SE_SUCCESS)
    {
       // Serial.println("se_secure_store Success");
    }
    else
      Serial.println("se_secure_store Failed");
    ret = SE_COM_FAIL;
    ret = se_secure_read(i, data, SIZE);
    if (ret == SE_SUCCESS)
    {
       //Serial.println("se_secure_read Success");
    }
    else
      Serial.println("se_secure_read Failed");


    for (size_t i = 0; i < SIZE; i++)
    {
      Serial.print(data[i], HEX);
      Serial.print("-");

    }
    Serial.println("");
    memset(data, 0, 48);
  }

  Serial.println("\n\n");
  Serial.println("------------------------------------------------------------------------------------------------------------------");
  
}

int initialize_device(bool personalize)
{
  Serial.println("\n\n\t     ... Initializing Device, Setting the i2c pins ... \n\n");
  se_configure_hardware(0xC0, 0, 400000, 18, 19);
  
  SE_STATUS ret = SE_COM_FAIL;
  ret = se_init(0);
  if (ret == SE_SUCCESS)
  {
    // do nothing
  }
  else
  {
    Serial.println("Secure - Element init failed");
    return -1;  
  }
    
  Serial.println("\n\n");

  
  if(personalize)
  {
    Serial.println("\n\n\t\t     ... Running configure ... \n\n\t\t... This may take up to a minute ...\n");
    ret = se_secure_storage_personalize(false,test_pin_0,test_pin_1,test_pin_2);
    if (ret == SE_SUCCESS)
    {
     // do nothing 
    }
    else
    {
      Serial.println("Personalize Failed");    
      return -1;  
    }
  }
  Serial.println("\n\n");
  Serial.println("------------------------------------------------------------------------------------------------------------------");

  return 0;
 
  
}
