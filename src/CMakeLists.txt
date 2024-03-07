# Add the paillier_small library
add_library(paillier_small OBJECT paillier_small.cpp)

# Link the paillier_small library with SSL and Crypto
target_link_libraries(paillier_small PRIVATE ssl crypto)

