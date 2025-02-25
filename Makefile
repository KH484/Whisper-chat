# Compiler and flags
CXX = clang++                                        # The C++ compiler to use (clang++)
CXXFLAGS = -std=c++17                                # Compiler flags to use C++17 standard
BOOST_INCLUDE = /opt/homebrew/opt/boost/include      # Boost header files location
OPENSSL_INCLUDE = /opt/homebrew/opt/openssl@3/include  # OpenSSL header files location
BOOST_LIB = /opt/homebrew/opt/boost/lib              # Boost libraries location
OPENSSL_LIB = /opt/homebrew/opt/openssl@3/lib        # OpenSSL libraries location

# List all source files (whisper.cpp and whisperFunctions.cpp)
SRC = whisper.cpp whisperFunctions.cpp

OUT = whisper                                        # The name of the output executable

# Libraries to link: OpenSSL, Boost, and pthread
LIBS = -lssl -lcrypto -lboost_system -pthread

# Build target: compiles the source into the executable
all: $(OUT)

# Rule to create the executable from the sources
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(BOOST_INCLUDE) -I$(OPENSSL_INCLUDE) $(SRC) -o $(OUT) -L$(BOOST_LIB) -L$(OPENSSL_LIB) $(LIBS)

# Clean up build files: removes the executable
clean:
	rm -f $(OUT)
