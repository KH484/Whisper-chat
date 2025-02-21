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
  string user2;

  cout << "Welcome to Whisper chat, made for parents to share parenting advice & tips" << '\n';

  string userChoice;
  cout << "Would you like to:" << '\n';
  cout << "1: Send a message " << '\n';
  cout << "2: Wait for a message " << '\n';
  cout << "Please enter your choice of 1 or 2: " << '\n';
  cin >> userChoice;
  cout << userChoice;

  if (userChoice == "1") {                                                      // function to get the name to use later
    cout << "What is your name?: " << '\n';
    cin >> user1;
    cout << "Hello " << user1 << '\n';
    setupConnection(portNumber, ipAddress, user1, user2);
    } else {
    string user2;
    cout << "What is your name?: " << '\n';
    cin >> user2;
    cout << "Hello " << user2 << '\n';
    joinConnection(portNumber, ipAddress, user1, user2);
  }
  return 0;
}
