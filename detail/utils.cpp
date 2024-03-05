#include "utils.hpp"
#include "math.hpp"

//GENERATE 32 bit ints for testing
unsigned int generate::generate_prime(unsigned int N){
    // create a uniform distribution
    std::random_device rd;
    std::default_random_engine generator(rd());
    //MIN VALUE ON LEFT AND MAX VALUE ON RIGHT
    std::uniform_int_distribution<int> distribution(1 << (N-1), (1 << N) - 1);

    // generate a random number and make sure it's odd
    int random_number = distribution(generator) | 1;
    while (math::is_prime(random_number)) {
      random_number = random_number + 2;
    }
    return random_number;
}

unsigned int generate::generate_g(unsigned int N, unsigned int n)
{
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<int> distribution(1 << (n-1), std::pow(n, 2) - 1);
      
      unsigned int g_value = distribution(gen); 
      while (math::euler_totient(std::pow(n,2), g_value)) 
      {
        g_value = distribution(gen);
      }
      return g_value;
}
 
//GENERATE key of 32bit lengths
void generate::key_att(unsigned int n_length, unsigned int& n, unsigned int& g, unsigned int& lambda, unsigned int& gMu)
{
    unsigned int p_value = generate::generate_prime(n_length/2);
    unsigned int q_value = p_value;
    while ((p_value != q_value) && (std::gcd(p_value*q_value,(p_value-1)*(q_value-1)) == 1)) {
      q_value = generate::generate_prime(n_length/2);
    }

    //BIGNUM *p = BN_new();
    //BIGNUM *q = BN_new();
    //BN_CTX *ctx = BN_CTX_new();

    //BN_set_word(p, p_value);
    //BN_set_word(q, q_value);
    //BN_mul(n, p, q, ctx);

    //BN_free(p);
    //BN_free(q);
    //BN_CTX_free(ctx);
    n = p_value*q_value;
    g = generate::generate_g(n_length, p_value*q_value);
    //BN_set_word(lambda, std::lcm(p_value-1,q_value-1));
    lambda = std::lcm(p_value-1, q_value-1);
    
    //chance of overflow so reccomended only 1-20 bit integers to prevent overflows
    gMu = math::powermod(g, lambda, n); 
}








