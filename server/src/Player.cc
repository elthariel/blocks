#include "Player.hh"

Player::Player(TcpConnection<Server>::pointer socket)
  : _socket(socket), _id(NEXT_ID++), _pos(0, 0, 0)
{
}
