CXX = gcc -fPIC
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic 

# Directories
SRC_DIR = src
DETAIL_DIR = detail

# Object files
MATH_OBJ = $(DETAIL_DIR)/math.o
PAILLIER_OBJ = $(SRC_DIR)/paillier_small.o
MAIN_OBJ = main.o

# Libraries
MATH_LIB = $(DETAIL_DIR)/math.so
PAILLIER_LIB = $(SRC_DIR)/paillier_small.so
# Executable
EXECUTABLE = main

# Compilation and linking
all: $(EXECUTABLE)

$(EXECUTABLE): $(MAIN_OBJ) $(PAILLIER_LIB) $(MATH_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $< -L. ./src/paillier_small.so ./detail/math.so -fPIC -lstdc++ -Wl,-rpath

$(MAIN_OBJ): ./main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PAILLIER_LIB): $(PAILLIER_OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $@ $< -lssl -lcrypto

$(MATH_LIB): $(MATH_OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $@ $< -lssl -lcrypto

$(PAILLIER_OBJ): $(SRC_DIR)/paillier_small.cpp
	$(CXX) -fPIC -c $(CXXFLAGS)  $< -o $@ 

$(MATH_OBJ): $(DETAIL_DIR)/math.cpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@ -lssl -lcrypto

clean:
	rm -f $(EXECUTABLE) $(MAIN_OBJ) $(PAILLIER_LIB) $(PAILLIER_OBJ) $(MATH_LIB) $(MATH_OBJ)

.PHONY: all clean

