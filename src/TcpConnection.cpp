#include "TcpConnection.hpp"
#include "Message.hpp"
#include "Connection.hpp"
#include "Endpoint.hpp"

#include <boost/asio.hpp>
#include <iostream>


tsyn::TcpConnection::TcpConnection( boost::asio::io_service& ioService )
  : m_socket( ioService )
  , m_buffer()
  , m_ownerConnection( nullptr )
  , m_payloadLength( 0 )
{
}


tsyn::TcpConnection::TcpConnection(
    const boost::asio::ip::tcp::endpoint& endPoint, boost::asio::io_service& ioService )
  : m_socket( ioService )
  , m_buffer()
  , m_ownerConnection( nullptr )
  , m_payloadLength( 0 )
{
  m_socket.connect( endPoint );
}


void
tsyn::TcpConnection::start( Connection& ownerConnection )
{
  m_ownerConnection = &ownerConnection;
  startLengthRead();
}


void
tsyn::TcpConnection::startLengthRead()
{
  boost::asio::async_read(
      m_socket,
      boost::asio::buffer( m_buffer, tsyn::Message::LENGTH_SIZE ),
      std::bind( &TcpConnection::handleLengthRead, this,
      std::placeholders::_1 ) );
}


namespace
{
  uint32_t deserializePayloadLength( const char* message )
  {
    uint32_t messageLength( 0 );
    for ( size_t i = 0; i < tsyn::Message::LENGTH_SIZE; ++i )
    {
      messageLength <<= 8;
      messageLength |= ( message[i] & 0xff );
    }
    return messageLength;
  }
}


void
tsyn::TcpConnection::handleLengthRead( const boost::system::error_code& error )
{
  if ( error )
  {
    return;
  }

  m_payloadLength = deserializePayloadLength( &m_buffer[ 0 ] );
  std::cout << "payload length: " << m_payloadLength << std::endl;
  startMessageRead();
}


void
tsyn::TcpConnection::startMessageRead()
{
  boost::asio::async_read(
      m_socket,
      boost::asio::buffer( &m_buffer[ tsyn::Message::HEADER_LENGTH ], m_payloadLength ),
      std::bind( &TcpConnection::handleMessageRead, this,
      std::placeholders::_1 ) );
}


void
tsyn::TcpConnection::handleMessageRead( const boost::system::error_code& )
{
  std::cout << Data( &m_buffer[ tsyn::Message::HEADER_LENGTH ], m_payloadLength ).c_str() << std::endl;
  m_ownerConnection->receive( Data( &m_buffer[ tsyn::Message::HEADER_LENGTH ], m_payloadLength ) );
  startLengthRead();
}


void
tsyn::TcpConnection::send( Data&& message )
{
  write( m_socket, boost::asio::buffer( message ) );
}


boost::asio::ip::tcp::socket&
tsyn::TcpConnection::socket()
{
  return m_socket;
}


tsyn::TcpConnection::Ref
tsyn::TcpConnection::connectTo( const Endpoint& endPoint, boost::asio::io_service& ioService )
{
  boost::asio::ip::tcp::endpoint boostEndpoint(
      boost::asio::ip::address::from_string( endPoint.ipAddress() ),
      endPoint.port() );
  return Ref(
      new TcpConnection( boostEndpoint, ioService ) );
}

