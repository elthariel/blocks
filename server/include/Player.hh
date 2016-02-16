#pragma once

#include "TcpConnection.hh"
#include "position.hh"

// struct HasId
// {
//   static int next_id() { return __id++; }
//   static int __id;
// };
//
// int HasId::__id = 0;

static int NEXT_ID = 0;

class Player
{
  public:
    Player(TcpConnection::pointer);

    int  id()       { return _id; }
    void id(int id) { _id = id; }

  private:
    TcpConnection::pointer _socket;
    int _id;
    blocks::wpos _pos;
};
