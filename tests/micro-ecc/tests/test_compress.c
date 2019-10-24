/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include "tests.h"


#ifndef uECC_TEST_NUMBER_OF_ITERATIONS
#define uECC_TEST_NUMBER_OF_ITERATIONS   256
#endif



void test_compress(void) {
  uint8_t public[64];
  uint8_t private[32];
  uint8_t compressed_point[33];
  uint8_t decompressed_point[64];

  int i;
  int c;

  const struct uECC_Curve_t * curves[5];
  int num_curves = 0;
#if uECC_SUPPORTS_secp160r1
  curves[num_curves++] = uECC_secp160r1();
#endif
#if uECC_SUPPORTS_secp192r1
  curves[num_curves++] = uECC_secp192r1();
#endif
#if uECC_SUPPORTS_secp224r1
  curves[num_curves++] = uECC_secp224r1();
#endif
#if uECC_SUPPORTS_secp256r1
  curves[num_curves++] = uECC_secp256r1();
#endif
#if uECC_SUPPORTS_secp256k1
  curves[num_curves++] = uECC_secp256k1();
#endif

  printf("Testing compression and decompression of %d random EC points\n",
         uECC_TEST_NUMBER_OF_ITERATIONS);

  for (c = 0; c < num_curves; ++c) {
    for (i = 0; i < uECC_TEST_NUMBER_OF_ITERATIONS; ++i) {
      printf(".");
      fflush(stdout);

      memset(public, 0, sizeof(public));
      memset(decompressed_point, 0, sizeof(decompressed_point));

      /* Generate arbitrary EC point (public) on Curve */
      TEST_ASSERT_NOT_EQUAL(uECC_make_key(public, private, curves[c]),0);

      /* compress and decompress point */
      uECC_compress(public, compressed_point, curves[c]);
      uECC_decompress(compressed_point, decompressed_point, curves[c]);

      TEST_ASSERT_EQUAL(memcmp(public, decompressed_point, sizeof(public)),0);
    }
    printf("\n");
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_compress);
  return UNITY_END();
}