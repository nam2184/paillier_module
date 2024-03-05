#include "paillier_small.hpp"
#include "../detail/math.hpp"
#include "paillier.hpp"
int DEFAULT_KEY_SIZE = 16;

PublicKeySmall::PublicKeySmall(const unsigned int n, const unsigned int g) : n(n), g(g){}
PrivateKeySmall::PrivateKeySmall(const unsigned int n, const unsigned int lambda, const unsigned int gMu) : n(n), lambda(lambda), gMu(gMu) {}

unsigned int PublicKeySmall::encrypt(const unsigned int value)
{
  return 0;
}

unsigned int PrivateKeySmall::decrypt(const unsigned int cipher)
{
  return 0;
}


PaillierKeyGenSmall::PaillierKeyGenSmall() 
{
  unsigned int n;
  unsigned int g;
  unsigned int lambda;
  unsigned int gMu;
  PaillierKeyGenSmall::key_att(DEFAULT_KEY_SIZE, n, g, lambda, gMu);
}

//GENERATE 32 bit ints for testing
unsigned int PaillierKeyGenSmall::generate_prime(unsigned int N){
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

unsigned int PaillierKeyGenSmall::generate_g(unsigned int N, unsigned int n)
{
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<int> distribution(1 << (n-1), n * n - 1);
      
      unsigned int g_value = distribution(gen); 
      while (math::euler_totient(std::pow(n,2), g_value)) 
      {
        g_value = distribution(gen);
      }
      return g_value;
}
 
//GENERATE key of 32bit lengths
void PaillierKeyGenSmall::key_att(unsigned int n_length, unsigned int& n, unsigned int& g, unsigned int& lambda, unsigned int& gMu)
{
    unsigned int p_value = PaillierKeyGenSmall::generate_prime(n_length/2);
    unsigned int q_value = p_value;
    while ((p_value != q_value) && (std::gcd(p_value*q_value,(p_value-1)*(q_value-1)) == 1)) {
      q_value = PaillierKeyGenSmall::generate_prime(n_length/2);
    }

    n = p_value*q_value;
    g = PaillierKeyGenSmall::generate_g(n_length, p_value*q_value);
    lambda = std::lcm(p_value-1, q_value-1);
    
    //chance of overflow so reccomended only 1-20 bit integers to prevent overflows
    auto l = (math::powermod(g, lambda, n*n) - 1) / n;
    gMu = math::modInverse(l, n);
    if (gMu == 0)
    {
      throw std::invalid_argument("gMu is 0");
    }
  
}

PublicKeySmall PaillierKeyGenSmall::generatePublicKey() {
  return PublicKeySmall(n, g); 
}

PrivateKeySmall PaillierKeyGenSmall::generatePrivateKey() {
  return PrivateKeySmall(n, lambda, gMu); 
}





