#include <iostream>
#include <string>
#include "whisper_functions.h"
#include <boost/asio.hpp>               // Boost Asio header files for asynchronous operations
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ssl.hpp>           // Boost SSL support header file
#include <openssl/ssl.h>                // open SSL header files necessary to handle the SSL functionality
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <atomic>



int main()
{
  runProgramme();
  return 0;
}
