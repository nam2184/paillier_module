#ifndef PUBLIC_KEY_SMALL_H
#define PUBLIC_KEY_SMALL_H

class PublicKeySmall {
private:
    const unsigned int n;
    const unsigned int g;
    const unsigned int r;

public:
    PublicKeySmall(const unsigned int n, const unsigned int g, const unsigned int r);

    // encryption function
    unsigned int encrypt(const unsigned int value);

    unsigned int cipher_add(unsigned int a, unsigned int b);
      
    unsigned int cipher_cmul(unsigned int a, unsigned int b);

};
#endif

#ifndef PRIVATE_KEY_SMALL_H
#define PRIVATE_KEY_SMALL_H

class PrivateKeySmall {
private:
    const unsigned int n;
    const unsigned int lambda;
    const unsigned int gMu;

public:
    PrivateKeySmall(const unsigned int n, const unsigned int lambda, const unsigned int gMu);
    

    // encryption function
    unsigned int decrypt(const unsigned int cipher);
};
#endif

#ifndef PAILLIER_KEY_GEN_SMALL_H
#define PAILLIER_KEY_GEN_SMALL_H

#include <vector>
#include <openssl/bn.h>
#include <random>
#include <numeric>
#include <openssl/rand.h>

extern int DEFAULT_KEY_SIZE;

class PaillierKeyGenSmall {
private:
    unsigned int n;
    unsigned int g;
    unsigned int lambda;
    unsigned int gMu;

public:
    PaillierKeyGenSmall(); 

    // Public key generation function
    PublicKeySmall generatePublicKey();

    // Private key generation function
    PrivateKeySmall generatePrivateKey();
    
    unsigned int generate_prime(unsigned int N);
      
    unsigned int generate_g(unsigned int N, unsigned int n);

    //void key_att(BIGNUM *n, BIGNUM *g, BIGNUM *lambda, BIGNUM *gMu, int N_length);
    void key_att(unsigned int N_length, unsigned int& n, unsigned int& g, unsigned int& lambda, unsigned int& gMu);
};

    
#endif
