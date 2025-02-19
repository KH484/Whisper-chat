#include <iostream>
#include <string>
// #include "whisperFunctions.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using std::cout;
using std::cin;
using std::string;

// namespace asio = boost::asio;
// namespace ssl = boost::asio::ssl;
// using boost::asio::ip::tcp;

// server sets up the port and listens
// client connects to the port and receives
// print message when connection is established

int main()
{
  string user1;
  cout << "Whisper Chat" << '\n';
  cout << "What is your name?: ";
  cin >> user1;
  cout << "Hello " << user1 << "Where busy parents can chat" << '\n';

  int userChoice;
  cout << "Would you like to:" << '\n';
  cout << "1: Send a message " << '\n';
  cout << "2: Wait to receive a message" << '\n';
  cout << "Please enter your choice of 1 or 2: " << '\n';
  cin >> userChoice;
  cout << userChoice;

  //if userChoice is 1 run function 1
  // if userChoice is 2 run function 2

  return 0;
}
