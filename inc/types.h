#ifndef __SECURE_ELEM_ABSTR_TYPES__
#define __SECURE_ELEM_ABSTR_TYPES__

#include <stdint.h>
#include <stdbool.h>

#ifndef U8
typedef unsigned char   U8;
#endif
#ifndef U16
typedef unsigned short U16;
#endif

#define ESTABLISH_SCI2C 0x00
#define RESUME_SCI2C    0x01
#define ESTABLISH_ENCRYPTED_SCI2C 0x02

typedef enum{
    SE_SUCCESS      = 0x00,
    SE_COM_FAIL     = 0xF7,
    SE_GEN_FAIL     = 0xF8,
    SE_UNKNOWN      = 0xFF,
    SE_VERIFY_SUCCESS = 0xF6,
    SE_VERIFY_FAIL = 0xF5,
} SE_STATUS ;

#endif
