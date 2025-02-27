#include <iostream>
#include <string>
#include "whisperFunctions.h"
#include <boost/asio.hpp>               // Boost Asio header files for asynchronous operations
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ssl.hpp>           // Boost SSL support header file
#include <openssl/ssl.h>                // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <vector>

using std::cout;
using std::cin;
using std::string;
using std::vector;

// generate or specifiy a port number for the connection

int portPreference()
{
  int portNumber;
  string portSelection;
  string portChoice;
  vector<int> portNumbersInUse {5000, 5432, 7000, 44950, 44960, 54818};

  cout << "First we will create a port in order to setup the chat. " << '\n';
  cout << '\n' << "Do you want to select one or have one generated automatically? " << '\n';
  cout << "Please select one of the following numbers: " << '\n';
  cout << "1: specify a port " << '\n';
  cout << "2: generate one automatically " << '\n';
  std::getline(cin, portChoice);  // Read user choice as a string
  if (portChoice == "1"){
    cout << '\n' << "Type a port number: ";
    std::getline(cin, portSelection);  // Read user choice as a string
    portNumber = std::stoi(portSelection);
    if (portNumber >= 1024 && portNumber <= 65535){
      return portNumber;
    } else {
      cout << "Please type a number within the correct range" << '\n';
    }
  } else {
    //generate random port number
    portNumber = 6301;
  }
  cout << "port number is " << portNumber << '\n';
  return portNumber;
}

bool chatOpen = true;

void sendMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name) {
  string message;
  cout << name << ": " << std::flush;  // Prompt for the next message
  while (chatOpen) {
    try {
      std::getline(cin, message);
      if (message == "EXIT") {
        message += "\n";                                                                      // Send the shutdown message to the client
        boost::asio::write(ssl_socket, boost::asio::buffer(message));             // Send shutdown signal to the client
        cout << "The chat has ended as 'EXIT' was typed." << '\n';
        cout << "Thank you for using Whisper Chat. Goodbye" << '\n';                      // Exit condition for the loop
        chatOpen = false;
        break;  // Exit the loop
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
      // Check if the received message is the shutdown signal "Shutdown"
      if (message == "EXIT") {
        cout << "Server has ended the chat. Exiting..." << '\n';  // Inform the client that the server ended the chat
        chatOpen = false;  // Close the chat connection
        break;  // Exit the loop and stop receiving messages
      }
      // Clear the prompt line (clear the line and move cursur back to beginning of the line)
      cout << "\n" << "\033[1K\033[F";
      // If message is not the shutdown signal, display it as part of the chat
      cout << "\033[35m" + message + "\033[0m" << '\n';  // Print received message
      // Print the prompt again, so the user knows where to type the next message
      cout << name << ": " << std::flush;  // Prompt for the next message
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
  portNumber = portPreference();
  boost::asio::io_context io_context;                               // Create an io_context object for Boost.Asio for asynchronous operations
  boost::system::error_code ec;                                     // catch any errors arising from the wrong ip address during conversion from string
  boost::asio::ip::tcp::acceptor acceptor(io_context);              // Define the TCP acceptor to listen on a specific endpoint
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress, ec), portNumber);  // uses the string ip address and makes it an ip address to use as the endpoint combined with the port number

  string name;
  while (true){
    try {
      acceptor.open(endpoint.protocol());                            // Open the acceptor with the protocol type (TCP)
      acceptor.bind(endpoint);                                       // Bind the acceptor to the endpoint (IP address and port)
      acceptor.listen();
      cout << "\n" << "Listening on port " << endpoint.port() << "...";
      cout << "\n" << "Please wait to be connected." << '\n';    // Start listening for incoming connections

      boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);                     // Create SSL context with TLSv12
      ctx.use_certificate_file(certFile, boost::asio::ssl::context::pem);                   // Load the certificate and private key
      ctx.use_private_key_file(privateKeyFile, boost::asio::ssl::context::pem);
      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket(io_context,ctx);    // adds the SSL functionality to a regular TCP socket
      acceptor.accept(ssl_socket.lowest_layer());                                           // Block until a connection is accepted then accept the TCP connection using the socket.

      try {
        ssl_socket.handshake(boost::asio::ssl::stream_base::server);                        // Handshake to establish an SSL connection
        cout << "\n" << "SSL Handshake complete.  Your messages will be encrypted" << '\n';         // If handshake is successful
      } catch (const boost::system::system_error& e) {
        cout << "Error during SSL handshake: " << e.what() << '\n';
        return 1; // Exit the function if handshake fails
      }

      cout << "\n" << "Client connected! You can now send messages." << '\n';  // Once connection is established, send a message

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
  return portNumber;
}

int joinConnection(int portNumber, string ipAddress, string user2)
{
  boost::asio::io_context io_context;                                 // Create an io_context object for Boost.Asio for asynchronous operations
  boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);  // Create SSL context
  ctx.load_verify_file(certFile);                                    // Load server certificate
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket(io_context, ctx);  // Create SSL socket
  cout << "Connecting on port" << portNumber;
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress), portNumber);   // uses the string ip address and makes it an ip address to use as the endpoint combined with the port number

  string name;
  cout << "\n" << "Connecting to Whisperer 1. Please wait to be connected..." << '\n';
  try {
    ssl_socket.lowest_layer().connect(endpoint);  // Connect the SSL socket to the server
    ssl_socket.handshake(boost::asio::ssl::stream_base::client);  // Perform SSL handshake
    cout << "SSL Handshake complete.  Your messages will be encrypted." << '\n';  // Handshake completed successfully
    cout << "Connected to server! You can now send messages" << '\n';

    std::thread read_thread(readMessage, std::ref(ssl_socket), user2);     // Start a thread for receiving messages
    std::thread send_thread(sendMessage, std::ref(ssl_socket), user2);     // Start a thread for sending messages

    read_thread.join();                                                     // Join the threads
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

int runProgramme(){
  string user1, user2, userChoice;
  int portNumber;
  string port;
  const string ipAddress = "192.168.1.219";

  cout << "\n" <<"Welcome to" << "\033[94m" << " -*-*-*-*- WHISPER  CHAT -*-*-*-*-  " << "\033[0m";
  cout << "Made for parents to share parenting advice & tips while the little ones are asleep" << '\n';

  cout << "\n" << "Would you like to:" << "\n";
  cout << "1: Start the chat as Whisperer 1 (Server) " << '\n';
  cout << "2: Start the chat as Whisperer 2 (Client) " << '\n';
  cout << "\n" << "Please enter your choice of 1 or 2: " << '\n';
  std::getline(cin, userChoice);

  if (userChoice == "1") {
    cout << '\n' << "What is your name?: " << '\n';                              // function to get the name to use later
    std::getline(cin, user1);
    cout << "Hello " << user1 << '\n';
    cout << '\n' << "Type 'EXIT' at anytime to leave the chat" << '\n';
    setupConnection(portNumber, ipAddress, user1);                // Setup the connection function
  } else if (userChoice == "2") {
      cout << '\n' << "Enter the port number selected by whisperer 1 (Server): " << '\n';
      std::getline(cin, port);
      portNumber = std::stoi(port);                                 // converting a string to an int
      cout << '\n' << "What is your name?: " << '\n';
      std::getline(cin, user2);
      cout << "Hello " << user2 << '\n';
      cout << "\n" << "Type 'EXIT' at anytime to leave the chat" << '\n';
      joinConnection(portNumber, ipAddress, user2);                  // Join the connection function
  } else {
      cout << "You typed something incorrectly.  Please only select 1 or 2" << '\n';
      runProgramme();                                               // error handling run the programme again if wrong number inserted
  }
  return 0;
}
