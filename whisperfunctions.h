#include <iostream>
#include <string>
#include <boost/asio.hpp>

using std::string;

// int portPreference();
int setupConnection(int portNumber, string ipAddress, string user1, string user2);
int joinConnection(int portNumber, string ipAddress, string user1, string user2);
void sendMessage(boost::asio::ip::tcp::socket& socket, const string& user1);
