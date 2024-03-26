# Variables
CXX := clang++
CXXFLAGS := -O3 -Wall -shared -std=c++11 -fPIC
PYBIND11_INCLUDE := $(shell python3 -m pybind11 --includes)
PYTHON_CONFIG := $(shell python3-config --extension-suffix)
SRC := paillier-module.cpp
MODULE := PyPailierModule$(PYTHON_CONFIG)


# Directories
SRC_DIR = src
DETAIL_DIR = detail

# Object files
MATH_OBJ = $(DETAIL_DIR)/math.o
PAILLIER_OBJ := $(SRC_DIR)/paillier.o
#MAIN_OBJ = main.o

# Libraries
MATH_LIB := $(DETAIL_DIR)/math.so
PAILLIER_LIB := $(SRC_DIR)/paillier.so
# Executable
MODULE_LIB = paillier_module.so

# Compilation and linking
all: $(MODULE)
	INCLUDES = -I/path/to/pybind11/include -I/usr/include/python3.x
$(PAILLIER_LIB): $(PAILLIER_OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $@ $< -lssl -lcrypto

$(MATH_LIB): $(MATH_OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $@ $< -lssl -lcrypto

$(PAILLIER_OBJ): $(SRC_DIR)/paillier.cpp
	$(CXX) -fPIC -c $(CXXFLAGS)  $< -o $@  -lssl -lcrypto

$(MATH_OBJ): $(DETAIL_DIR)/math.cpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@ -lssl -lcrypto

$(MODULE): $(SRC) $(PAILLIER_LIB) $(MATH_LIB)
	$(CXX) $(CXXFLAGS) $(PYBIND11_INCLUDE) -L.  ./$(PAILLIER_LIB) ./$(MATH_LIB)   -fPIC -lstdc++ -W -shared $< -o $@


#$(EXECUTABLE): $(MAIN_OBJ) $(PAILLIER_LIB) $(MATH_LIB)
	#$(CXX) $(CXXFLAGS) -o $@ $< -L. ./src/paillier.so ./detail/math.so -fPIC -lstdc++ -Wl,-rpath

clean:
	rm -f $(PAILLIER_LIB) $(PAILLIER_OBJ) $(MATH_LIB) $(MATH_OBJ)
	rm -f $(TARGET) $(MODULE)


.PHONY: all clean

