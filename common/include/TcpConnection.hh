#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "chunk_generated.h"
#include "CircularBuffer.hh"

using boost::asio::ip::tcp;

namespace blocks
{
    template <class Dispatcher, class Referer>
    class TcpConnection : public std::enable_shared_from_this<TcpConnection<Dispatcher, Referer>>
    {
      public:
        typedef std::shared_ptr<TcpConnection<Dispatcher, Referer>> pointer;

        static pointer create(boost::asio::io_service& io_service, Dispatcher *dispatcher)
        {
          return pointer(new TcpConnection<Dispatcher, Referer>(io_service, dispatcher));
        }

        tcp::socket& socket()
        {
          return _socket;
        }

        void write(uint8_t *buffer, uint64_t size)
        {
            uint8_t *new_buffer = new uint8_t[size + 8];
            memcpy(new_buffer, (uint8_t*)&size, 8);
            memcpy(new_buffer + 8, buffer, size);
            boost::asio::async_write(_socket,
                                     boost::asio::buffer(new_buffer, size + 8),
                                     boost::bind(&TcpConnection<Dispatcher, Referer>::handle_write,
                                                 this,
                                                 boost::asio::placeholders::error));
        }

        void read()
        {
            uint8_t *header = new uint8_t[8];
            boost::asio::async_read(_socket,
                                    boost::asio::buffer(header, 8),
                                    boost::asio::transfer_at_least(8),
                                    boost::bind(&TcpConnection<Dispatcher, Referer>::handleReadHeader,
                                                this,
                                                boost::asio::placeholders::error,
                                                header));
        }

        void attach_referer(Referer *referer)
        {
            _referer = referer;
        }

        Referer *referer() const
        {
            return _referer;
        }

      private:
        TcpConnection(boost::asio::io_service& io_service, Dispatcher *dispatcher)
          : _socket(io_service), _dispatcher(dispatcher) {}

        void handleReadHeader(const boost::system::error_code& error, uint8_t *header)
        {
            if (error)
            {
                std::cout << "Error read" << std::endl;
                return ;
            }

            uint64_t size = *(uint64_t*)header;
            uint8_t *body = new uint8_t[size];

            std::cout << "Header content " << size <<  std::endl;

            boost::asio::async_read(_socket,
                                    boost::asio::buffer(body, size),
                                    boost::asio::transfer_at_least(size),
                                    boost::bind(&TcpConnection<Dispatcher, Referer>::handleReadBody,
                                                this,
                                                boost::asio::placeholders::error,
                                                header,
                                                body));
        }

        void handleReadBody(const boost::system::error_code& error, uint8_t *header, uint8_t *body)
        {
            if (error)
                std::cout << "Error read" << std::endl;
            else
                _dispatcher->dispatch(this->shared_from_this(), body);
                read();
        }

        void handle_write(const boost::system::error_code& e)
        {
            if (e)
                std::cout << "Error write" << std::endl;
        }

        Dispatcher *_dispatcher;
        Referer *_referer;
        tcp::socket _socket;
        CircularBuffer _cb;
    };
}
