#pragma once

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "chunk_generated.h"
#include "CircularBuffer.hh"

using boost::asio::ip::tcp;

template <class T>
class TcpConnection : public boost::enable_shared_from_this<TcpConnection<T>>
{
  public:
    typedef boost::shared_ptr<TcpConnection<T>> pointer;

    static pointer create(boost::asio::io_service& io_service, T *dispatcher)
    {
      return pointer(new TcpConnection<T>(io_service, dispatcher));
    }

    tcp::socket& socket()
    {
      return _socket;
    }

    void read()
    {
        uint8_t *header = new uint8_t[8];
        boost::asio::async_read(_socket, boost::asio::buffer(header, 8), boost::asio::transfer_at_least(8),
             boost::bind(&TcpConnection<T>::handleReadHeader, this, boost::asio::placeholders::error, header));
    }

  private:
    TcpConnection(boost::asio::io_service& io_service, T *dispatcher)
      : _socket(io_service), _dispatcher(dispatcher) {}

    void handleReadHeader(const boost::system::error_code& error, uint8_t *header)
    {
        uint64_t size = *(uint64_t*)header;
        uint8_t *body = new uint8_t[size];

        boost::asio::async_read(_socket, boost::asio::buffer(body, size), boost::asio::transfer_at_least(size),
             boost::bind(&TcpConnection<T>::handleReadBody, this, boost::asio::placeholders::error, header, body));
    }

    void handleReadBody(const boost::system::error_code& error, uint8_t *header, uint8_t *body)
    {

        if (error)
            _dispatcher->dispatch(header);
        else
            read();
    }

    void handle_write(const boost::system::error_code&, size_t) {}

    T *_dispatcher;
    tcp::socket _socket;
    CircularBuffer _cb;
};
