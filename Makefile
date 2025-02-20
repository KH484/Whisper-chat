# Compiler and flags
CXX = clang++                                          # The C++ compiler to use (clang++)
CXXFLAGS = -std=c++17                                  # Compiler flags to use C++17 standard
BOOST_INCLUDE = /opt/homebrew/opt/boost/include        # Boost header files location
OPENSSL_INCLUDE = /opt/homebrew/opt/openssl@3/include  # OpenSSL header files location
BOOST_LIB = /opt/homebrew/opt/boost/lib                # Boost libraries location
OPENSSL_LIB = /opt/homebrew/opt/openssl@3/lib          # OpenSSL libraries location
SRC = whisper.cpp                                      # The source file for your project
OUT = whisper                                          # The name of the output executable
LIBS = -lssl -lcrypto -lboost_system -pthread          # Libraries to link: OpenSSL, Boost, and pthread

# Build target: compiles the source into the executable
all: $(OUT)

# Rule to create the executable from the source
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(BOOST_INCLUDE) -I$(OPENSSL_INCLUDE) -I$(OPENSSL_INCLUDE)/openssl $(SRC) -o $(OUT) -L$(BOOST_LIB) -L$(OPENSSL_LIB) $(LIBS)

# Clean up build files: removes the executable
clean:
	rm -f $(OUT)                     # Removes the compiled executable