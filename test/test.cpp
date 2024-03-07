
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../src/paillier_small.hpp"

TEST_CASE("Encryption - Add", "[Paillier]") {
  PaillierKeyGenSmall keygen;
  PublicKeySmall publickey = keygen.generatePublicKey();
  PrivateKeySmall privatekey = keygen.generatePrivateKey();
  REQUIRE(privatekey.decrypt(publickey.encrypt(2) + publickey.encrypt(4)) == 6);
}


