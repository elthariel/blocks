#pragma once

#include "chunk_generated.h"

namespace blocks
{
  namespace events
  {
    struct server_connected
    {
    };

    struct server_disconnected
    {
    };

    struct client_connected
    {
    };

    struct client_disconnected
    {
    };

    struct parent_network_event
    {
      virtual void emit(ex::EventManager &, fbs::Message *) = 0;
    };

    template <fbs::Action _action, typename T>
    struct network_event : public parent_network_event
    {
      typedef const T message_type;
      typedef network_event<_action, T> self_type;

      network_event(message_type *m): msg(m) {}
      message_type *msg;

      virtual void emit(ex::EventManager &em, fbs::Message *msg)
      {
        auto tmp = static_cast<message_type *>(msg->body());
        self_type e(tmp);
        em.emit<self_type>(e);
      }
    };

    typedef network_event<fbs::Action::Action_MOVE, fbs::Player> player_moved;
    // typedef network_event<fbs::Action::Action_INITIAL_POS, fbs::Player> player_initial_pos;
    typedef network_event<fbs::Action::Action_PLAYER_CONNECT, fbs::Player> player_connected;
    typedef network_event<fbs::Action::Action_CHUNK, fbs::Chunk> chunk_received;
    typedef network_event<fbs::Action::Action_UPDATE_BLOCK, fbs::BlockPos> block_update;
    typedef network_event<fbs::Action::Action_AUTH, fbs::BoolAnswer> auth;
  }
}
