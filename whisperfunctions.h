#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>           // Boost SSL support header file
#include <openssl/ssl.h>                // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>
#include <openssl/x509.h>

using std::string;

// int portPreference();
int setupConnection(int portNumber, string ipAddress, string user1);
int joinConnection(int portNumber, string ipAddress, string user2);
void sendMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name);
void readMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name);
int runProgramme();
