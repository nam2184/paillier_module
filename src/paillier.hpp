#ifndef PUBLIC_KEY_H
#define PUBLIC_KEY_H

#include <openssl/bn.h>
#include <openssl/types.h>
class PublicKey {
private:
    const BIGNUM *n;
    const BIGNUM *g;
    BIGNUM *br;
    BN_CTX *ctx;
    BN_MONT_CTX *mont_ctx;

public:
    BIGNUM *nsquared;

public:
    PublicKey(const BIGNUM *n, const BIGNUM *g, const int r);
    ~PublicKey();
    // encryption function
    void encrypt(BIGNUM* value, BIGNUM *cipher);

    void add_cip(BIGNUM *result, BIGNUM* cipher1, BIGNUM* cipher2);
    
    void mul_cons(BIGNUM *result, BIGNUM* cipher, BIGNUM* cons);
};
#endif

#ifndef PRIVATE_KEY_H
#define PRIVATE_KEY_H

class PrivateKey {
private:
    const BIGNUM *n;
    const BIGNUM *lambda;
    const BIGNUM *gMu;
    BIGNUM *one;
    BN_CTX *ctx;
    PublicKey publickey;

public:
    BIGNUM *nsquared;

public:
    PrivateKey(const BIGNUM *n, const BIGNUM *lambda, const BIGNUM *gMu, const PublicKey& publickey);
    ~PrivateKey();

    //dencryption function
    void decrypt(BIGNUM *cipher, BIGNUM *plaintext);
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
    PrivateKey generatePrivateKey(PublicKey& publickey);
    
    void generate_bprime(unsigned int N, BIGNUM *num, BN_CTX *ctx);
        
    void generate_bg(unsigned int N, BIGNUM *n, BIGNUM *g, BN_CTX *ctx);
  
    void key_batt(BIGNUM *n, BIGNUM *g, BIGNUM *lambda, BIGNUM *gMu, unsigned int N_length); 
};

    
#endif // PAILLIER_KEY_GEN_H

