//Python wrapper
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include "./src/paillier.hpp"


namespace py = pybind11;

struct PyPailier 
{
  PaillierKeyGen key_gen;
  PublicKey public_key;
  PrivateKey private_key;

  PyPailier() : public_key(key_gen.generatePublicKey()), private_key(key_gen.generatePrivateKey(public_key)) {}
  
  py::bytes encrypt(const unsigned char* bytes, int length)
  { 
      BIGNUM *num = BN_new();
      BIGNUM *cipher = BN_new();
   
      if (length == 0) 
      {
        throw std::invalid_argument("Invalid byte array or length");
      }
      BN_bin2bn(bytes, length, num);
      public_key.encrypt(num, cipher);
     
      
      int num_bytes = BN_num_bytes(cipher);
      unsigned char* buffer = new unsigned char[num_bytes];

      int written_bytes = BN_bn2bin(cipher, buffer);

      BN_free(num);
      BN_free(cipher);
      
      return py::bytes(reinterpret_cast<const char*>(buffer), written_bytes);
  }

  py::bytes decrypt(const unsigned char* bytes, int length)
  { 
      BIGNUM *num = BN_new();
      BIGNUM *plaintext = BN_new();
   
      if (length == 0) 
      {
        throw std::invalid_argument("Invalid byte array or length");
      }
      BN_bin2bn(bytes, length, num);
      private_key.decrypt(num, plaintext);
     
      
      int num_bytes = BN_num_bytes(plaintext);
      unsigned char* buffer = new unsigned char[num_bytes];

      int written_bytes = BN_bn2bin(plaintext, buffer);

      BN_free(num);
      BN_free(plaintext);

      return py::bytes(reinterpret_cast<const char*>(buffer), written_bytes);
  }
};



PYBIND11_MODULE(PyPailierModule, m) {
    py::class_<PyPailier> pai(m, "PyPailier");

     pai.def(py::init<>())  // Constructor
        .def("encrypt", &PyPailier::encrypt, py::arg("bytes"), py::arg("length"),
             "Encrypt bytes using public key")
        .def("decrypt", &PyPailier::decrypt, py::arg("bytes"), py::arg("length"),
             "Decrypt bytes using private key");

}
