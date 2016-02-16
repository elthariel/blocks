#pragma once

#include "../../common/include/TcpConnection.hh"
#include "Client.hh"

class TcpClient {
  public:
    TcpClient(boost::asio::io_service& io_service,
		 boost::asio::ip::tcp::endpoint& endpoint,
         Client *client);

    void connect(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint);

    void handle_connect(TcpConnection<Client>::pointer socket, const boost::system::error_code& error);

private:
    TcpConnection<Client>::pointer _socket;
    Client *_client;

};
