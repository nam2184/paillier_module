# Add the math library
add_library(math OBJECT math.cpp)

# Link the math library with SSL and Crypto
target_link_libraries(math PRIVATE ssl crypto)

