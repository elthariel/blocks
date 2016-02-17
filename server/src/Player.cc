#include "Player.hh"
#include "Protocole.hh"

namespace blocks
{
    Player::Player(TcpConnection<Server, Player>::pointer socket)
      : _socket(socket), _id(NEXT_ID++), _pos(42, 0, 0)
    {
        _socket->attach_referer(this);
        _socket->write(Protocole::create_message(fbs::Action::Action_INITIAL_POS));
    }

    flatbuffers::Offset<fbs::Player> Player::serialize(flatbuffers::FlatBufferBuilder &builder)
    {
        auto pos = fbs::Pos(_pos.x(), _pos.y(), _pos.z());
        return fbs::CreatePlayer(builder, id(), &pos);
    }
}
