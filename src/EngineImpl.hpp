#pragma once

#include "Types.hpp"
#include "RingBuffer.hpp"

namespace tsyn
{
  class Clock;

  class EngineImpl
  {
    public:
      EngineImpl() = delete;
      EngineImpl( const EngineImpl& ) = delete;
      EngineImpl& operator=( const EngineImpl& ) = delete;

      EngineImpl( const Clock& clk )
        : m_sendQueue()
        , m_receiveQueue()
        , m_clock( clk )
      {
      }

      template < class Starter >
      void start( Starter starter )
      {
        starter( m_sendQueue, m_receiveQueue );
      }

      bool send( const Peer::Id& peerId, Data&& payload )
      {
        return m_sendQueue.push( SendQueueData::Ref(
              new SendQueueData{ m_clock.now(), peerId, payload } ) );
      }

      bool receive( QueueData::Ref& queueData )
      {
        return m_receiveQueue.pop( queueData );
      }

    private:
      SendQueue    m_sendQueue;
      ReceiveQueue m_receiveQueue;
      const Clock& m_clock;
  };
}

