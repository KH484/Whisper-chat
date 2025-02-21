#include <iostream>
#include <string>
#include "whisperFunctions.h"
#include <boost/asio.hpp>               // Boost Asio header files for asynchronous operations
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ssl.hpp>           // Boost SSL support header file
#include <openssl/ssl.h>                // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>

using std::cout;
using std::cin;
using std::string;

//think about how to make global variables
string ipAddress = "192.168.1.186";
int portNumber = 6300;

int main()
{
  string user1;
  cout << "Whisper Chat" << '\n';
  cout << "What is your name?: ";
  cin >> user1;
  cout << "Hello " << user1 << "Welcome to Whisper chat, made for parents to share parenting advice & tips" << '\n';

  string userChoice;
  cout << "Would you like to:" << '\n';
  cout << "1: Send a message " << '\n';
  cout << "2: Wait for a message " << '\n';
  cout << "Please enter your choice of 1 or 2: " << '\n';
  cin >> userChoice;
  cout << userChoice;

  if (userChoice == "1") {
    setupConnection(portNumber, ipAddress);
  } else {
    joinConnection(portNumber, ipAddress);
  }
  return 0;
}
