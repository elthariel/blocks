#pragma once

#include <map>

class TcpServer;
class Player;
#include "TcpConnection.hh"

class Server
{
  public:
    Server();

    void connect_player(TcpConnection::pointer);
    void dispatch();

  private:
    TcpServer *_tcp_server;
    std::map<int, Player *> _players;
    // Map _map;
};
#include "TcpServer.hh"
#include "Player.hh"
