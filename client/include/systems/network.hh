#pragma once

#include "entity.hh"
#include "pipe.hh"

#include "events/map.hh"
#include "events/network.hh"
#include "systems/network.hh"
#include "events/input.hh"
#include "components/basic.hh"
#include "Protocole.hh"
#include "common/block.hh"
#include "Bus.hh"


namespace blocks
{
  class Game;

  namespace systems
  {
    struct Network : public ex::System<Network>,
                     public ex::Receiver<Network>,
                     public nocopy
    {
      typedef std::pair<fbs::Action, events::parent_network_event *> event_item;

      Network(std::string host, std::string port, Game *);

      // Entity setup things
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      // Network in network thread
      void dispatch(uint8_t *);

      // Network events dispatch
      void dispatch_events(ex::EntityManager &entities, ex::EventManager &em);
      template <class network_event_type>
      void emit(ex::EventManager &em, fbs::Message *msg)
      {
        auto tmp = static_cast<typename network_event_type::message_type *>(msg->body());
        network_event_type e(tmp);
        em.emit<network_event_type>(e);
      }

      // Events handler
      void receive(const events::chunk_requested &e);
      // void receive(const events::player_initial_pos &e);
      void receive(const events::player_moved &e);
      void receive(const events::player_connected &e);
      void receive(const events::key &e);
      void receive(const events::auth &e);

    protected:
      // Game/Network thread communication
      Pipe<fbs::Message *> _network_to_game_pipe;
      // Pipe<fbs::Message *> _game_to_network_pipe;

      // Network
      // TcpClient::connection::pointer _socket = nullptr;
      // TcpClient _client;
      Bus<Network> _bus;
      ex::TimeDelta _passed = 0;

      // FIXME: hack
      common::wpos _last_pos;
      Game *_game;
      std::map<fbs::Action, events::parent_network_event *> _events_ptrs;
      std::map<int, ex::Entity> _characters;
      bool _connected = false;
      bool _serv_connect_sent = false;
      char _player_id[8];
    };

  }
}
#include "game.hh"
