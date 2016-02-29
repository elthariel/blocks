#include "Server.hh"

namespace blocks {

    Server::Server()
    {
      try
      {
        init_handler_ptrs();
        boost::asio::io_service io_service;
        _tcp_server = new TcpServer(io_service, this);
        io_service.run();
      }
      catch (std::exception& e)
      {
        std::cerr << e.what() << std::endl;
      }
    }

    void Server::init_handler_ptrs()
    {
      _handler_ptrs.insert(std::pair<fbs::Action, handler_ptr>(fbs::Action::Action_MOVE,
                                                               &Server::on_move));

      _handler_ptrs.insert(std::pair<fbs::Action, handler_ptr>(fbs::Action::Action_ASK_CHUNK,
                                                               &Server::on_ask_chunk));

      _handler_ptrs.insert(std::pair<fbs::Action, handler_ptr>(fbs::Action::Action_BREAK_BLOCK,
                                                               &Server::on_break_block));

      _handler_ptrs.insert(std::pair<fbs::Action, handler_ptr>(fbs::Action::Action_PLACE_BLOCK,
                                                               &Server::on_place_block));
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
        common::cid _cid(pos->x(), pos->y(), pos->z());
        
        auto chunk = _map.get(_cid);
        socket->write(Protocole::create_message(fbs::Action::Action_CHUNK, fbs::AType::AType_Chunk, chunk));
    }

    void Server::on_move(TcpConnection<Server, Player>::pointer socket, fbs::Message *message)
    {
        auto player = socket->referer();
        auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();

        common::wpos _pos(pos->x(), pos->y(), pos->z());
        player->pos(_pos);

        _tcp_server->send_all_except(socket, Protocole::create_message(fbs::Action::Action_MOVE,
                                                                       fbs::AType::AType_Player,
                                                                       player));
    }

    void Server::on_break_block(TcpConnection<Server, Player>::pointer socket, fbs::Message *message)
    {
        auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();
        common::wpos _wpos(pos->x(), pos->y(), pos->z());
        auto block = _map.get(_wpos.cid())->at(_wpos.cpos());
        block.id(0);
        block.air(true);
        block.transparent(true);

        BlockPos bpos(block, _wpos);
        socket->write(Protocole::create_message(fbs::Action::Action_UPDATE_BLOCK, fbs::AType::AType_BlockPos, &bpos));
    }

    void Server::on_place_block(TcpConnection<Server, Player>::pointer socket, fbs::Message *message)
    {
        auto bpos = static_cast<const fbs::BlockPos *>(message->body());
        common::wpos _wpos(bpos->pos()->x(), bpos->pos()->y(), bpos->pos()->z());
        auto block = _map.get(_wpos.cid())->at(_wpos.cpos());

        if (block.air())
        {
          auto bblock = bpos->block();
          block.id(bblock->id());
          block.air(false);
          block.transparent(bblock->transparent());
          block.variant(bblock->variant());
          block.light(bblock->light());
        }

        BlockPos new_bpos(block, _wpos);
        socket->write(Protocole::create_message(fbs::Action::Action_UPDATE_BLOCK, fbs::AType::AType_BlockPos, &new_bpos));
    }

    void Server::dispatch(TcpConnection<Server, Player>::pointer socket, uint8_t *buffer)
    {
        auto message = flatbuffers::GetMutableRoot<fbs::Message>(buffer);

        (*this.*(_handler_ptrs[message->action()]))(socket, message);
    }
}
