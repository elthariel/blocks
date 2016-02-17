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
    }

    void Server::on_ask_chunk(TcpConnection<Server, Player>::pointer socket, fbs::Message *message)
    {
        auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();
        std::cout << "Ask Chunk " << pos->x() << ":" << pos->y() << ":" << pos->z() << ":" << std::endl;
    }

    // void on_move(uint8_t *buffer)
    // {
    //
    // }

    void Server::dispatch(TcpConnection<Server, Player>::pointer socket, uint8_t *buffer)
    {
        std::cout << "Got from " << socket->referer()->id() << buffer << std::endl;

        auto message = flatbuffers::GetMutableRoot<fbs::Message>(buffer);
        switch(message->action())
        {
            case fbs::Action::Action_ASK_CHUNK  : on_ask_chunk(socket, message); break;
          //   case fbs::Action::Action_CHUNK        : on_chunk(message); break;
        }


    }

}
