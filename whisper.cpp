#include <iostream>
#include <string>
#include "whisperFunctions.h"

// Boost Asio header files for asynchronous operations
#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
// Boost SSL support header file
#include <boost/asio/ssl.hpp>
// open SSL header files necessary to handle the SSL functionality
#include <openssl/ssl.h>
#include <openssl/crypto.h>

using std::cout;
using std::cin;
using std::string;

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
    setupConnection(6300, "192.168.1.186");
  } else {
    joinConnection(6300, "192.168.1.186");
  }
  return 0;
}
