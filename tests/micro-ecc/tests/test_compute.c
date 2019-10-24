/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include "tests.h"

void test_compute(void) {
  int i;
  uint8_t private[32];
  uint8_t public[64];
  uint8_t public_computed[64];

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

  printf("Testing 256 random private key pairs\n");
  for (c = 0; c < num_curves; ++c) {
    for (i = 0; i < 256; ++i) {
      printf(".");
      fflush(stdout);

      memset(public, 0, sizeof(public));
      memset(public_computed, 0, sizeof(public_computed));

      TEST_ASSERT_NOT_EQUAL(uECC_make_key(public, private, curves[c]),0);

      TEST_ASSERT_NOT_EQUAL(uECC_compute_public_key(private, public_computed, curves[c]),0);

      TEST_ASSERT_EQUAL(memcmp(public, public_computed, sizeof(public)), 0);
    }

    printf("\n");
    printf("Testing private key = 0\n");

    memset(private, 0, sizeof(private));
    TEST_ASSERT_EQUAL(uECC_compute_public_key(private, public_computed, curves[c]),0);
    // if (success) {
    //   printf("uECC_compute_public_key() should have failed\n");
    // }
    printf("\n");
  }

}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_compute);
  return UNITY_END();
}