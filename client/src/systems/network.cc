#include "systems/network.hh"
#include "common/player.hh"


namespace blocks
{
  //TMP

  namespace systems
  {

    Network::Network(std::string host, std::string port, Game *game)
      : _bus(host, atoi(port.c_str()), "events", this, false), _last_pos(0, 0, 0), _game(game)
    {
      // _events_ptrs.insert(event_item(fbs::Action::Action_MOVE,
      //                                new events::player_moved(nullptr)));
      // _events_ptrs.insert(event_item(fbs::Action::Action_PLAYER_CONNECT,
      //                                new events::player_connected(nullptr)));
      _rpc_ptrs.insert(std::make_pair(fbs::AType::AType_Chunk,
                                       new events::chunk_received(nullptr)));
      // _events_ptrs.insert(event_item(fbs::Action::Action_UPDATE_BLOCK,
      //                                new events::block_update(nullptr)));
      // _events_ptrs.insert(event_item(fbs::Action::Action_AUTH,
      //                                new events::auth(nullptr)));

      common::PlayerAuth pauth("test", "test");
      _bus.rpc("auth", &pauth,
        [&](fbs::RPC *rpc)
        {
          auto player = common::Player::deserialize(static_cast<const fbs::Player *>(rpc->body()));
          sprintf(_player_id, "%d", player->id());
          _bus.subscribe_events(std::string("world.players.") + _player_id);
          _game->create_player(player->pos());
          _connected = true;
        },
        [&](fbs::Error *error)
        {

        });
    }

    void Network::configure(ex::EventManager &em)
    {
      em.subscribe<events::chunk_requested>(*this);
      em.subscribe<events::player_moved>(*this);
      em.subscribe<events::player_connected>(*this);
      em.subscribe<events::key>(*this);
      // em.subscribe<events::auth>(*this);
    }

    void Network::update(ex::EntityManager &entities,
                         ex::EventManager &events,
                         ex::TimeDelta dt)
    {
      _passed += dt;

      auto lambda = [=](ex::Entity entity,
                        components::Player &player,
                        components::Node &node)
      {
        auto node_pos = node.get_pos();
        common::wpos pos(node_pos.get_x(), node_pos.get_y(), node_pos.get_z());

        if (pos != _last_pos && _passed > 0.01)
        {
            _passed = 0;
            _last_pos = pos;
            // std::cout << "MOVE" << std::endl;
            // _bus.emit("world.chunks", fbs::Action::Action_MOVE, &pos);
        }
      };

      entities.each<components::Player,
                    components::Node>(lambda);

      dispatch_events(entities, events);
    }


    void Network::dispatch(fbs::Message *message)
    {
      if (message != nullptr)
        _network_to_game_pipe << message;

    }

    void Network::dispatch_rpc(fbs::RPC *message, Bus<Network>::DoneCallback done)
    {

    }

    void Network::dispatch_events(ex::EntityManager &entities, ex::EventManager &em)
    {
      fbs::Message *msg = nullptr;
      fbs::RPC *rpc = nullptr;

      if (_connected && !_serv_connect_sent)
      {
        _serv_connect_sent = true;
        em.emit<events::server_connected>();
      }

      while (_network_to_game_pipe.size())
      {
        _network_to_game_pipe >> msg;
        if (msg != nullptr)
        {
          std::map<fbs::Action, events::parent_network_event *>::iterator it;
          it = _events_ptrs.find(msg->action());
          if (it != _events_ptrs.end())
            _events_ptrs[msg->action()]->emit(em, msg);
        }
      }

      while (_network_rpc_to_game_pipe.size())
      {
        _network_rpc_to_game_pipe >> rpc;
        if (rpc != nullptr)
        {
          std::map<fbs::AType, events::parent_network_event *>::iterator it;
          it = _rpc_ptrs.find(rpc->body_type());
          if (it != _rpc_ptrs.end())
            _rpc_ptrs[rpc->body_type()]->emit(em, rpc);
        }
      }
    }

    void Network::receive(const events::chunk_requested &e)
    {
      // std::cout << "Ask chunk" << std::endl;
      _bus.rpc("chunks", &e.id,
        [&](fbs::RPC *msg)
        {
          _network_rpc_to_game_pipe << msg;
        },
        [&](fbs::Error *error)
        {

        });
      // _bus.emit("world.chunks", fbs::Action::Action_ASK_CHUNK, &e.id);
    }

    void Network::receive(const events::player_moved &e)
    {
      auto player = e.msg;
      auto entity = _characters.at(player->id());
      auto ppos = player->pos();
      auto pos = entity.component<components::Node>();
      pos->set_pos(ppos->x(), ppos->y(), ppos->z());
      std::cout << "Move" << ppos->x() << ":" <<  ppos->y() << ":" <<  ppos->z() << std::endl;
    }

    void Network::receive(const events::player_connected &e)
    {
      auto player = e.msg;
      auto _pos = player->pos();
      auto pos = common::wpos(_pos);
      _characters.insert(std::pair<int, ex::Entity>(player->id(),
                                                    _game->create_character(pos)));
    }

    void Network::receive(const events::key &e)
    {
      if (e.code == events::key::kcode::USE && e.type == events::key::ktype::DOWN)
      {
        // auto entity = _game->player();
        // auto pos = entity.component<components::Position>().get()->to_wpos();

        // Block block(1);
        // BlockPos bpos(block, pos);

        // _bus.emit(Protocole::create_message(fbs::Action::Action_PLACE_BLOCK,
        //                                          fbs::AType::AType_BlockPos, &bpos));
      }
      else if (e.code == events::key::kcode::MINE && e.type == events::key::ktype::DOWN)
      {
        // auto entity = _game->player();
        // auto pos = entity.component<components::Position>().get()->to_wpos();

        // _bus.emit(Protocole::create_message(fbs::Action::Action_BREAK_BLOCK,
        //                                          fbs::AType::AType_PosObj, &pos));
      }
    }
  }
}
