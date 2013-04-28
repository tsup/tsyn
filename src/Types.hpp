#pragma once

#include <memory>

#include <tsyn/Types.hpp>

namespace tsyn
{
  class Sender;
  typedef std::unique_ptr< Sender > SenderRef;

  constexpr size_t RECEIVE_QUEUE_SIZE = 1024;
  typedef RingBuffer< ReceiveQueueData, RECEIVE_QUEUE_SIZE > ReceiveQueue;
}

