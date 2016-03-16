#include "systems/network.hh"
#include "common/player.hh"


namespace blocks
{
  //TMP

  namespace systems
  {

    Network::Network(std::string host, std::string port, Game *game)
      : _bus(host, atoi(port.c_str()), "events", this), _last_pos(0, 0, 0), _game(game)
    {
      // ---
        // sprintf(_player_id, "%d", 1);
        // _bus.subscribe("world.players.1");
        // _game->create_player(_last_pos);
        // _connected = true;

      // _events_ptrs.insert(event_item(fbs::Action::Action_MOVE,
      //                                new events::player_moved(nullptr)));
      // _events_ptrs.insert(event_item(fbs::Action::Action_PLAYER_CONNECT,
      //                                new events::player_connected(nullptr)));
      _events_ptrs.insert(event_item(fbs::Action::Action_CHUNK,
                                     new events::chunk_received(nullptr)));
      // _events_ptrs.insert(event_item(fbs::Action::Action_UPDATE_BLOCK,
      //                                new events::block_update(nullptr)));
      // _events_ptrs.insert(event_item(fbs::Action::Action_AUTH,
      //                                new events::auth(nullptr)));
      common::PlayerAuth pauth("test", "test");
      // common::wpos pos(1, 2, 3);

      // _bus.subscribe("world.chunks");
      // _bus.emit("tamere42", Protocole::create_message(fbs::Action::Action_MOVE,
      //                                                 fbs::AType::AType_PlayerAuth))

      // _bus.rpc

      // _bus.subscribe("world.players.1");
      // _connected = true;
      // sprintf(_player_id, "%d", 1);
      _bus.rpc(Protocole::create_rpc(fbs::AType::AType_PlayerAuth, &pauth),
      [&](fbs::RPC *rpc)
      {
        auto player = common::Player::deserialize(static_cast<const fbs::Player *>(rpc->body()));
        std::cout << "AUTH ANSWER " << player->id() << " " << player->login() << std::endl;
        sprintf(_player_id, "%d", player->id());
        std::cout << "TA RACE: " << std::string("world.players.") + _player_id << std::endl;
        _bus.subscribe(std::string("world.players.") + _player_id);
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
      // em.subscribe<events::player_initial_pos>(*this);
      em.subscribe<events::key>(*this);
      em.subscribe<events::auth>(*this);
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
            // _bus.emit("world.chunks", Protocole::create_message(_player_id, fbs::Action::Action_MOVE,
            //                                     fbs::AType::AType_PosObj,
            //                                     &pos));
        }
      };

      entities.each<components::Player,
                    components::Node>(lambda);

      dispatch_events(entities, events);
    }


    void Network::dispatch(uint8_t *buffer)
    {
      std::cout << "Network Dispatch " << std::endl;
      auto message = flatbuffers::GetMutableRoot<blocks::fbs::Message>(buffer);
      if (message != nullptr)
        _network_to_game_pipe << message;

      // if (_socket == nullptr)
      //   _socket = socket;
    }

    void Network::dispatch_events(ex::EntityManager &entities, ex::EventManager &em)
    {
      fbs::Message *msg = nullptr;

      if (_connected && !_serv_connect_sent)
      {
        _serv_connect_sent = true;
        em.emit<events::server_connected>();
      }

      while (_network_to_game_pipe.size())
      {
        std::cout << "Network Dispatch Events depop" << std::endl;
        _network_to_game_pipe >> msg;
        if (msg != nullptr)
        {
          // if (msg->action() == fbs::Action::Action_INITIAL_POS)
          std::map<fbs::Action, events::parent_network_event *>::iterator it;
          it = _events_ptrs.find(msg->action());
          std::cout << "Action " << msg->action() << std::endl;
          if (it != _events_ptrs.end())
          {
            _events_ptrs[msg->action()]->emit(em, msg);
          }
        }
      }
    }

    void Network::receive(const events::chunk_requested &e)
    {
      std::cout << "Ask chunk" << std::endl;
      _bus.emit("world.chunks", Protocole::create_message(_player_id, fbs::Action::Action_ASK_CHUNK,
                                               fbs::AType::AType_PosObj, &e.id));
    }

    void Network::receive(const events::auth &e)
    {
      // auto msg = e.msg;
      // bool answer = msg->answer();
      std::cout << "Auth result " << std::endl;

    }

    // void Network::receive(const events::player_initial_pos &e)
    // {
    //   auto player = e.msg;
    //   auto _pos = player->pos();
    //   common::wpos pos(_pos->x(), _pos->y(), _pos->z());
    //   // XXX:  _game->create_player(pos);
    // }

    void Network::receive(const events::player_moved &e)
    {
      std::cout << "MOVE" << std::endl;
      return ;
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
      common::wpos pos(_pos->x(), _pos->y(), _pos->z());
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
