#include <iostream>                     // part of the standard library
#include <string>                       // part of the standard library
#include "whisper_functions.h"          // referencing my header file
#include <boost/asio.hpp>               // Boost Asio header files for asynchronous operations
#include <boost/asio/ip/address.hpp>    // Boost Asio header files referencing the ip address object
#include <boost/asio/ssl.hpp>           // Boost SSL support header file
#include <openssl/ssl.h>                // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>             // open SSL header files necessary to handle the SSL functionality
#include <openssl/x509.h>               // open SSL header files necessary to handle the SSL functionality

int main()
{
  runProgramme();
  return 0;
}
