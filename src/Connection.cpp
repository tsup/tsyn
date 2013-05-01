#include "Connection.hpp"

#include <sstream>
#include <limits>
#include <tsyn/Clock.hpp>

#include "Message.hpp"
#include "LowLevelConnection.hpp"
#include "RingBuffer.hpp"

tsyn::Connection::Connection( LowLevelConnectionRef lowLevelConn, ReceiveQueue & receiveQueue )
  : m_lowLevelConn( std::move(lowLevelConn) )
  , m_receiveQueue(receiveQueue)
{
  m_lowLevelConn->start( *this );
}

void tsyn::Connection::send( const tsyn::Data & payload, tsyn::Clock::Time timestamp )
{
  m_lowLevelConn->send( serialize( Message{timestamp, MessageType::USER, payload} ) );
}

void tsyn::Connection::receive( const tsyn::Data & receivedData )
{
  QueueData::Ref queueData( new QueueData );
  queueData->timestamp = 0ULL;

  for ( size_t i = 0; i < 8; ++i )
  {
    queueData->timestamp <<= 8;
    queueData->timestamp |= ( receivedData[i] & 0xff );
  }
  queueData->peerId = "TODO";
  queueData->payload = receivedData.substr( HEADER_LENGTH );

  m_receiveQueue.push( std::move(queueData) );
}

