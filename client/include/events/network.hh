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

    template <fbs::Action action, typename T>
    struct network_event
    {
      typedef const T message_type;

      network_event(message_type *_m): msg(_m) {}
      message_type *msg;
    };

    typedef network_event<fbs::Action::Action_MOVE, fbs::Player> player_initial_pos;
    typedef network_event<fbs::Action::Action_INITIAL_POS, fbs::Player> player_moved;
    typedef network_event<fbs::Action::Action_PLAYER_CONNECT, fbs::Player> player_connected;
    typedef network_event<fbs::Action::Action_CHUNK, fbs::Chunk> chunk_received;
  }
}
