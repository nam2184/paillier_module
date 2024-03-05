#ifndef PUBLIC_KEY_H
#define PUBLIC_KEY_H

#include <openssl/bn.h>
class PublicKey {
private:
    BIGNUM *n;
    BIGNUM *g;

public:
    PublicKey(BIGNUM *n, BIGNUM *g);

    // encryption function
    int encrypt(int value);
};
#endif

#ifndef PRIVATE_KEY_H
#define PRIVATE_KEY_H

class PrivateKey {
private:
    BIGNUM *n;
    BIGNUM *lambda;
    BIGNUM *gMu;

public:
    PrivateKey(BIGNUM *n, BIGNUM *lambda, BIGNUM *gMu);
    

    // encryption function
    int decrypt(int cipher);
};
#endif

#ifndef PAILLIER_KEY_GEN_H
#define PAILLIER_KEY_GEN_H

extern int DEFAULT_KEY_SIZE;

class PaillierKeyGen {
private:
    BIGNUM *n;
    BIGNUM *g;
    BIGNUM *lambda;
    BIGNUM *gMu;

public:
    PaillierKeyGen();
    ~PaillierKeyGen();

    // Public key generation function
    PublicKey generatePublicKey();

    // Private key generation function
    PrivateKey generatePrivateKey();
    
    void generate_bprime(unsigned int N, BIGNUM *num, BN_CTX *ctx);
        
    void generate_bg(unsigned int N, BIGNUM *n, BIGNUM *g, BN_CTX *ctx);
  
    void key_batt(BIGNUM *n, BIGNUM *g, BIGNUM *lambda, BIGNUM *gMu, unsigned int N_length); 
};

    
#endif // PAILLIER_KEY_GEN_H

