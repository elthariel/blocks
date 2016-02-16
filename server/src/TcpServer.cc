#include "TcpServer.hh"

namespace blocks {

    TcpServer::TcpServer(boost::asio::io_service& io_service, Server *server)
      : _acceptor(io_service, tcp::endpoint(tcp::v4(), _port)), _server(server)
    {
      start_accept();
    }

    void TcpServer::start_accept()
    {
      TcpConnection<Server, Player>::pointer new_connection = TcpConnection<Server, Player>::create(_acceptor.get_io_service(), _server);

      _acceptor.async_accept(new_connection->socket(),
                             boost::bind(&TcpServer::handle_accept,
                                         this,
                                         new_connection,
                                         boost::asio::placeholders::error));
    }

    void TcpServer::handle_accept(TcpConnection<Server, Player>::pointer new_connection, const boost::system::error_code& error)
    {
      if (!error)
      {
        _server->on_connect_player(new_connection);
        start_accept();
      }
    }
}
