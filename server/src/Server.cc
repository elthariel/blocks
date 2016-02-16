#include "Server.hh"

namespace blocks {

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

    void Server::on_connect_player(TcpConnection<Server, Player>::pointer socket)
    {
      socket->read();
      auto player = new Player(socket);
      _players.insert(std::pair<int, Player *>(player->id(), player));
      socket->write((uint8_t*)"Toto tata", 10);
    }

    void Server::dispatch(TcpConnection<Server, Player>::pointer socket, uint8_t *body)
    {
        std::cout << "Got from " << socket->referer()->id() << body << std::endl;
        // socket->write((uint8_t*)"Toto tata", 10);

    }
}
