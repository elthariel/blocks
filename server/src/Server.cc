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
        cid _cid(pos->x(), pos->y(), pos->z());
        auto chunk = _map.get(_cid);
        socket->write(Protocole::create_message(fbs::Action::Action_CHUNK, fbs::AType::AType_Chunk, chunk));
    }

    void Server::dispatch(TcpConnection<Server, Player>::pointer socket, uint8_t *buffer)
    {
        auto message = flatbuffers::GetMutableRoot<fbs::Message>(buffer);
        switch(message->action())
        {
            case fbs::Action::Action_INITIAL_POS  : break;
            case fbs::Action::Action_MOVE         : break;
            case fbs::Action::Action_ASK_CHUNK    : on_ask_chunk(socket, message); break;
            case fbs::Action::Action_CHUNK        : break;
            case fbs::Action::Action_NEW_BLOCK    : break;
            case fbs::Action::Action_DELETE_BLOCK : break;
        }
    }

}
