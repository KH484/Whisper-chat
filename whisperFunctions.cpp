
/*
  function to generate random port number:

  random port number between 1024 and 63353 avoiding:
  vector <int> portsInUse {7000,5000,53781,49576,1024,1026,5432,44950,54687,54688,54671,53790,54679,53934}
  generate a random number
  check if the number is in the vector using count
  then if not in the vector allocate that port but if so then regenerate a randome number

*/

/*
  setup and accept connection

  server:
  socket setup
  socket bind
  socket listen
  socket accept
  send / receive


  client :
  socket setup
  socket accept
  send / receive

  try:

  boost::asio::ip::tcp::socket socket(io_context);
  boost::asio::ip::tcp::endpoint endpoint(
  boost::asio::ip::address::from_string("192.168.1.219"), #randomPortNumber);
  acceptor.open(endpoint.protocol());
  acceptor.bind(endpoint);
  acceptor.listen();

  end coversation:
  void close(); close the socket

  error:
  if not error print to the system conenction on port etc

  if error:
  print did not work

  // read function / receive
  // write function / send

  read operation
      string message = read_(socket_);
      cout << message << endl;
  write operation
      send_(socket_, "Hello From Server!");
      cout << "Servent sent Hello message to Client!" << endl;
  return 0;

*/
