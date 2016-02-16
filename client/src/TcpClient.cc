#include "TcpClient.hh"

TcpClient::TcpClient(boost::asio::io_service& io_service,
	 boost::asio::ip::tcp::endpoint& endpoint,
     Client *client)
{
    _client = client;
    connect(io_service, endpoint);
}

void TcpClient::connect(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint)
{
    _socket = TcpConnection<Client>::create(io_service, _client);

    // Connect to server
    boost::asio::ip::tcp::socket& socket = _socket->socket();
    socket.async_connect(endpoint,
                  boost::bind(&TcpClient::handle_connect, this,
                      _socket,
                      boost::asio::placeholders::error));
}

void TcpClient::handle_connect(TcpConnection<Client>::pointer socket, const boost::system::error_code& error)
{
    if (!error)
    {
      std::cout << "Connected to server" << std::endl;
      _client->on_connected(socket);
    }
}
