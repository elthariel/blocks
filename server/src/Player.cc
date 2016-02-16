#include "Player.hh"

namespace blocks {

    Player::Player(TcpConnection<Server, Player>::pointer socket)
      : _socket(socket), _id(NEXT_ID++), _pos(42, 0, 0)
    {
        _socket->attach_referer(this);
        // Player::create_message(fbs::Type::Type_PLAYER, fbs::Action::Action_MOVE, serialize());
        auto message = serialize();
        std::cout << "Player constructor " << std::get<1>(message) << "\n";
        _socket->write(std::get<0>(message), std::get<1>(message));
    }

    std::tuple<uint8_t *, size_t> Player::serialize()
    {
        flatbuffers::FlatBufferBuilder builder;

        auto pos = fbs::Pos(_pos.x(), _pos.y(), _pos.z());
        auto player = fbs::CreatePlayer(builder, id(), &pos);

        auto message = fbs::CreateMessage(builder, fbs::Type::Type_PLAYER, fbs::Action::Action_MOVE, fbs::AType::AType_Player, player.Union());

        builder.Finish(message);
        return std::make_tuple(builder.GetBufferPointer(), builder.GetSize());
        // return player.Union();
        // return flatbuffers::GetMutableRoot<fbs::Player>(builder.GetBufferPointer());
    }
}
