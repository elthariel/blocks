#pragma once

#include <iostream>

#include "common/position.hh"

namespace blocks {
  namespace common {

    class PlayerAuth
    {
      public:
        PlayerAuth(const char *login, const char *pass) : _login(login), _pass(pass)
        {
          std::cout << "PLAYER" << _login << _pass << std::endl;
        }

        flatbuffers::Offset<fbs::PlayerAuth> serialize(flatbuffers::FlatBufferBuilder &builder)
        {
          auto login = builder.CreateString(_login);
          auto pass = builder.CreateString(_pass);
          std::cout << "SERIALIZE PLAYER" << _login <<std::endl;
          return fbs::CreatePlayerAuth(builder, login, pass);
        }

      private:
        std::string _login;
        std::string _pass;
    };

    class Player
    {
      public:
        Player(int id, std::string login, common::wpos pos);

        int  id()           { return _id; }
        void id(int id)     { _id = id; }

        std::string login()          { return _login; }

        common::wpos pos()          { return _pos; }
        void pos(common::wpos &pos) { _pos = pos; }

        static Player *deserialize(const fbs::Player *player)
        {
          auto pos = common::wpos::deserialize<common::wpos>(player->pos());
          return new Player(player->id(), player->login()->str(), pos);
        }

        flatbuffers::Offset<fbs::Player> serialize(flatbuffers::FlatBufferBuilder &builder);

      private:
        int _id;
        std::string _login;
        common::wpos _pos;
    };
  }
}
