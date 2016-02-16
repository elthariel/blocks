#include "TcpConnection.hh"

TcpConnection::pointer TcpConnection::create(boost::asio::io_service& io_service, Server *server)
{
  return pointer(new TcpConnection(io_service, server));
}

tcp::socket& TcpConnection::socket()
{
  return socket_;
}

void TcpConnection::start()
{
  // message_ = make_daytime_string();
  // std::vector<int> message_ = {92, 33, 34};
  // //
  // boost::asio::async_write(socket_,
  //                          boost::asio::buffer(message_),
  //                          boost::bind(&TcpConnection::handle_write,
  //                                      shared_from_this(),
  //                                      boost::asio::placeholders::error,
  //                                      boost::asio::placeholders::bytes_transferred));

}

void TcpConnection::read()
{
    uint8_t *header = new uint8_t[8];
    boost::asio::async_read(socket_, boost::asio::buffer(header, 8), boost::asio::transfer_at_least(8),
         boost::bind(&TcpConnection::handleReadHeader, this, boost::asio::placeholders::error, header));
}

TcpConnection::TcpConnection(boost::asio::io_service& io_service, Server *server)
  : socket_(io_service), _server(server)
{
}

void TcpConnection::handleReadHeader(const boost::system::error_code& error, uint8_t *header)
{
    std::cout << "READ HEADER: " << *(uint64_t*)header << std::endl;
}

void TcpConnection::handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
{
}
