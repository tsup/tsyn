#pragma once

#include <memory>

#include <tsyn/Types.hpp>

namespace tsyn
{
  class Sender;
  typedef std::unique_ptr< Sender > SenderRef;

  template<typename T, size_t Size>
  class RingBuffer;

  constexpr size_t RECEIVE_QUEUE_SIZE = 1024;
  typedef RingBuffer< QueueData, RECEIVE_QUEUE_SIZE > ReceiveQueue;
}

