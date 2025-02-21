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
// ideas for improvement include a loop to keep asking if a wrong port number entered
// check if the port number entered is within the correct range
int portPreference()
{
  int portNumber;
  int portChoice;
  cout << "First we will create a port in order to access the chat. " << '\n';
  cout << "Do you want to select one or have one generated automatically? " << '\n';
  cout << "Please select one of the following numbers: " << '\n';
  cout << "1: specify a port " << '\n';
  cout << "2: generate one automatically " << '\n';
  cin >> portChoice;
  if (portChoice == 1){
    cout << "Please type the number for the port you wish to use " << '\n';
    cin >> portNumber;
  } else {
    portNumber = 6300;
  }
  return portNumber;
}

int setupConnection(int portNumber, string ipAddress)
  {
  // portPreference();
  boost::asio::io_context io_context;                               // Create an io_context object for Boost.Asio for asynchronous operations
  boost::system::error_code ec;                                     // catch any errors arising from the wrong ip address during conversion from string
  boost::asio::ip::tcp::acceptor acceptor(io_context);              // Define the TCP acceptor to listen on a specific endpoint
  boost::asio::ip::tcp::resolver resolver(io_context);              // Resolver to resolve IP address or domain name
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress, ec), portNumber);

  // boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ipAddress, std::to_string(portNumber));
  // boost::asio::ip::tcp::endpoint endpoint = *endpoints.begin();      // Get the first endpoint from the resolved list

  try {
    acceptor.open(endpoint.protocol());                             // Open the acceptor with the protocol type (TCP)
    acceptor.bind(endpoint);                                        // Bind the acceptor to the endpoint (IP address and port)
    acceptor.listen();                                              // Start listening for incoming connections
    cout << "Listening on " << endpoint.port() << "..." << '\n';
    boost::asio::ip::tcp::socket socket(io_context);                // Accept a TCP connection
    acceptor.accept(socket);                                        // Block until a connection is accepted then create a socket
    cout << "Client connected!" << '\n';
  }
  catch (const boost::system::system_error& error)                  // creating a variable called error and referencing it in the catch block so we can log and print the error
  {
    cout << "Error: " << error.what() << '\n';
    return 1;                                                       //returning 1 so we know the programme did not complete successfully
  }
  return 0;                                                         // Return 0 to indicate successful connection setup
}

int joinConnection(int portNumber, string ipAddress)
{
    boost::asio::io_context io_context;  // Create an io_context object for Boost.Asio for asynchronous operations
    boost::asio::ip::tcp::socket socket(io_context);  // Creating a socket that links to the asynchronous object
    boost::asio::ip::tcp::resolver resolver(io_context);  // Resolver to resolve IP address or domain name
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ipAddress, std::to_string(portNumber));     // Resolve the IP address and port number
    boost::asio::ip::tcp::endpoint endpoint = *endpoints.begin();    // Get the first endpoint from the resolved list

    try {
        socket.connect(endpoint);                             // Connect to the other client
        cout << "Connected to server!" << '\n';
    }
    catch (const boost::system::system_error& error) {
        cout << "Error: " << error.what() << '\n';
        return 1;                                             // Return 1 to indicate failure
    }
    return 0;  // Return 0 for success
}


//   read operation
//       string message = read_(socket_);
//       cout << message << endl;
//   write operation
//       send_(socket_, "Hello From Server!");
//       cout << "Servent sent Hello message to Client!" << endl;
//   return 0;

//
/*
  function to generate random port number:

  random port number between 1024 and 63353 avoiding:
  vector <int> portsInUse {7000,5000,53781,49576,1024,1026,5432,44950,54687,54688,54671,53790,54679,53934}
  generate a random number
  check if the number is in the vector using count
  then if not in the vector allocate that port but if so then regenerate a randome number
*/
