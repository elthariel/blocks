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
      auto player = new Player(socket);
      _players.insert(std::pair<int, Player *>(player->id(), player));
      _tcp_server->send_all_except(socket, Protocole::create_message(fbs::Action::Action_PLAYER_CONNECT,
                                                                     fbs::AType::AType_Player,
                                                                     player));
      socket->read();
    }

    void Server::on_ask_chunk(TcpConnection<Server, Player>::pointer socket, fbs::Message *message)
    {
        auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();
        cid _cid(pos->x(), pos->y(), pos->z());
        auto chunk = _map.get(_cid);
        socket->write(Protocole::create_message(fbs::Action::Action_CHUNK, fbs::AType::AType_Chunk, chunk));
    }

    void Server::on_move(TcpConnection<Server, Player>::pointer socket, fbs::Message *message)
    {
        auto player = socket->referer();
        auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();

        wpos _pos(pos->x(), pos->y(), pos->z());
        player->pos(_pos);

        _tcp_server->send_all_except(socket, Protocole::create_message(fbs::Action::Action_MOVE,
                                                                       fbs::AType::AType_Player,
                                                                       player));
    }

    void Server::dispatch(TcpConnection<Server, Player>::pointer socket, uint8_t *buffer)
    {
        auto message = flatbuffers::GetMutableRoot<fbs::Message>(buffer);
        switch(message->action())
        {
            case fbs::Action::Action_INITIAL_POS       : break;
            case fbs::Action::Action_MOVE              : on_move(socket, message);      break;
            case fbs::Action::Action_ASK_CHUNK         : on_ask_chunk(socket, message); break;
            case fbs::Action::Action_CHUNK             : break;
            case fbs::Action::Action_NEW_BLOCK         : break;
            case fbs::Action::Action_DELETE_BLOCK      : break;
            case fbs::Action::Action_PLAYER_CONNECT    : break;
            case fbs::Action::Action_PLAYER_DISCONNECT : break;
        }
    }

}
