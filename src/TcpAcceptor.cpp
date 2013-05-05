#include "TcpAcceptor.hpp"
#include "TcpConnection.hpp"
#include "Connection.hpp"

tsyn::TcpAcceptor::TcpAcceptor( boost::asio::io_service& io_service,
                   const boost::asio::ip::tcp::endpoint& endpoint,
                   ReceiveQueue& receiveQueue,
                   ConnectionTable& connections )
  : m_service( io_service )
  , m_acceptor( io_service, endpoint )
  , m_nextConnection( nullptr )
  , m_receiveQueue( receiveQueue )
  , m_connections( connections )
{
  startAccept();
}


void
tsyn::TcpAcceptor::startAccept()
{
  m_nextConnection.reset( new TcpConnection( m_service ) );
  m_acceptor.async_accept(
      m_nextConnection->socket(),
      std::bind( &TcpAcceptor::handleAccept, this,
      std::placeholders::_1 ) );
}


void
tsyn::TcpAcceptor::handleAccept( const boost::system::error_code& error )
{
  if ( error )
  {
    m_nextConnection.reset( nullptr );
    return;
  }

  const std::string endpoint( std::string( "tcp://" ) +
      m_nextConnection->socket().remote_endpoint().address().to_string() + ":" +
      std::to_string(m_nextConnection->socket().remote_endpoint().port() ) );

  std::cout << "Connection accepted from: " << endpoint << std::endl;
  if ( m_connections.hasKey( endpoint ) )
  {
    m_nextConnection.reset( nullptr );
    startAccept();
    return;
  }

  m_connections.insert( endpoint,
                        Connection::Ref( new Connection(
                            std::move( m_nextConnection ),
                            m_receiveQueue ) ) );
  startAccept();
}

