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

const string ipAddress = "127.0.0.1";


int main()
{
  string user1, user2, userChoice;
  int portNumber = 6300;

  cout << "Welcome to Whisper chat, made for parents to share parenting advice & tips" << '\n';
  cout << "Would you like to:" << '\n';
  cout << "1: Send a message " << '\n';
  cout << "2: Wait for a message " << '\n';
  cout << "Please enter your choice of 1 or 2: " << '\n';
  cin >> userChoice;
  cout << userChoice;

  if (userChoice == "1") {
    cin.ignore();                                                        // clear the cin so it doesn't display the last input
    cout << "What is your name?: " << '\n';                              // function to get the name to use later
    cin >> user1;
    cout << "Hello " << user1 << '\n';
    cout << "Type 'EXIT' at anytime to leave the chat" << '\n';
    setupConnection(portNumber, ipAddress, user1, user2);                // Setup the connection function
    cin.ignore();
    }if (userChoice == "2") {
      cin.ignore();
      cout << "What is your name?: " << '\n';
      cin >> user2;
      cout << "Hello " << user2 << '\n';
      cout << "Type 'EXIT' at anytime to leave the chat" << '\n';
      joinConnection(portNumber, ipAddress, user1, user2);                  // Join the connection function
      cin.ignore();
    }else {
      cout << "You typed something incorrectly.  Please only select 1 or 2" << '\n';
    }
  return 0;
}
