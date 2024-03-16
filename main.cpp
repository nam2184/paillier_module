//Run the main
#include <iostream>
#include "./src/paillier_small.hpp"
#include <openssl/crypto.h>
#include <openssl/ssl.h>

int main() {
   PaillierKeyGenSmall keygen;
   PublicKeySmall publickey = keygen.generatePublicKey();
   PrivateKeySmall privatekey = keygen.generatePrivateKey();
   auto a = publickey.encrypt(17);
   auto b = publickey.encrypt(18);
   unsigned int c = publickey.cipher_add(a, b);
   std::cout << "CIphertext a: "  << a << std::endl;
   std::cout << "CIphertext b: "  << b << std::endl;
   std::cout << "Ciphertotal : "  << c << std::endl;
   auto res = privatekey.decrypt(c); 
   std::cout <<"Decrypted key : " << res << std::endl;
    
   return 0;
}

