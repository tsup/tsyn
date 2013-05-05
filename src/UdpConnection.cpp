#include "UdpConnection.hpp"
#include "Connection.hpp"

#include <iostream>

tsyn::UdpConnection::UdpConnection()
{
  std::cout << "UdpConnectioncreated!" << std::endl;
}


void
tsyn::UdpConnection::start( Connection& ownerConnection )
{
  m_ownerConnection = &ownerConnection;
}


void
tsyn::UdpConnection::receive( const Data& data )
{
  m_ownerConnection->receive( data );
}


void
tsyn::UdpConnection::send( Data&& )
{
}

