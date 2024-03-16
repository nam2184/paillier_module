#include "paillier.hpp"
#include "../detail/math.hpp"
#include <openssl/bn.h>

int DEFAULT_KEY_SIZE = 3072;

//Public Key constructor
PublicKey::PublicKey(BIGNUM *n, BIGNUM *g) : n(n), g(g){}
PrivateKey::PrivateKey(BIGNUM *n, BIGNUM *lambda, BIGNUM *gMu) : n(n), lambda(lambda), gMu(gMu) {}

//Key gen constructor
PaillierKeyGen::PaillierKeyGen() 
{
      PaillierKeyGen::key_batt(n,g,lambda,gMu,DEFAULT_KEY_SIZE);
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
  //add r
  return PublicKey(n,g);
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
    BIGNUM *one = BN_new();
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
        BN_free(one);
        BN_CTX_free(ctx);
        return;
    }

    BN_set_word(one, 1);

    PaillierKeyGen::generate_bprime(n_length/2, p, ctx);
    BN_copy(q,p);
    BN_mul(n, p, q, ctx);
    BN_sub(ps1, p, one);
    BN_sub(qs1, q, one);
    BN_mul(pq1, ps1, qs1, ctx);
    BN_gcd(gcd, n, pq1, ctx);

    while ((BN_cmp(p,q) != 0) && (BN_cmp(gcd, one) != 0)) {
      PaillierKeyGen::generate_bprime(n_length/2,q, ctx);
      BN_mul(n, p, q, ctx); 
      BN_sub(qs1, q, one);
      BN_mul(pq1, ps1, qs1, ctx);
      BN_gcd(gcd, n, pq1, ctx);
    } 

    BN_free(p);
    BN_free(q); 
    BN_free(qs1); 
    
    PaillierKeyGen::generate_bg(n_length/2, n, g, ctx);
    
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
    BN_free(one);
    BN_free(nsquared);
    BN_CTX_free(ctx); 
}

