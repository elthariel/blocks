#include "TcpClient.hh"
#include "systems/network.hh"

#include <string>

namespace blocks {

  TcpClient::TcpClient(systems::Network &net, std::string ip, std::string port)
    : _network(net)
  {
    try
    {
      boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip),
                                              std::stoi(port));

      connect(endpoint);
      _t = new boost::thread(boost::bind(&boost::asio::io_service::run, &_io_service));
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  }

  TcpClient::~TcpClient()
  {
    _t->join();
  }

  void TcpClient::connect(boost::asio::ip::tcp::endpoint& endpoint)
  {
    _socket = connection::create(_io_service, &_network);

    // Connect to server
    boost::asio::ip::tcp::socket& socket = _socket->socket();
    socket.async_connect(endpoint,
                         boost::bind(&TcpClient::handle_connect,
                                     this,
                                     boost::asio::placeholders::error));
  }

  void TcpClient::handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      std::cout << "Connected to server" << std::endl;
      _socket->read();
    }
    else
    {
      std::cout << "Error connect" << std::endl;
    }
  }
}
