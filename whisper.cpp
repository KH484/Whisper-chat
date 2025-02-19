#include <iostream>
#include <string>
// #include <boost/asio.hpp>
// #include <boost/asio/ssl.hpp>

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
  cout << "Welcome to Whisper Chat";
  cout << "What is your name?: ";
  cin >> user1;
  cout << user1;

  return 0;
}
