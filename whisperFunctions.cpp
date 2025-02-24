#include <iostream>
#include <string>
#include "whisperFunctions.h"
#include <boost/asio.hpp>                      // Boost Asio header files for asynchronous operations
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ssl.hpp>                 // Boost SSL support header file
#include <openssl/ssl.h>                      // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>

using std::cout;
using std::cin;
using std::string;

// generate or specifiy a port number for the connection
// ideas for improvement include a loop to keep asking if a wrong port number entered and check if the port number entered is within the correct range

// int portPreference()
// {
//   int portNumber;
//   int portChoice;
//   cout << "First we will create a port in order to setup the chat. " << '\n';
//   cout << "Do you want to select one or have one generated automatically? " << '\n';
//   cout << "Please select one of the following numbers: " << '\n';
//   cout << "1: specify a port " << '\n';
//   cout << "2: generate one automatically " << '\n';
//   cin >> portChoice;
//   if (portChoice == 1){
//     cin.ignore();
//     if (portChoice <= 1024 && portChoice > 65535){
//       cin >> portNumber;
//     } else {
//       cout << "Please type a number within the correct range" << '\n';
//     }
//   } else {
//     portNumber = 6300;
//   }
//   cout << "port number is " << portNumber << '\n';
//   return portNumber;
//   cin.ignore();
// }

void sendMessage(boost::asio::ip::tcp::socket& socket, string name) {
  string message;
  while (true) {                                                     // infinite loop (until the user types EXIT)
      std::getline(cin, message);                                    // Read the whole sentence input from user
      boost::asio::write(socket, boost::asio::buffer(message));      // Send the message to the client
      cout << name << ": " << message << '\n';                      // Print the message out with user1's name
      if (message == "EXIT") {
        cout << "The chat has ended." << '\n';
        cout << "Thank you for using Whisper Chat. Goodbye" << '\n';                      // Exit condition for the loop
        break;
      }
  }
}

void readMessage(boost::asio::ip::tcp::socket& socket, string name) {
  string message;
  while (true){
    boost::asio::streambuf buffer;                        // Buffer to hold the incoming data
    boost::asio::read_until(socket, buffer, "\n");        // Read data until newline (Enter key) is pressed
    if (buffer.size() > 0) {
    std::istream input_stream(&buffer);                   // Extract the received message from the buffer
    std::getline(input_stream, message);
    cout << name << ": " << "\033[35m" << message << "\033[0m" << '\n';
    } else {
      cout << "Houston we have a problem. Message not received" << '\n';
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using Whisper Chat. Goodbye" << '\n';
      break;
    }
  }
}


int setupConnection(int portNumber, string ipAddress, string user1, string user2){
  boost::asio::io_context io_context;                               // Create an io_context object for Boost.Asio for asynchronous operations
  boost::system::error_code ec;                                     // catch any errors arising from the wrong ip address during conversion from string
  boost::asio::ip::tcp::acceptor acceptor(io_context);              // Define the TCP acceptor to listen on a specific endpoint
  // portNumber = portPreference();
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress, ec), portNumber);  // uses the string ip address and makes it an ip address to use as the endpoint combined with the port number

  string name;
  while (true){
    try {
      acceptor.open(endpoint.protocol());                             // Open the acceptor with the protocol type (TCP)
      acceptor.bind(endpoint);                                        // Bind the acceptor to the endpoint (IP address and port)
      acceptor.listen();
      cout << "Listening on " << endpoint.port() << "..." << '\n';    // Start listening for incoming connections
      boost::asio::ip::tcp::socket socket(io_context);                // Accept a TCP connection
      acceptor.accept(socket);                                        // Block until a connection is accepted then create a socket
      cout << "Client connected! You can now write a message:" << '\n';  // Once connection is established, send a message
      while (true){
        name = user1;
        readMessage(socket, name);
        name = user2;
        sendMessage(socket, name);
        }
      } catch (const boost::system::system_error& error)                  // creating a variable called error and referencing it in the catch block so we can log and print the error
    {
      cout << "Error: " << error.what() << '\n';
      cout << "Houston we have a problem. Message not received" << '\n';
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using Whisper Chat. Goodbye" << '\n';
      return 1;
      break;                                                          // returning 1 so we know the programme did not complete successfully
    }
    return 0;
  }
}


int joinConnection(int portNumber, string ipAddress, string user1, string user2)
{
  boost::asio::io_context io_context;                                 // Create an io_context object for Boost.Asio for asynchronous operations
  boost::asio::ip::tcp::socket socket(io_context);                    // Creating a socket that links to the asynchronous object
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress), portNumber);   // uses the string ip address and makes it an ip address to use as the endpoint combined with the port number

  cin.ignore();
  string name;
  try {
    socket.connect(endpoint);                             // Connect to the other client
    cout << "Connected to server!" << '\n';
    while (true){
    name = user2;
    readMessage(socket, name);
    name = user1;
    sendMessage(socket, name);
    }
  }
  catch (const boost::system::system_error& error) {
      cout << "Error: " << error.what() << '\n';
      cout << "Houston we have a problem. Message not received" << '\n';
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using Whisper Chat. Goodbye" << '\n';
      return 1;                                             // Return 1 to indicate failure
  }
  return 0;  // Return 0 for success
}
