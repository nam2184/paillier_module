//Run the main
#include <iostream>
#include "./src/paillier_small.hpp"
#include <openssl/crypto.h>
#include <openssl/ssl.h>

int main(int argc, char* argv[]) {
   OpenSSL_add_all_algorithms();
   SSL_load_error_strings(); 
   PaillierKeyGenSmall keygen;
   PublicKeySmall publickey = keygen.generatePublicKey();
   PrivateKeySmall privatekey = keygen.generatePrivateKey();
   std::cout << publickey.encrypt(2) + publickey.encrypt(4);
   std::cout << privatekey.decrypt(publickey.encrypt(2) + publickey.encrypt(4));
    
   RAND_cleanup();
   EVP_cleanup();
   CRYPTO_cleanup_all_ex_data();
   return 0;
}

