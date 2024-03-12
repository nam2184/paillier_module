#include "paillier_small.hpp"
#include "../detail/math.hpp"
#include "paillier.hpp"
int DEFAULT_KEY_SIZE = 16;

PublicKeySmall::PublicKeySmall(const unsigned int n, const unsigned int g, const unsigned int r) : n(n), g(g), r(r){}
PrivateKeySmall::PrivateKeySmall(const unsigned int n, const unsigned int lambda, const unsigned int gMu) : n(n), lambda(lambda), gMu(gMu) {}

unsigned int PublicKeySmall::encrypt(const unsigned int value)
{
  unsigned int nsquared = n*n;                      
  unsigned int k1 = math::powermod(g, value, nsquared);
  unsigned int k2 = math::powermod(r, n, nsquared);

  return (k1 * k2) % (nsquared);
}

unsigned int PrivateKeySmall::decrypt(const unsigned int cipher)
{
    unsigned int l = (math::powermod(cipher, lambda, n*n) - 1) / n;
    return (l* gMu) % n;
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
    std::uniform_int_distribution<unsigned int> distribution(1 << (N-1), (1 << N) - 1);

    // generate a random number and make sure it's odd
    int random_number = distribution(generator) | 1;
    while (math::is_prime(random_number) == false) {
      random_number = random_number + 2;
    }
    return random_number;
}

unsigned int PaillierKeyGenSmall::generate_g(unsigned int N, unsigned int n)
{
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<unsigned int> distribution(1 << (N-1), n * n - 1);
      
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
    std::cout << p_value << " and " << q_value << std::endl;
  while (p_value == q_value || std::gcd(p_value*q_value,(p_value-1)*(q_value-1)) != 1 || math::is_prime(q_value) == false) {
      q_value = PaillierKeyGenSmall::generate_prime(n_length/2);
      std::cout << "q_value is : "<< q_value << std::endl; 
    }
    std::cout << "q_value is : outside" << q_value << std::endl;
    n = p_value*q_value;
    g = PaillierKeyGenSmall::generate_g(n_length, p_value*q_value);
    lambda = (p_value-1 *q_value-1)/std::gcd(p_value-1,q_value-1);
    
    //chance of overflow so reccomended only 1-20 bit integers to prevent overflows
    unsigned int l = (math::powermod(g, lambda, n*n) - 1) / n;
    gMu = math::modInverse(l, n);
    if (gMu == 0)
    {
      throw std::invalid_argument("gMu is 0");
    }  
}

PublicKeySmall PaillierKeyGenSmall::generatePublicKey() {
   std::random_device rd;
   std::default_random_engine generator(rd());
   std::uniform_int_distribution<unsigned int> distribution(0, n);
  
   return PublicKeySmall(n, g, distribution(generator)); 
}

PrivateKeySmall PaillierKeyGenSmall::generatePrivateKey() {
  return PrivateKeySmall(n, lambda, gMu); 
}





