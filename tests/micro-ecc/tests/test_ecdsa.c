/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include "tests.h"

void test_ecdsa(void) {
  int i, c;
  uint8_t private[32] = {0};
  uint8_t public[64] = {0};
  uint8_t hash[32] = {0};
  uint8_t sig[64] = {0};

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

  printf("Testing 256 signatures\n");
  for (c = 0; c < num_curves; ++c) {
    for (i = 0; i < 256; ++i) {
      printf(".");
      fflush(stdout);
      TEST_ASSERT_NOT_EQUAL(uECC_make_key(public, private, curves[c]),0);
      memcpy(hash, public, sizeof(hash));
      TEST_ASSERT_NOT_EQUAL(uECC_sign(private, hash, sizeof(hash), sig, curves[c]),0);
      TEST_ASSERT_NOT_EQUAL(uECC_verify(public, hash, sizeof(hash), sig, curves[c]),0);
    }
    printf("\n");
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_ecdsa);
  return UNITY_END();
}
