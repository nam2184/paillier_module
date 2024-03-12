//Run the main
#include <iostream>
#include "./src/paillier_small.hpp"
#include <openssl/crypto.h>
#include <openssl/ssl.h>

int main(int argc, char* argv[]) {
   PaillierKeyGenSmall keygen;
   PublicKeySmall publickey = keygen.generatePublicKey();
   PrivateKeySmall privatekey = keygen.generatePrivateKey();
   std::cout << publickey.encrypt(2) + publickey.encrypt(4) << std::endl;
  
   std::cout << privatekey.decrypt(publickey.encrypt(2) + publickey.encrypt(4)) << std::endl;
     
   return 0;
}

