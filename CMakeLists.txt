cmake_minimum_required(VERSION 3.12)

project(Paillier-Encryption
        VERSION 0.1)

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the source directories
add_subdirectory(detail)
add_subdirectory(src)
# Add the main executable
add_executable(main main.cpp)

# Link the main executable with paillier_small and math libraries
target_link_libraries(main PRIVATE paillier_small math)

# Specify library search paths for SSL and Crypto
find_library(SSL_LIBRARY ssl)
find_library(CRYPTO_LIBRARY crypto)
if (SSL_LIBRARY AND CRYPTO_LIBRARY)
    target_link_libraries(main PRIVATE ${SSL_LIBRARY} ${CRYPTO_LIBRARY})
else()
    message(FATAL_ERROR "SSL and Crypto libraries not found.")
  endif()
