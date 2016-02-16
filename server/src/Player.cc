#include "Player.hh"

namespace blocks {

    Player::Player(TcpConnection<Server, Player>::pointer socket)
      : _socket(socket), _id(NEXT_ID++), _pos(0, 0, 0)
    {
        _socket->attach_referer(this);
    }
}
