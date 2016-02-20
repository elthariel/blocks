
#include "systems/network.hh"
#include "events/network.hh"
#include "components/basic.hh"
#include "Protocole.hh"

namespace blocks
{
  namespace systems
  {
    Network::Network(blocks::MeshingThread &mt, std::string host, std::string port, Game *game)
      : _meshing_thread(mt), _client(*this, host, port), _last_pos(0, 0, 0), _game(game)
    {
    }

    void Network::configure(ex::EventManager &em)
    {
      em.subscribe<events::load_chunk>(*this);
    }

    void Network::update(ex::EntityManager &entities,
                         ex::EventManager &events,
                         ex::TimeDelta dt)
    {
      _passed += dt;

      auto lambda = [=](ex::Entity entity,
                        components::Player &player,
                        components::Position &position)
      {
        common::wpos pos(position.get_x(), position.get_y(), position.get_z());
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
                    components::Position>(lambda);

      if (_socket != nullptr && !_connect_event_sent)
      {
        _connect_event_sent = true;
        events.emit<events::server_connected>();
      }
    }

    void Network::receive(const events::load_chunk &e)
    {
      _socket->write(Protocole::create_message(fbs::Action::Action_ASK_CHUNK,
                                               fbs::AType::AType_PosObj, &e.id));
    }


    void Network::dispatch(TcpClient::connection::pointer socket, uint8_t *buffer)
    {
      auto message = flatbuffers::GetMutableRoot<blocks::fbs::Message>(buffer);
      switch(message->action())
      {
        case fbs::Action::Action_INITIAL_POS       : on_initial_pos(socket, message); break;
        case fbs::Action::Action_MOVE              : on_move(message); break;
        case fbs::Action::Action_ASK_CHUNK         : break;
        case fbs::Action::Action_CHUNK             : on_chunk(message); break;
        case fbs::Action::Action_NEW_BLOCK         : break;
        case fbs::Action::Action_DELETE_BLOCK      : break;
        case fbs::Action::Action_PLAYER_CONNECT    : on_player_connect(message); break;
        case fbs::Action::Action_PLAYER_DISCONNECT : break;
      }
    }

    void Network::on_initial_pos(TcpClient::connection::pointer socket,
                                 blocks::fbs::Message *message)
    {
      if (_socket == nullptr)
        _socket = socket;

      auto player = static_cast<const fbs::Player*>(message->body());
      // initial pos event
      auto _pos = player->pos();
      common::wpos pos(_pos->x(), _pos->y(), _pos->z());
      _game->create_player(pos);
    }

    void Network::on_chunk(blocks::fbs::Message *message)
    {
      auto chunk = static_cast<const blocks::fbs::Chunk*>(message->body());
      _meshing_thread.input_pipe << blocks::common::Chunk::deserialize(chunk);
    }

    void Network::on_move(blocks::fbs::Message *message)
    {
      auto player = static_cast<const blocks::fbs::Player*>(message->body());
      auto entity = _characters.at(player->id());
      auto ppos = player->pos();
      auto pos = entity.component<components::Position>();
      pos.get()->set(ppos->x(), ppos->y(), ppos->z());
      std::cout << "Move" << ppos->x() << ":" <<  ppos->y() << ":" <<  ppos->z() << std::endl;
    }

    void Network::on_player_connect(blocks::fbs::Message *message)
    {
      auto player = static_cast<const blocks::fbs::Player*>(message->body());

      auto _pos = player->pos();
      common::wpos pos(_pos->x(), _pos->y(), _pos->z());
      _characters.insert(std::pair<int, ex::Entity>(player->id(), _game->create_character(pos)));
    }
  }
}
