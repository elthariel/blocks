#include "systems/network.hh"

namespace blocks
{
  namespace systems
  {
    Network::Network(std::string host, std::string port, Game *game)
      : _client(*this, host, port), _last_pos(0, 0, 0), _game(game)
    {
      _events_ptrs.insert(event_item(fbs::Action::Action_MOVE,
                                     new events::player_moved(nullptr)));
      _events_ptrs.insert(event_item(fbs::Action::Action_INITIAL_POS,
                                     new events::player_initial_pos(nullptr)));
      _events_ptrs.insert(event_item(fbs::Action::Action_PLAYER_CONNECT,
                                     new events::player_connected(nullptr)));
      _events_ptrs.insert(event_item(fbs::Action::Action_CHUNK,
                                     new events::chunk_received(nullptr)));
      _events_ptrs.insert(event_item(fbs::Action::Action_UPDATE_BLOCK,
                                     new events::block_update(nullptr)));
    }

    void Network::configure(ex::EventManager &em)
    {
      em.subscribe<events::chunk_requested>(*this);
      em.subscribe<events::player_moved>(*this);
      em.subscribe<events::player_connected>(*this);
      em.subscribe<events::player_initial_pos>(*this);
      em.subscribe<events::key>(*this);
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
            _socket->write(Protocole::create_message(fbs::Action::Action_MOVE,
                                                     fbs::AType::AType_PosObj,
                                                     &pos));
        }
      };

      entities.each<components::Player,
                    components::Node>(lambda);

      dispatch_events(entities, events);
    }

    void Network::dispatch(TcpClient::connection::pointer socket, uint8_t *buffer)
    {
      auto message = flatbuffers::GetMutableRoot<blocks::fbs::Message>(buffer);
      if (message != nullptr)
        _network_to_game_pipe << message;

      if (_socket == nullptr)
        _socket = socket;
    }

    void Network::dispatch_events(ex::EntityManager &entities, ex::EventManager &em)
    {
      fbs::Message *msg = nullptr;

      while (_network_to_game_pipe.size())
      {
        _network_to_game_pipe >> msg;
        if (msg != nullptr)
        {
          if (msg->action() == fbs::Action::Action_INITIAL_POS)
            em.emit<events::server_connected>();

          _events_ptrs[msg->action()]->emit(em, msg);
        }
      }
    }

    void Network::receive(const events::chunk_requested &e)
    {
      _socket->write(Protocole::create_message(fbs::Action::Action_ASK_CHUNK,
                                               fbs::AType::AType_PosObj, &e.id));
    }

    void Network::receive(const events::player_initial_pos &e)
    {
      auto player = e.msg;
      auto _pos = player->pos();
      common::wpos pos(_pos->x(), _pos->y(), _pos->z());
      // XXX:  _game->create_player(pos);
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
      common::wpos pos(_pos->x(), _pos->y(), _pos->z());
      _characters.insert(std::pair<int, ex::Entity>(player->id(), _game->create_character(pos)));
    }

    void Network::receive(const events::key &e)
    {
      // if (e.code == events::key::kcode::USE && e.type == events::key::ktype::DOWN)
      // {
      //   auto entity = _game->player();
      //   auto pos = entity.component<components::Position>().get()->to_wpos();

      //   Block block(1);
      //   BlockPos bpos(block, pos);

      //   _socket->write(Protocole::create_message(fbs::Action::Action_PLACE_BLOCK,
      //                                            fbs::AType::AType_BlockPos, &bpos));
      // }
      // else if (e.code == events::key::kcode::MINE && e.type == events::key::ktype::DOWN)
      // {
      //   auto entity = _game->player();
      //   auto pos = entity.component<components::Position>().get()->to_wpos();

      //   _socket->write(Protocole::create_message(fbs::Action::Action_BREAK_BLOCK,
      //                                            fbs::AType::AType_PosObj, &pos));
      // }
    }
  }
}
