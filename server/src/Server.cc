#include "Server.hh"

Server::Server()
{
  try
  {
    boost::asio::io_service io_service;
    _tcp_server = new TcpServer(io_service, this);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void Server::connect_player(TcpConnection::pointer socket)
{
  socket->read();
  auto player = new Player(socket);
  _players.insert(std::pair<int, Player *>(player->id(), player));
}

void Server::dispatch()
{

}
