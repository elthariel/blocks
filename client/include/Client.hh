#pragma once

#include "../../common/include/TcpConnection.hh"

class TcpClient;

class Client
{
public:
    Client(char *ip, char *port);

    void on_connected(TcpConnection<Client>::pointer socket);

    void dispatch(uint8_t *body);

private:
    TcpClient *_client_tcp;

};
#include "TcpClient.hh"
