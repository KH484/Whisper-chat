# Compiler and flags
CXX = clang++                                        # The C++ compiler to use (clang++)
CXXFLAGS = -std=c++17                                # Compiler flags to use C++17 standard
BOOST_INCLUDE = /opt/homebrew/opt/boost/include      # Boost header files location
OPENSSL_INCLUDE = /opt/homebrew/opt/openssl@3/include  # OpenSSL header files location
BOOST_LIB = /opt/homebrew/opt/boost/lib              # Boost libraries location
OPENSSL_LIB = /opt/homebrew/opt/openssl@3/lib        # OpenSSL libraries location

# Include OpenSSL and Boost paths
CPPFLAGS = -I$(OPENSSL_INCLUDE) -I$(BOOST_INCLUDE)    # Include paths for OpenSSL and Boost
LDFLAGS = -L$(OPENSSL_LIB) -L$(BOOST_LIB)            # Library paths for OpenSSL and Boost

# List all source files (whisper.cpp and whisperFunctions.cpp)
SRC = whisper.cpp whisperFunctions.cpp

OUT = whisper                                        # The name of the output executable

# Libraries to link: OpenSSL, Boost, and pthread
LIBS = -lssl -lcrypto -lboost_system -pthread        # Link OpenSSL and Boost System libraries (no need for -lboost_ssl)

# Build target: compiles the source into the executable
all: $(OUT)

# Rule to create the executable from the sources
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC) -o $(OUT) $(LDFLAGS) $(LIBS)

# Clean up build files: removes the executable
clean:
	rm -f $(OUT)
