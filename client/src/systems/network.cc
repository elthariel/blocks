
#include "systems/network.hh"
#include "events/network.hh"
#include "Protocole.hh"

namespace blocks
{
  namespace systems
  {
    Network::Network(blocks::MeshingThread &mt, std::string host, std::string port)
      : _meshing_thread(mt), _client(*this, host, port)
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
      if (_socket != nullptr && !_connect_event_sent)
      {
        events.emit<events::server_connected>();
        _connect_event_sent = true;
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
        case fbs::Action::Action_INITIAL_POS  : on_initial_pos(socket, message); break;
        case fbs::Action::Action_MOVE         : break;
        case fbs::Action::Action_ASK_CHUNK    : break;
        case fbs::Action::Action_CHUNK        : on_chunk(message); break;
        case fbs::Action::Action_NEW_BLOCK     : break;
        case fbs::Action::Action_DELETE_BLOCK  : break;
      }
    }

    void Network::on_initial_pos(TcpClient::connection::pointer socket,
                                 blocks::fbs::Message *message)
    {
      if (_socket == nullptr)
        _socket = socket;

      auto player = static_cast<const fbs::Player*>(message->body());
    //   auto size = 8;
    //   for (auto i = 0; i < size; i++)
    //     for (auto j = 0; j < size; j++)
    //       for (auto k = -2; k <= 2 ; k++)
    //       {
    //         auto pos = cid(i, j, k);
    //         _socket->write(Protocole::create_message(fbs::Action::Action_ASK_CHUNK,
    //                                                  fbs::AType::AType_PosObj, &pos));
    //       }
    }

    void Network::on_chunk(blocks::fbs::Message *message)
    {
      auto fbs_chunk = static_cast<const blocks::fbs::Chunk*>(message->body());
      auto game_chunk = blocks::Chunk::deserialize(fbs_chunk);
      _meshing_thread.input_pipe << game_chunk;
    }
  }
}
