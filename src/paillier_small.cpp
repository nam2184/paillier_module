#include "paillier_small.hpp"
#include "../detail/math.hpp"
#include "paillier.hpp"
int DEFAULT_KEY_SIZE = 8;

PublicKeySmall::PublicKeySmall(const unsigned int n, const unsigned int g, const unsigned int r) : n(n), g(g), r(r){}
PrivateKeySmall::PrivateKeySmall(const unsigned int n, const unsigned int lambda, const unsigned int gMu) : n(n), lambda(lambda), gMu(gMu) {}

unsigned int PublicKeySmall::encrypt(const unsigned int value)
{
  std::cout << "r is : " << r << std::endl; 
  unsigned int nsquared = n*n;                      
  unsigned int k1 = math::powermod(g, value, nsquared);
  unsigned int k2 = math::powermod(r, n, nsquared);
  std::cout << "k1 : " << k1 << " k2 : " << k2 << std::endl;
  std::cout << "n_squared : " << nsquared << std::endl;
  
  unsigned int encrypted_value = k1 * k2;
  std::cout << "Before modulus " << encrypted_value << std::endl;
  encrypted_value %= nsquared;

  std::cout << "encrypted : " << encrypted_value << std::endl;
  return  encrypted_value;
}

unsigned int PublicKeySmall::cipher_add(unsigned int a, unsigned int b)
{
    return (a*b) % (n*n);
}
      
unsigned int PublicKeySmall::cipher_cmul(unsigned int a, unsigned int b)
{
  return math::powermod(a, b, n);
}


unsigned int PrivateKeySmall::decrypt(const unsigned int cipher)
{
    unsigned int l = std::floor((math::powermod(cipher, lambda, n*n) - 1) / n);
    std::cout << "l from encrypt is : " << l << std::endl;
    return (l * gMu) % n;
}


PaillierKeyGenSmall::PaillierKeyGenSmall() 
{ 
  PaillierKeyGenSmall::key_att(DEFAULT_KEY_SIZE, n, g, lambda, gMu);
  std::cout << "n after constructor" << n << std::endl; 
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
      std::cout << "length is : " << N << std::endl;
      std::uniform_int_distribution<unsigned int> distribution(2, 3);
      
      unsigned int g_value = distribution(gen); 
      while (!math::euler_totient(n*n, g_value)) 
      {
        g_value = distribution(gen);
      }
      std::cout<< "g_value is : " << g_value << std::endl;
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
    g = PaillierKeyGenSmall::generate_g(n_length/2, p_value*q_value);
    lambda = std::floor((p_value-1)*(q_value-1)/std::gcd(p_value-1,q_value-1));
    std::cout << "powermod check" << math::powermod(g, lambda, n*n) << std::endl;
    //chance of overflow so reccomended only 1-20 bit integers to prevent overflows
    unsigned int l = std::floor((math::powermod(g, lambda, n*n) - 1) / n);
    std::cout << "l is " << l <<  std::endl;
    gMu = math::modInverse(l, n);
    std::cout << "n after modInverse : " << n << std::endl;
    if (gMu == 0)
    {
      throw std::invalid_argument("gMu is 0");
    } 
    std::cout << "gMu is : " << gMu << std::endl;
}

PublicKeySmall PaillierKeyGenSmall::generatePublicKey() {
   std::cout <<"n before random distribution: " << n << std::endl;
   std::cout <<"g before random distribution: " << g << std::endl;
   std::cout <<"lambda before random distribution: " << lambda << std::endl;

   std::random_device rd;
   std::default_random_engine generator(rd());
   std::uniform_int_distribution<unsigned int> distribution(0, 10);
   std::cout << "n is : " << n << std::endl; 
   return PublicKeySmall(n, g, distribution(generator)); 
}

PrivateKeySmall PaillierKeyGenSmall::generatePrivateKey() {
  return PrivateKeySmall(n, lambda, gMu); 
}




