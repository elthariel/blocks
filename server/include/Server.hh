#pragma once

#include <map>

class TcpServer;
class Player;

#include "TcpConnection.hh"
class Server
{
  public:
    Server();

    void on_connect_player(TcpConnection<Server>::pointer);
    void dispatch(uint8_t *body);

  private:
    TcpServer *_tcp_server;
    std::map<int, Player *> _players;
    // Map _map;
};

#include "TcpServer.hh"
#include "Player.hh"
