#pragma once

#include <memory>

#include <tsyn/Types.hpp>
#include "PeerIndexedContainer.hpp"

namespace tsyn
{
  class LowLevelConnection;
  typedef std::unique_ptr< LowLevelConnection > LowLevelConnectionRef;

  template<typename T, size_t Size>
  class RingBuffer;

  constexpr size_t RECEIVE_QUEUE_SIZE = 1024;
  typedef RingBuffer< QueueData::Ref, RECEIVE_QUEUE_SIZE > ReceiveQueue;

  typedef QueueData SendQueueData;
  constexpr size_t SEND_QUEUE_SIZE = 1024;
  typedef RingBuffer< SendQueueData::Ref, SEND_QUEUE_SIZE > SendQueue;

  class UdpSocket;
  typedef std::unique_ptr< UdpSocket > UdpSocketRef;

  class TcpAcceptor;
  typedef std::unique_ptr< TcpAcceptor > TcpAcceptorRef;

  class Connection;
  typedef PeerIndexedContainer< Connection > ConnectionTable;
}

