#pragma once

#include <thread>
#include <iostream>
#include <iomanip>
#include <boost/asio.hpp>

#include "LowLevelConnection.hpp"

namespace tsyn
{
  class Connection;

  class TcpConnection : public LowLevelConnection
  {
    public:
      typedef std::unique_ptr< TcpConnection > Ref;
      TcpConnection( boost::asio::io_service& io_service )
        : m_socket( io_service )
      {
      }

      boost::asio::ip::tcp::socket& socket()
      {
        return m_socket;
      }

      void start_receive()
      {
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer( &m_data, 1 ),
            std::bind( &TcpConnection::handle_read, this,
                       std::placeholders::_1 ) );
      }

      void handle_read( const boost::system::error_code& error )
      {
        if ( error )
        {
          std::cout << "connection error: " << error << std::endl;
          return;
        }

        std::cout << "received character: " << std::setfill( '0' ) << std::setw( 2 ) << std::hex << ( int( m_data ) & 0xFF ) << std::endl;
        start_receive();
      }

      virtual void start( Connection& ) override
      {
      }

      virtual void send( const Data& ) override
      {
      }

    private:
      boost::asio::ip::tcp::socket m_socket;
      char m_data;
  };


  class TcpAcceptor
  {
    public:
      typedef std::unique_ptr< TcpAcceptor > Ref;
      TcpAcceptor( boost::asio::io_service& io_service,
                   const boost::asio::ip::tcp::endpoint& endpoint )
        : m_service( io_service )
        , m_acceptor( io_service, endpoint )
        , m_nextConnection( nullptr )
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
        m_nextConnection->start_receive();
        m_connections.push_back( std::move( m_nextConnection ) );
        start_accept();
      }

    private:
      boost::asio::io_service&          m_service;
      boost::asio::ip::tcp::acceptor    m_acceptor;
      std::vector< TcpConnection::Ref > m_connections;
      TcpConnection::Ref                m_nextConnection;
  };


  class Network
  {
    public:
      void listen( int port )
      {
        boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port );
        m_acceptors.emplace_back( new TcpAcceptor( m_service, endpoint ) );
      }

      void run();
      void stop();
    private:
      std::thread                     m_networkThread;
      boost::asio::io_service         m_service;
      std::vector< TcpAcceptor::Ref > m_acceptors;
  };

}

