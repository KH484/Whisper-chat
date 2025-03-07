#include <iostream>                      // Standard library header to allow input / output
#include <string>                        // Standard library header to allow string class
#include <boost/asio.hpp>                // accessing the Boost Asio header files
#include <boost/asio/ssl.hpp>            // Boost SSL support header file
#include <openssl/ssl.h>                 // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>              // part of the Open SSL library
#include <openssl/x509.h>                // part of the open SSL library
#include <vector>                        // part of the standard library
#include <vector>                        // part of the standard library
#include <random>                        // part of the standard library
#include <algorithm>                     // part of the standard library

using std::string;                       // shorthand for using string class

// declaring all the functions so the other compiler knows how to link the files

int generateNumber();
int portPreference();
int setupConnection(int portNumber, string ipAddress, string user1);
int joinConnection(int portNumber, string ipAddress, string user2);
void sendMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name);
void readMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name);
int runProgramme();
