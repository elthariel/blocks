#include "Client.hh"

Client::Client(char *ip, char *port)
{
    try
    {
      boost::asio::io_service io_service;
      boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), atoi(port));

      _client_tcp = new TcpClient(io_service, endpoint, this);
      io_service.run();

    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
}

void Client::on_connected(TcpConnection<Client>::pointer socket)
{
    socket->read();
}

void Client::dispatch(uint8_t *body)
{

}
