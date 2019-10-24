/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include "tests.h"

void test_ecdh(void) {
  int i, c;
  uint8_t private1[32] = {0};
  uint8_t private2[32] = {0};
  uint8_t public1[64] = {0};
  uint8_t public2[64] = {0};
  uint8_t secret1[32] = {0};
  uint8_t secret2[32] = {0};

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

      TEST_ASSERT_NOT_EQUAL(uECC_make_key(public1, private1, curves[c]), 0);
      TEST_ASSERT_NOT_EQUAL(uECC_make_key(public2, private2, curves[c]), 0);

      TEST_ASSERT_NOT_EQUAL(uECC_shared_secret(public2, private1, secret1, curves[c]), 0);
      TEST_ASSERT_NOT_EQUAL(uECC_shared_secret(public1, private2, secret2, curves[c]), 0);

      TEST_ASSERT_EQUAL(memcmp(secret1, secret2, sizeof(secret1)),0);
    }
    printf("\n");
  }

}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_ecdh);
  return UNITY_END();
}