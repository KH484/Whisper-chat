#include <iostream>
#include <string>
// #include "whisperFunctions.h"

// Boost Asio header files for asynchronous operations
#include <boost/asio.hpp>
// Boost SSL support header file
#include <boost/asio/ssl.hpp>
// open SSL header files necessary to handle the SSL functionality
#include <openssl/ssl.h>
#include <openssl/crypto.h>

using std::cout;
using std::cin;
using std::string;
using boost::asio::ip::tcp;

//quick way to reference boost asio
namespace asio = boost::asio;
//quick way to reference ssl
namespace ssl = boost::asio::ssl;
using boost::asio::ip::tcp;

// A TCP server needs to:
// Create a TCP endpoint (IP + port).
// Create a TCP acceptor to listen on that endpoint.
// Wait for incoming connections and accept them.
// Create a socket for communication once the connection is accepted.
// A TCP client will:
// Create a TCP socket.
// Connect to a server's endpoint (IP + port).
// Send/receive messages to/from the server.

//Think of an acceptor as a sort of passive socket: it just waits for the other endpoint's socket to request for a connection, it is read-only

int main()
{
  string user1;
  cout << "Whisper Chat" << '\n';
  cout << "What is your name?: ";
  cin >> user1;
  cout << "Hello " << user1 << "Welcome to Whisper chat, made for parents to share parenting advice & tips" << '\n';

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
