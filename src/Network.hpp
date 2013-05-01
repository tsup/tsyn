#pragma once

#include <thread>
#include <iostream>
#include <iomanip>
#include <boost/asio.hpp>

#include "LowLevelConnection.hpp"
#include "Connection.hpp"
#include "RingBuffer.hpp"
#include "Types.hpp"

namespace tsyn
{
  class Connection;

  class TcpConnection : public LowLevelConnection
  {
    public:
      typedef std::unique_ptr< TcpConnection > Ref;
      TcpConnection( boost::asio::io_service& io_service )
        : m_socket( io_service )
        , m_ownerConnection( nullptr )
      {
      }

      boost::asio::ip::tcp::socket& socket()
      {
        return m_socket;
      }

      void receive_length()
      {
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer( m_buffer, 4 ),
            std::bind( &TcpConnection::handle_length, this,
                       std::placeholders::_1 ) );
      }

      void handle_length( const boost::system::error_code& error )
      {
        if ( error )
        {
          std::cout << "connection error: " << error << std::endl;
          return;
        }

        uint32_t messageLength( 0 );
        for ( size_t i = 0; i < 4; ++i )
        {
          messageLength <<= 8;
          messageLength |= ( m_buffer[i] & 0xff );
        }
        std::cout << "received length: " << messageLength << std::endl;
        receive_payload( messageLength );
      }

      void receive_payload( uint32_t payloadLength  )
      {
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer( m_buffer, payloadLength ),
            std::bind( &TcpConnection::handle_payload, this, payloadLength,
                       std::placeholders::_1 ) );
      }

      void handle_payload( uint32_t length, const boost::system::error_code& error )
      {
        if ( error )
        {
          std::cout << "connection error: " << error << std::endl;
          return;
        }

        std::cout << "received payload:" << Data( m_buffer, length ) << std::endl;
        m_ownerConnection->receive( Data( m_buffer, length ) );
        receive_length();
      }

      virtual void start( Connection& owner ) override
      {
        m_ownerConnection = &owner;
        receive_length();
      }

      virtual void send( const Data& ) override
      {
      }

    private:
      boost::asio::ip::tcp::socket m_socket;
      char m_buffer[ 1024 ];
      Connection* m_ownerConnection;
      uint32_t m_payloadLength;
  };


  class TcpAcceptor
  {
    public:
      typedef std::unique_ptr< TcpAcceptor > Ref;
      TcpAcceptor( boost::asio::io_service& io_service,
                   const boost::asio::ip::tcp::endpoint& endpoint,
                   ReceiveQueue& receiveQueue )
        : m_service( io_service )
        , m_acceptor( io_service, endpoint )
        , m_nextConnection( nullptr )
        , m_receiveQueue( receiveQueue )
      {
        start_accept();
      }


      void start_accept()
      {
        m_nextConnection.reset( new TcpConnection( m_service ) );
        m_acceptor.async_accept(
            m_nextConnection->socket(),
            std::bind( &TcpAcceptor::handle_accept, this,
                       std::placeholders::_1 ) );
      }


      void handle_accept( const boost::system::error_code& error )
      {
        if ( error )
        {
          m_nextConnection.reset( nullptr );
          return;
        }

        const std::string endpoint(
          m_nextConnection->socket().remote_endpoint().address().to_string() +
          ":" +
          std::to_string(m_nextConnection->socket().remote_endpoint().port() ) );
        std::cout << "Connection accepted from: " << endpoint << std::endl;
        m_connections.emplace_back(
            new Connection( std::move( m_nextConnection ),
                            m_receiveQueue ) );
        start_accept();
      }

    private:
      boost::asio::io_service&          m_service;
      boost::asio::ip::tcp::acceptor    m_acceptor;
      std::vector< Connection::Ref >    m_connections;
      TcpConnection::Ref                m_nextConnection;
      ReceiveQueue&                     m_receiveQueue;
  };


  class Network
  {
    public:
      void listen( int port )
      {
        boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port );
        m_acceptors.emplace_back( new TcpAcceptor( m_service, endpoint, m_receiveQueue ) );
      }

      void run();
      void stop();
    private:
      std::thread                     m_networkThread;
      boost::asio::io_service         m_service;
      std::vector< TcpAcceptor::Ref > m_acceptors;
      ReceiveQueue                    m_receiveQueue;
  };

}

