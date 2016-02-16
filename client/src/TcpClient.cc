#include "TcpClient.hh"

namespace blocks {

    TcpClient::TcpClient(char *ip, char *port, Game *game)
    {
        _game = game;
        try
        {
          boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), atoi(port));

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
        _socket = TcpConnection<Game, Game>::create(_io_service, _game);

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
