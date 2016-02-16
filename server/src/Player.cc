#include "Player.hh"
#include "Chunk.hh"

Player::Player(TcpConnection::pointer socket)
  : _socket(socket), _id(NEXT_ID++), _pos(0, 0, 0)
{
}
