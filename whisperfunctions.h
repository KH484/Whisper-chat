#include <iostream>
#include <string>
#include <boost/asio.hpp>

using std::string;

// int portPreference();
void test_certificate_loading();
int setupConnection(int portNumber, string ipAddress, string user1, string user2);
int joinConnection(int portNumber, string ipAddress, string user1, string user2);
void sendMessage(boost::asio::ip::tcp::socket& socket, string name);
void readMessage(boost::asio::ip::tcp::socket& socket, string name);
