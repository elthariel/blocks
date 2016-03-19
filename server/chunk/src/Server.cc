#include "Server.hh"

namespace blocks {

    Server::Server()
      : _bus("localhost", 5672, "events_server", this, true)
    {
      init_handler_ptrs();
      init_handler_rpc_ptrs();
      // _bus.subscribe_events("world.chunks");
      _bus.subscribe_rpc("chunks");
    }

    void Server::init_handler_ptrs()
    {
      // _handler_ptrs.insert(std::pair<fbs::Action, handler_ptr>(fbs::Action::Action_MOVE,
      //                                                          &Server::on_move));

      // _handler_ptrs.insert(std::make_tuple(fbs::Action::Action_ASK_CHUNK, &Server::on_ask_chunk));

      // _handler_ptrs.insert(std::pair<fbs::Action, handler_ptr>(fbs::Action::Action_BREAK_BLOCK,
      //                                                          &Server::on_break_block));
      //
      // _handler_ptrs.insert(std::pair<fbs::Action, handler_ptr>(fbs::Action::Action_PLACE_BLOCK,
      //                                                          &Server::on_place_block));
    }

    void Server::init_handler_rpc_ptrs()
    {
      _handler_rpc_ptrs.insert(std::make_pair(fbs::AType::AType_PosObj, &Server::on_ask_chunk));
    }

    // void Server::on_ask_chunk(fbs::Message *message)
    // {
    //     auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();
    //     common::cid _cid(pos->x(), pos->y(), pos->z());
    //
    //     auto chunk = _map.get(_cid);
    //
    //     _bus.emit("world.players.*", fbs::Action::Action_CHUNK, chunk);
    // }

    void Server::on_move(fbs::Message *message)
    {
        // auto player = socket->referer();
        // auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();
        //
        // common::wpos _pos(pos->x(), pos->y(), pos->z());
        // player->pos(_pos);
        //
        // _bus.emit("world.players.*", Protocole::create_message("0", fbs::Action::Action_MOVE,
        //                                                      fbs::AType::AType_Player,
        //                                                      player));
    }

    void Server::on_break_block(fbs::Message *message)
    {
        auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();
        common::wpos _wpos(pos->x(), pos->y(), pos->z());
        auto block = _map.get(_wpos.cid())->at(_wpos.cpos());
        block.id(0);
        block.air(true);
        block.transparent(true);

        BlockPos bpos(block, _wpos);
        _bus.emit("world.players.*", fbs::Action::Action_UPDATE_BLOCK, &bpos);
    }

    void Server::on_place_block(fbs::Message *message)
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

        _bus.emit("world.players.*", fbs::Action::Action_UPDATE_BLOCK, &new_bpos);
    }

    void Server::dispatch(fbs::Message *message)
    {
        std::map<fbs::Action, handler_ptr>::iterator it;
        it = _handler_ptrs.find(message->action());

        if (it != _handler_ptrs.end())
          (*this.*(_handler_ptrs[message->action()]))(message);
    }

    void Server::on_ask_chunk(fbs::RPC *message, Bus<Server>::DoneCallback done)
    {
      auto pos = static_cast<const fbs::PosObj *>(message->body())->pos();
      common::cid _cid(pos->x(), pos->y(), pos->z());

      auto chunk = _map.get(_cid);

      // auto msg = Protocole::create_rpc(res);
      done(NULL, Protocole::create_rpc(chunk));

      // _bus.emit("world.players.*", fbs::Action::Action_CHUNK, chunk);
    }

    void Server::dispatch_rpc(fbs::RPC *message, Bus<Server>::DoneCallback done)
    {
      std::map<fbs::AType, handler_rpc_ptr>::iterator it;
      it = _handler_rpc_ptrs.find(message->body_type());

      if (it != _handler_rpc_ptrs.end())
        (*this.*(_handler_rpc_ptrs[message->body_type()]))(message, done);

    }
}
