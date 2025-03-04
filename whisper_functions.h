#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>           // Boost SSL support header file
#include <openssl/ssl.h>                // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <vector>
#include <random>
#include <algorithm>

using std::string;

// int portPreference();
int GenerateNumber();
int PortPreference();
int SetupConnection(int portNumber, string ipAddress, string user1);
int JoinConnection(int portNumber, string ipAddress, string user2);
void SendMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name);
void ReadMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name);
int RunProgramme();
