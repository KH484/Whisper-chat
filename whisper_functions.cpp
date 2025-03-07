#include <iostream>                                                              // Standard library header to allow input / output
#include <string>                                                                // Standard library header to allow string class
#include "whisper_functions.h"                                                   // Loading my header file whisper_functions
#include <boost/asio.hpp>                                                        // Boost Asio header files for asynchronous operations
#include <boost/asio/ip/address.hpp>                                             // Boost Asio header files for making an ip address object
#include <boost/asio/ssl.hpp>                                                    // Boost SSL support header file
#include <openssl/ssl.h>                                                         // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>                                                      // open SSL header files necessary to handle the SSL functionality
#include <openssl/x509.h>                                                        // open SSL header files necessary to handle the SSL functionality
#include <vector>                                                                // Standard library header to allow me to make vectors
#include <random>                                                                // Standard library header to allow me to make use of the random generator
#include <algorithm>                                                             // Standard library header to allow me to make use of the find algorithm


using std::cout;                                                                 // Enabling a shortcut for cout
using std::cin;                                                                  // Enabling a shortcut for cin
using std::string;                                                               // Enabling a shortcut for string
using std::vector;                                                               // Enabling a shortcut for vector

// Function below to generate a random number avoiding ports already in use

int generateNumber(){                                                            // the function will return an int value
  static std::random_device rd;                                                  // Initialize random_device to get a random seed
  static std::mt19937 gen(rd());                                                 // Initialize the Mersenne Twister pseudo-random number generator with the seed
  static std::uniform_int_distribution<> dist(1024, 65535);                      // Define a uniform distribution in the range only within port range you should use

  vector<int> portNumbersInUse {5000, 5432, 7000, 44950, 44960, 54818};          // initialising a vector with the ports already in use

  int randomNumber;                                                              // initialising an int variable called randomNumber

  do {                                                                           // a do while loop to ensure it will keep generating a number until it finds one that is not already in use
    randomNumber = dist(gen);                                                    // generate the number
  } while (std::find(portNumbersInUse.begin(), portNumbersInUse.end(), randomNumber) != portNumbersInUse.end()); // if it does not return vector.end the number was found
  return randomNumber;                                                              // when the loop evaluates to false the port number will be valid and it will return the number
}

// This function is to generate or specifiy a port number for the connection and then it validates the selected port

int portPreference(){                                                           // The function will return an int value
  vector<int> portNumbersInUse {5000, 5432, 7000, 44950, 44960, 54818};         // initialising a vector with the ports in use listed
  string portSelection, userChoice;                                             // initialising some string variables
  int portNumber;                                                               // initialising an int variable for the portnumber to be used to setup the connection

  cout << "First we will create a port in order to setup the chat. " << '\n';                   // menu prints out
  cout << '\n' << "Do you want to select one or have one generated automatically? " << '\n';

  while(true) {                                                                  // while loop - I want the programme to continue to loop until I find a portnumber that is valid
    cout << "Please select one of the following numbers: " << '\n';              // menu options
    cout << "1: specify a port " << '\n';
    cout << "2: generate one automatically " << '\n';
    std::getline(cin, userChoice);                                               // Take the user's input and record it as the value for the variable userChoice
    if (userChoice == "1"){                                                      // if the user's input evaluates to the string "1"
      cout << '\n' << "Type a port number: ";                                    // output statement
      std::getline(cin, portSelection);                                          // Take the user's input and record it as the value for the variable portSelection
      try {                                                                      // try catch block - this is to do with the conversion from string to interger.  If the user was to input a letter other than a number the conversion would throw an exception.
        portNumber = std::stoi(portSelection);                                   // try to convert the string number to an integer
        if (portNumber < 1024 || portNumber > 65535){                            // checking if port specified by the user is within correct range
          cout << "Please type a number within the correct range (1024 - 65535)" << '\n';                           // if not - this will print to let the user know it is incorrect
          continue;                                                                                                 // continue statement just to make sure that the loop will run again
        }
        if (std::find(portNumbersInUse.begin(), portNumbersInUse.end(), portNumber) != portNumbersInUse.end()) {    // if the find algorithm does not return (vector.end()) the number was found
          cout << "Port number is already in use. Try again or generate a port automatically."<< "\n";              // print statement to the user to let them know it was not valid
          continue;                                                                                                 // loops around again
        }
        return portNumber;                                                                                          // valid unused port number so return this variable to use later in the setup connection function
      } catch (std::invalid_argument const& ex) {                                                                   // catch the exceptions if the user types something other than a number and the conversion can't take place
        cout << "You have entered something incorrectly. Please only type numbers";                                 // print statement to the user
      }
    } else if (userChoice == "2") {                                                                                 // if user selects generate a port instead then the generate number function will run instead
      portNumber = generateNumber();                                                                                // return the value of generatenumber function and put it in the variable portnumber
      cout << "port number is " << portNumber << '\n';                                                              // print statement to the user
      return portNumber;                                                                                            // making this avaiable through the return statement so I can to pass as a parameter to setup connection function
    } else {                                                                                                        // catching mistakes in user input - only select 1 or 2
      cout << "Please only enter 1 or 2";
    }
  }
}

bool chatOpen = true;                                                                                               // setting up a boolean variable chatOpen to equal a true value

// function to send the messages

void sendMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name) {                 // function called send message that has no return value so is void. Passing in the socket and a string name.  The computer will replace this with either user 1 or user 2 depending on whether setup connection is run or join connection is run.
  string message;                                                                                                   // message variable initialised
  while (chatOpen) {                                                                                                // while loop as I want this function to continuously run to allow multiple messages
    try {                                                                                                           // try block - if there is an error sending then it will be caught in the catch block
      cout << name << ": " << std::flush;                                                                           // Prompt for the first message
      std::getline(cin, message);                                                                                   // take the whole line of user input including spaces and put that into the variable message
      if (message == "EXIT") {                                                                                      // if message evaluates to the word "EXIT"
        message += "\n";                                                                                            // Format the message with the newline character as we are using read until the new line character
        boost::asio::write(ssl_socket, boost::asio::buffer(message));                                               // Send the message to the other peer
        chatOpen = false;                                                                                           // Turn the loop condition to false
        cout << "The chat has ended as 'EXIT' was typed." << '\n';                                                  // print out a message to the user to let them know
        cout << "Thank you for using " << "\033[36m" << "Whisper Chat" << "\033[0m" << "."  << "Goodbye" << '\n';   // Exit message for the programme including colour effects
        break;                                                                                                      // Exit the programme and the loop
      }else {                                                                                                       // otherwise if exit was not typed
        string messageFormat = name + ": " + message + '\n';                                                        // formatting the message with the name of the sender and the message and the new line character
        boost::asio::write(ssl_socket, boost::asio::buffer(messageFormat));                                         // Send the message to the client using the SSL socket
      }
    } catch (std::exception& error){                                                                                // catching exceptions / errors in the sending of the messages
      std::cerr << "Send Exception: " << error.what() << '\n';
      break;                                                                                                        // stop the programme if an error occurs
    }
  }
}

//function to read the messages

void readMessage(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_socket, string name) {                  // function to read the message - it doesn't return a value so it is void.  Passing in SSL socket and the string name.
  string message;                                                                                                    // initialising string variable called message
  boost::asio::streambuf buffer;                                                                                     // Setting up a buffer to hold the incoming data
  while (chatOpen){                                                                                                  // setting up a while loop using the boolean value
    boost::asio::read_until(ssl_socket, buffer, "\n");                                                               // Read data from the buffer until newline character (this is why we sent a new line character in the send message function)
    if (buffer.size() > 0) {                                                                                         // if the buffer holds some data (checking if there is a message)
    std::istream input_stream(&buffer);                                                                              // These two lines extract the received message from the buffer
    std::getline(input_stream, message);
      if (message == "EXIT") {                                                                                       // if the EXIT message is received
        chatOpen = false;                                                                                            // change the boolean value to false and stop the loop
        cout << "EXIT was typed. Exiting the programme..." << '\n';                                                  // inform the client that the chat will be ending
        cout << "Thank you for using " << "\033[36m" << "Whisper Chat" << "." << "\033[0m" << "Goodbye" << '\n';     // goodbye message
        break;                                                                                                       // Exit the loop and stop receiving messages
      }
      cout << "\n" << "\033[1K\033[F";                                                                               // Clear the prompt line (clear the line and move cursur back to beginning of the line)
      cout << "\033[35m" + message + "\033[0m" << '\n';                                                              // Print received message using a different colour font
      cout << name << ": " << std::flush;                                                                            // Print the prompt again, so the user knows where to type the next message
    } else {                                                                                                         // if the buffer did not contain any data
      cout << "Houston we have a problem. Message not received" << '\n';                                             // print messages to the user to let them know the chat will be ending
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using " << "\033[36m" << "Whisper Chat" << "." << "\033[0m" << "Goodbye" << '\n';
      chatOpen = false;                                                                                              // change the loop condition to false
      break;                                                                                                         // stop the loop and exit
    }
  }
}

// SSL certificates and keys

const string certFile = "SSLfiles/whisper.crt";                                                                      // SSL server certificate (putting into variables so I can reference them easily)
const string privateKeyFile = "SSLfiles/whisper.key";                                                                // SSL Private key
const string publicKeyFile = "SSLfiles/whisper_public.key";                                                          // SSL public key

// function to setup the connection

int setupConnection(int portNumber, string ipAddress, string user1){                                                // setting up a function called setup connection passing in the string name, the port number that was create and the ip address
  portNumber = portPreference();                                                                                    // run the portpreference function and put the return value into the variable portNumber
  boost::asio::io_context io_context;                                                                               // Create an io_context object for Boost.Asio to enable asynchronous operations
  boost::system::error_code ec;                                                                                     // catch any errors arising from the wrong ip address during conversion from string using Boost Asio error handling
  boost::asio::ip::tcp::acceptor acceptor(io_context);                                                              // Define the TCP acceptor and pass in the io context to allow asynchronous operations
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress, ec), portNumber);                // uses the string ip address and converts it an ip address object to use as the endpoint combined with the port number

    try {                                                                                                           // using a try block in case of errors or exceptions
      acceptor.open(endpoint.protocol());                                                                           // Open the acceptor with the protocol type (TCP)
      acceptor.bind(endpoint);                                                                                      // Bind the acceptor to the endpoint (IP address and port)
      acceptor.listen();                                                                                            // Start listening for incoming connections
      cout << "\n" << "Listening on port " << endpoint.port() << "...";                                             // keep the user updated with what's happening
      cout << "\n" << "Please wait to be connected." << '\n';

      boost::asio::ssl::context SSLContext(boost::asio::ssl::context::tlsv12);                                      // Create SSL context using TLSv1.2
      SSLContext.use_certificate_file(certFile, boost::asio::ssl::context::pem);                                    // these two lines load the certificate and private key
      SSLContext.use_private_key_file(privateKeyFile, boost::asio::ssl::context::pem);
      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket(io_context,SSLContext);                     // adds the SSL functionality to a regular TCP socket
      acceptor.accept(ssl_socket.lowest_layer());                                                                   // Block until a connection is accepted then accept the TCP connection (the lowest layer) using the socket.

      try {                                                                                                         // try block to handle errors or exceptions
        ssl_socket.handshake(boost::asio::ssl::stream_base::server);                                                // Handshake with client to establish an SSL connection
        cout << "\n" << "SSL Handshake complete.  Your messages will be encrypted" << '\n';                         // If handshake is successful this line will print to let the user know
      } catch (const boost::system::system_error& e) {                                                              // catch any errors
        cout << "Error during SSL handshake: " << e.what() << '\n';                                                 // let use know there were errors while trying to connect
        return 1;                                                                                                   // Exit the function if handshake fails and return 1 - a sign of programme failure
      }

      cout << "\n" << "Client connected! You can now send messages." << '\n';                                       // Once connection is established, let the user know that they can send a message
      cout << "\n" << "Type 'EXIT' at anytime to leave the chat" << '\n';

      std::thread read_thread(readMessage, std::ref(ssl_socket), user1);                                            // Create a thread for reading messages
      std::thread send_thread(sendMessage, std::ref(ssl_socket), user1);                                            // Create a thread for sending messages

      read_thread.join();                                                                                           // Join the threads (this makes sure that the main thread waits for the threads to finish)
      send_thread.join();
    }
    catch (const boost::system::system_error& error)                                                                // creating a variable called error and referencing it in the catch block so we can log and print the error
    {
      cout << "Error: " << error.what() << '\n';
      cout << "Houston we have a problem. Message not received" << '\n';                                            // updating the user on error handling
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using Whisper Chat. Goodbye" << '\n';
      return 1;                                                                                                     // returning 1 so we know the programme did not complete successfully
    }
  return 0;                                                                                                         // if succesful return 0
}

//function to join the connection

int joinConnection(int portNumber, string ipAddress, string user2)                                                  // function to join the connection passing in the portnumber, ip address and string name
{
  boost::asio::io_context io_context;                                                                               // Create an io_context object for Boost.Asio for asynchronous operations
  boost::asio::ssl::context SSLContext(boost::asio::ssl::context::tlsv12);                                          // Create SSL context and use TLS version 1.2
  SSLContext.load_verify_file(certFile);                                                                            // load the server certificate in order to verify identity
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ipAddress), portNumber);                    // uses the string ip address and converts it an ip address to use as the endpoint combined with the port number                                 // Load server certificate
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket(io_context, SSLContext);                        // Create SSL socket wrapping the TCP socket
  cout << "Connecting on port " << portNumber;                                                                      // updating the user on what's happening

  cout << "\n" << "Connecting to Whisperer 1. Please wait to be connected..." << '\n';
  try {                                                                                                             // try block to make sure that we catch any errors when trying to join
    ssl_socket.lowest_layer().connect(endpoint);                                                                    // Connect the SSL socket to the server
    ssl_socket.handshake(boost::asio::ssl::stream_base::client);                                                    // Perform SSL handshake (as the client)
    cout << '\n' << "SSL Handshake complete.  Your messages will be encrypted." << '\n';                            // Handshake completed successfully
    cout << '\n' << "Connected to server! You can now send messages" << '\n';
    cout << "\n" << "Type 'EXIT' at anytime to leave the chat" << '\n';
    std::thread read_thread(readMessage, std::ref(ssl_socket), user2);                                              // Start a thread for receiving messages
    std::thread send_thread(sendMessage, std::ref(ssl_socket), user2);                                              // Start a thread for sending messages

    read_thread.join();                                                                                             // Join the threads
    send_thread.join();
  }
  catch (const boost::system::system_error& error) {                                                                // catch any exceptions / errors in this block
      cout << "Error: " << error.what() << '\n';
      cout << "Houston we have a problem. Message not received" << '\n';
      cout << "The chat has ended." << '\n';
      cout << "Thank you for using Whisper Chat. Goodbye" << '\n';
      return 1;                                                                                                     // Return 1 to indicate failure
  }
  return 0;                                                                                                         // Return 0 for success
}

// ASCII artwork saved as a variable

string wordArt = R"(
  .------------------------------------------------------------------------------.
  |    __          ___     _                           _____ _           _       |
  |    \ \        / / |   (_)                         / ____| |         | |      |
  |     \ \  /\  / /| |__  _ ___ _ __   ___ _ __     | |    | |__   __ _| |_     |
  |      \ \/  \/ / | '_ \| / __| '_ \ / _ \ '__|    | |    | '_ \ / _` | __|    |
  |       \  /\  /  | | | | \__ \ |_) |  __/ |       | |____| | | | (_| | |_     |
  |        \/  \/   |_| |_|_|___/ .__/ \___|_|        \_____|_| |_|\__,_|\__|    |
  |                             | |                                              |
  |                             |_|                                              |
  '------------------------------------------------------------------------------'
  )";

// run programme function that returns an int value

int runProgramme(){
  string user1, user2, userChoice;                                                                          // setting up variables
  int portNumber;
  string port;
  const string ipAddress = "192.168.1.219";                                                                 // string ip address that will be used for use in the setup or join conenction function

  cout << "\n" << "\033[36m" << wordArt << "\033[0m";                                                       // load the artwork

  cout << "\n" << "\033[34m" << "* Made for parents to share parenting advice & tips while the little ones are asleep *" << "\033[0m" << '\n';  // message to decribed what the programme is about

  cout << "\n" << "Would you like to:" << "\n";                                                             // user menu
  cout << "1: Start the chat as Whisperer 1 (Server) " << '\n';
  cout << "2: Start the chat as Whisperer 2 (Client) " << '\n';
  cout << "\n" << "Please enter your choice of 1 or 2: " << '\n';
  std::getline(cin, userChoice);                                                                              // save user input as the variable userChoice

  if (userChoice == "1") {                                                                                    // if userChoice is 1
    cout << '\n' << "What is your name?: " << '\n';
    std::getline(cin, user1);                                                                                 // Saving the name to use later
    cout << "Hello " << user1 << ". Welcome to " << "\033[36m" <<  "Whisper Chat!" << "\033[0m" <<'\n';       // welcome message with coloured font
    cout << '\n' << "Type 'EXIT' at anytime to leave the chat" << '\n';                                       // letting the use know they can exit using "EXIT"
    setupConnection(portNumber, ipAddress, user1);                                                            // Run the setup connection function (passing in portnumber placeholder, ip address and the name)
  } else if (userChoice == "2") {                                                                             // if userChoice is 2
      cout << '\n' << "Enter the port number selected by whisperer 1 (Server): " << '\n';                     // ask for the portnumber generated in the setup connection
      std::getline(cin, port);                                                                                // take the user input and use it as the variable value
      portNumber = std::stoi(port);                                                                           // converting from string to an int using the standard library and making it an int variable portnumber
      cout << '\n' << "What is your name?: " << '\n';
      std::getline(cin, user2);                                                                               // storing the name for later use
      cout << "Hello " << user2 << ". " << "Welcome to " << "\033[36m" <<  "Whisper Chat!" << "\033[0m" <<'\n'; // welcome message
      joinConnection(portNumber, ipAddress, user2);                                                           // run the join the connection function passing in the portnumber, ip address and name
  } else {                                                                                                    // handling any user input errors
      cout << "You typed something incorrectly.  Please only select 1 or 2" << '\n';
      return runProgramme();                                                                                  // Run the function again if wrong input
  }
  return 0;
}
