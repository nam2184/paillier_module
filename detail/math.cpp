#include "math.hpp"
#include "openssl/bn.h"
#include <iostream>

bool math::is_prime(unsigned int x)
{
      if(x <= 1) return false;
      if(x == 2 || x == 3) return true;
      if(x % 2 == 0 || x % 3 == 0) return false;
      if((x - 1) % 6 != 0 && (x + 1) % 6 != 0) return false;
      for(int i = 5; i * i <= x; i += 6)
      {
        if(x % i == 0 || x % (i + 2) == 0) return false;
      }
      std::cout<<"Prime is : " << x << std::endl;
    return true;
}


bool math::euler_totient(int n, int g)
{
  return std::gcd(g,n) == 1; 
}

bool math::euler_btotient(BIGNUM *n, BIGNUM *g, BN_CTX *ctx) 
{
  BIGNUM *one = BN_new();
  BIGNUM *gcd_result = BN_new();
  
  BN_gcd(gcd_result, g, n, ctx);

  return BN_cmp(gcd_result, one) == 1;
}

unsigned int math::powermod(unsigned int a, unsigned int b, unsigned int p) 
{   
    unsigned long number = 1;
    while (b)
    {
        if (b & 1)
            number = number * a % p;
        b >>= 1;
        a = (unsigned long)a * a % p;
    }
    return number; 
} 

// Function for extended Euclidean Algorithm
int math::gcdExtended(unsigned int a, unsigned int b, int* x, int* y)
{
 
    // Base Case
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }
 
    // To store results of recursive call
    int x1, y1;
    int gcd = gcdExtended(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;
 
    return gcd;
}
 

unsigned int math::modInverse(int A, int M)
{
    int x, y;
    int g = gcdExtended(A, M, &x, &y);
    if (g != 1)
        std::cout << "Inverse doesn't exist";
    else {
 
        // m is added to handle negative x
        int res = (x % M + M) % M;
        return res; 
    }
    return 0;
}





