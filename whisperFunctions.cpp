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
bool chatOpen = true;

void sendMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name) {
  string message;
  while (chatOpen) {
    try {
      std::getline(cin, message);
      if (message == "EXIT") {
        string messageFormat = name + ": " + message + '\n';                              // adding the newline character so getline knows it's the end of the message
        boost::asio::write(ssl_socket, boost::asio::buffer(messageFormat));
        cout << "The chat has ended." << '\n';
        cout << "Thank you for using Whisper Chat. Goodbye" << '\n';                      // Exit condition for the loop
        chatOpen = false;
        break;
      }else {
        string messageFormat = name + ": " + message + '\n';                              // adding the newline character so getline knows it's the end of the message
        boost::asio::write(ssl_socket, boost::asio::buffer(messageFormat));                // Send the message to the client
      }
    } catch (std::exception& error){
      std::cerr << "Send Exception: " << error.what() << '\n';
      break;
    }
  }
}

void readMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name) {
  string message;
  boost::asio::streambuf buffer; // Buffer to hold the incoming data
  while (chatOpen){
    boost::asio::read_until(ssl_socket, buffer, "\n");        // Read data until newline (Enter key) is pressed
    if (buffer.size() > 0) {
    std::istream input_stream(&buffer);                   // Extract the received message from the buffer
    std::getline(input_stream, message);
    cout << "\033[35m" + message + "\033[0m" << '\n';
    cout << name << ": " << std::flush;
    } else {
      cout << "Houston we have a problem. Message not received" << '\n';
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using Whisper Chat. Goodbye" << '\n';
      chatOpen = false;
      break;
    }
  }
}

const string certFile = "SSLfiles/whisper.crt";                     // SSL server certificate
const string privateKeyFile = "SSLfiles/whisper.key";               // SSL Private key
const string publicKeyFile = "SSLfiles/whisper_public.key";         // SSL public key

int setupConnection(int portNumber, string ipAddress, string user1){
  boost::asio::io_context io_context;                               // Create an io_context object for Boost.Asio for asynchronous operations
  boost::system::error_code ec;                                     // catch any errors arising from the wrong ip address during conversion from string
  boost::asio::ip::tcp::acceptor acceptor(io_context);              // Define the TCP acceptor to listen on a specific endpoint
  // portNumber = portPreference();
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress, ec), portNumber);  // uses the string ip address and makes it an ip address to use as the endpoint combined with the port number

  string name;
  while (true){
    try {
      acceptor.open(endpoint.protocol());                            // Open the acceptor with the protocol type (TCP)
      acceptor.bind(endpoint);                                       // Bind the acceptor to the endpoint (IP address and port)
      acceptor.listen();
      cout << "Listening on port " << endpoint.port() << ". Please wait to be connected..." << '\n';    // Start listening for incoming connections

      boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);                     // Create SSL context with TLSv12
      ctx.use_certificate_file(certFile, boost::asio::ssl::context::pem);                   // Load the certificate and private key
      ctx.use_private_key_file(privateKeyFile, boost::asio::ssl::context::pem);
      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket(io_context,ctx);    // adds the SSL functionality to a regular TCP socket
      acceptor.accept(ssl_socket.lowest_layer());                                           // Block until a connection is accepted then accept the TCP connection using the socket.

      try {
        ssl_socket.handshake(boost::asio::ssl::stream_base::server);                        // Handshake to establish an SSL connection
        cout << "SSL Handshake complete.  Your messages will be encrypted" << '\n';         // If handshake is successful
      } catch (const boost::system::system_error& e) {
        cout << "Error during SSL handshake: " << e.what() << '\n';
        return 1; // Exit the function if handshake fails
      }

      cout << "Client connected! You can now send messages." << '\n';  // Once connection is established, send a message

      // Create a thread for reading messages from the server
      std::thread read_thread(readMessage, std::ref(ssl_socket), user1);

      // Create a thread for sending messages to the server
      std::thread send_thread(sendMessage, std::ref(ssl_socket), user1);

      // Join the threads (this makes sure that the main thread waits for the threads to finish)
      read_thread.join();
      send_thread.join();
    }
    catch (const boost::system::system_error& error)                  // creating a variable called error and referencing it in the catch block so we can log and print the error
    {
      cout << "Error: " << error.what() << '\n';
      cout << "Houston we have a problem. Message not received" << '\n';
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using Whisper Chat. Goodbye" << '\n';
      return 1;
      break;                                                          // returning 1 so we know the programme did not complete successfully
    }
  }
  return 0;
}

int joinConnection(int portNumber, string ipAddress, string user2)
{
  boost::asio::io_context io_context;                                 // Create an io_context object for Boost.Asio for asynchronous operations
  boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);  // Create SSL context
  ctx.load_verify_file(certFile);                                    // Load server certificate
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket(io_context, ctx);  // Create SSL socket
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress), portNumber);   // uses the string ip address and makes it an ip address to use as the endpoint combined with the port number

  string name;
  cout << "Connecting to Peer 1. Please wait to be connected..." << '\n';
  try {
    ssl_socket.lowest_layer().connect(endpoint);  // Connect the SSL socket to the server
    ssl_socket.handshake(boost::asio::ssl::stream_base::client);  // Perform SSL handshake
    cout << "SSL Handshake complete.  Your messages will be encrypted." << '\n';  // Handshake completed successfully
    cout << "Connected to server! You can now send messages" << '\n';
    cout << user2 + ": ";

    // Start a thread for receiving messages
    std::thread read_thread(readMessage, std::ref(ssl_socket), user2);

    // Start a thread for sending messages
    std::thread send_thread(sendMessage, std::ref(ssl_socket), user2);

    // Join the threads
    read_thread.join();
    send_thread.join();
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
