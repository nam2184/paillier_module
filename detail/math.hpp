#ifndef MATH_H
#define MATH_H

#include <numeric>
#include <openssl/bn.h>
#include <iostream>

namespace math 
{
    bool is_prime(unsigned int x);
    bool euler_totient(int n, int g);
    bool euler_btotient(BIGNUM *n, BIGNUM *g, BN_CTX *ctx);
    unsigned int powermod(unsigned int x, unsigned int y, unsigned int p);
    int gcdExtended(unsigned int a, unsigned int b, int* x, int* y);
    unsigned int modInverse(int A, int M);
}
#endif


