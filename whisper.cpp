#include <iostream>
#include <string>
#include "whisperFunctions.h"
#include <boost/asio.hpp>               // Boost Asio header files for asynchronous operations
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ssl.hpp>           // Boost SSL support header file
#include <openssl/ssl.h>                // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>
#include <openssl/x509.h>

using std::cout;
using std::cin;
using std::string;

const string ipAddress = "127.0.0.1";

int main()
{
  string user1, user2, userChoice;
  int portNumber = 6300;

  cout << "\n" <<"Welcome to" << "\033[94m" << " -*-*-*-*- WHISPER  CHAT -*-*-*-*-  " << "\033[0m";
  cout << "Made for parents to share parenting advice & tips while the little ones are asleep" << '\n';
  cout << "\n" << "Would you like to:" << "\n";
  cout << "1: Start the chat as Whisperer 1 (Server) " << '\n';
  cout << "2: Start the chat as Whisperer 2 (Client) " << '\n';
  cout << "\n" << "Please enter your choice of 1 or 2: " << '\n';
  std::getline(cin, userChoice);

  if (userChoice == "1") {                                                       // clear the cin so it doesn't display the last input
    cout << '\n' << "What is your name?: " << '\n';                              // function to get the name to use later
    std::getline(cin, user1);
    cout << "Hello " << user1 << '\n';
    cout << '\n' << "Type 'EXIT' at anytime to leave the chat" << '\n';
    setupConnection(portNumber, ipAddress, user1);                // Setup the connection function
    }if (userChoice == "2") {
      cout << "What is your name?: " << '\n';
      std::getline(cin, user2);
      cout << "Hello " << user2 << '\n';
      cout << "\n" << "Type 'EXIT' at anytime to leave the chat" << '\n';
      joinConnection(portNumber, ipAddress, user2);                  // Join the connection function
    }else {
      cout << "You typed something incorrectly.  Please only select 1 or 2" << '\n';
    }
  return 0;
}
