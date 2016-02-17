#pragma once

#include "entity.hh"
#include "meshing_thread.hh"
#include "TcpClient.hh"

#include "events/map.hh"

namespace blocks
{
  class TcpClient;

  namespace systems
  {
    struct Network : public ex::System<Network>,
                     public ex::Receiver<Network>,
                     public nocopy
    {
      Network(MeshingThread &mt, std::string host, std::string port);

      // Entity things
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);
      void receive(const events::load_chunk &e);

      // Network
      void dispatch(TcpClient::connection::pointer, uint8_t *);
      void on_initial_pos(TcpClient::connection::pointer, fbs::Message*);
      void on_move(fbs::Message*);
      void on_chunk(fbs::Message*);

    protected:
      MeshingThread &_meshing_thread;

      // Entity things
      bool _connect_event_sent = false;

      // Network
      TcpClient::connection::pointer _socket = nullptr;
      TcpClient _client;
    };

  }
}
