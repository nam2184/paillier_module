#ifndef PRIME_GENERATOR_H
#define PRIME_GENERATOR_H

#include <vector>
#include <openssl/bn.h>
#include <random>
#include <numeric>
#include <openssl/rand.h>

namespace generate {
    unsigned int generate_prime(unsigned int N);
      
    unsigned int generate_g(unsigned int N, unsigned int n);

    //void key_att(BIGNUM *n, BIGNUM *g, BIGNUM *lambda, BIGNUM *gMu, int N_length);
    void key_att(unsigned int N_length, unsigned int& n, unsigned int& g, unsigned int& lambda, unsigned int& gMu);
     
}
#endif
