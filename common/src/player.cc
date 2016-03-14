#include "common/player.hh"

namespace blocks
{
  namespace common
  {
    Player::Player(int id, std::string login, common::wpos pos)
      : _id(id), _login(login), _pos(pos)
    {
    }
\
    flatbuffers::Offset<fbs::Player> Player::serialize(flatbuffers::FlatBufferBuilder &builder)
    {
      auto login = builder.CreateString(_login);
      auto pos = fbs::Pos(_pos.x(), _pos.y(), _pos.z());
      return fbs::CreatePlayer(builder, id(), login, &pos);
    }
  }
}
