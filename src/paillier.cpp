#include "paillier.hpp"
#include "../detail/math.hpp"
#include <openssl/bn.h>

int DEFAULT_KEY_SIZE = 1024;

//Public Key constructor
PublicKey::PublicKey(const BIGNUM *n, const BIGNUM *g, const int r) : n(n), g(g), br(BN_new()), ctx(BN_CTX_new()), mont_ctx(BN_MONT_CTX_new()), nsquared(BN_new())
{
    BN_mul(nsquared, n, n, ctx);
    BN_set_word(br, r);
    BN_MONT_CTX_set(mont_ctx, nsquared, ctx);
}

PublicKey::~PublicKey()
{
    BN_free(nsquared);
    BN_CTX_free(ctx);
    BN_free(br);
    BN_MONT_CTX_free(mont_ctx);
}

void PublicKey::encrypt(BIGNUM* value, BIGNUM *cipher)
{
    BIGNUM *k1 = BN_new();
    BIGNUM *k2 = BN_new();


    BN_mod_exp(k1, g, value , nsquared, ctx);
    BN_mod_exp(k2, br, n, nsquared, ctx);
    
    BN_mod_mul_montgomery(cipher, k1, k2, mont_ctx, ctx);

    BN_free(k1);
    BN_free(k2);
}

void PublicKey::add_cip(BIGNUM* result, BIGNUM* cipher1, BIGNUM* cipher2)
{
    BN_mod_mul_montgomery(result, cipher1, cipher2, mont_ctx, ctx);
}

void PublicKey::mul_cons(BIGNUM* result, BIGNUM* cipher1, BIGNUM* cons)
{
    BN_mod_exp(result, cipher1, cons, nsquared, ctx);
}

PrivateKey::PrivateKey(const BIGNUM *n, const BIGNUM *lambda, const BIGNUM *gMu, const PublicKey& publickey) : n(n), lambda(lambda), gMu(gMu), one(nullptr), ctx(BN_CTX_new()), publickey(publickey), nsquared(BN_new())
{
    BN_one(one);
    BN_mul(nsquared, n, n, ctx);
}

PrivateKey::~PrivateKey()
{
   BN_CTX_free(ctx);
   BN_free(one);
}
void PrivateKey::decrypt(BIGNUM *cipher, BIGNUM *plaintext)
{
  BIGNUM *l = BN_new();
  BIGNUM *temp = BN_new();

  BN_mod_exp(temp, cipher, lambda, nsquared, ctx);
  BN_sub(temp, temp, one);
  BN_div(l, NULL, temp, n, ctx);
  BN_mod_mul(plaintext, l, gMu, n, ctx);
  
  BN_free(temp);
  BN_free(l);
}

//Key gen constructor
PaillierKeyGen::PaillierKeyGen() : n(BN_new()), g(BN_new()), lambda(BN_new()), gMu(BN_new())  
{
    PaillierKeyGen::key_batt(n,g,lambda,gMu, DEFAULT_KEY_SIZE);
}
 
//Key Gen destructor
PaillierKeyGen::~PaillierKeyGen() 
{
    BN_free(n);
    BN_free(g);
    BN_free(lambda); 
    BN_free(gMu);
}

PublicKey PaillierKeyGen::generatePublicKey()
{ 
  //r = 2 for now for easier computations
    return PublicKey(n, g, 2);
}

PrivateKey PaillierKeyGen::generatePrivateKey(PublicKey& publickey)
{ 
  //r = 2 for now for easier computations
    return PrivateKey(n, lambda, gMu, publickey);
}

//GENERATE BIGNUM prime i.e. primes bit length 3072
void PaillierKeyGen::generate_bprime(unsigned int N, BIGNUM *num, BN_CTX *ctx)
{   
    BN_GENCB *cb = BN_GENCB_new();
  
    BN_generate_prime_ex2(num, N, 0, NULL, NULL, cb,ctx);
    BN_GENCB_free(cb); 
}

//Generate g by finding multiplicative
void PaillierKeyGen::generate_bg(unsigned int N, BIGNUM *n, BIGNUM *g, BN_CTX *ctx)
{ 
    BIGNUM *nsquared = BN_new();
    BN_mul(nsquared, n, n, ctx);
    do {
      BN_rand(g, N, 0, 1);
    } while(!math::euler_btotient(nsquared, g, ctx)); 
      BN_free(nsquared);
}

//GENERATE key of BIGNUM length
void PaillierKeyGen::key_batt(BIGNUM *n, BIGNUM *g, BIGNUM *lambda, BIGNUM *gMu, unsigned int n_length)
{
    BIGNUM *p = BN_new();
    BIGNUM *pq1 = BN_new();
    BIGNUM *ps1 = BN_new();
    BIGNUM *qs1 = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *gcd = BN_new();
    const BIGNUM *one = BN_value_one();
    BN_CTX *ctx = BN_CTX_new();

    if (!p || !pq1 || !ps1 || !qs1 || !q || !gcd || !one || !ctx) 
    {
        // Handle memory allocation failure
        // For example, free already allocated memory and return
        BN_free(p);
        BN_free(pq1);
        BN_free(ps1);
        BN_free(qs1);
        BN_free(q);
        BN_free(gcd);
        BN_free(const_cast<BIGNUM*>(one));
        BN_CTX_free(ctx);
        return;
    }

    generate_bprime(n_length/2, p, ctx);
    BN_copy(q,p);
    BN_mul(n, p, q, ctx);
    BN_sub(ps1, p, one);
    BN_sub(qs1, q, one);
    BN_mul(pq1, ps1, qs1, ctx);
    BN_gcd(gcd, n, pq1, ctx);

    while ((BN_cmp(p,q) != 0) && (BN_cmp(gcd, one) != 0)) {
      generate_bprime(n_length/2,q, ctx);
      BN_mul(n, p, q, ctx); 
      BN_sub(qs1, q, one);
      BN_mul(pq1, ps1, qs1, ctx);
      BN_gcd(gcd, n, pq1, ctx);
    } 

    BN_free(p);
    BN_free(q); 
    BN_free(qs1); 
    
    generate_bg(n_length/2, n, g, ctx);
    
    //Lambda from finding the lcm of (p-1,q-1)
    BN_div(lambda, NULL, pq1, gcd, ctx);
    BN_free(pq1);
    BN_free(gcd);
    
    BIGNUM *nsquared = BN_new();
    BN_mul(nsquared, n, n, ctx); 
    
    //Find gMu through modular multiplicative inverse
    BIGNUM *temp = BN_new();
    BN_mod_exp(temp, g, lambda, nsquared, ctx);
    BN_sub(ps1, temp, one);
    
    BN_div(temp, NULL, ps1, n, ctx);
    BN_mod_inverse(gMu, temp, n, ctx);
  
    BN_free(temp);
    BN_free(const_cast<BIGNUM*>(one));
    BN_free(nsquared);
    BN_CTX_free(ctx); 
}

