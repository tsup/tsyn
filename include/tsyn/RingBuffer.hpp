#pragma once

#include <array>
#include <atomic>

namespace tsyn
{

  /*
   * Single producer, single consumer lock free ring buffer implementation. Remember to
   * call push and pop from one associated thread.
   *
   * T: type of items in the buffer
   * size: the buffer is able to hold size - 1 elements
   */

  template < typename T, size_t size >
  class RingBuffer
  {
    public:
      typedef T value_type;

      RingBuffer()
        : m_head( 0 )
        , m_tail( 0 )
        , m_buffer()
      {
      }

      /*
       * Returns false if the buffer is full, otherwise item is going to be stored.
       */
      bool push( const T& item )
      {
        const size_t currentHead( m_head.load( std::memory_order_relaxed ) );
        const size_t nextHead( next( currentHead ) );
        const bool bufferIsFull( nextHead == m_tail.load( std::memory_order_acquire ) );
        if ( bufferIsFull )
        {
          return false;
        }

        m_buffer[ currentHead ] = item;
        m_head.store( nextHead, std::memory_order_release );
        return true;
      }

      /*
       * Returns false if the buffer is empty, otherwise item is going to hold the
       * first element in the buffer.
       */
      bool pop( T& item )
      {
        const size_t currentTail( m_tail.load( std::memory_order_relaxed ) );
        const bool bufferIsEmpty( m_head.load( std::memory_order_acquire ) == currentTail );
        if ( bufferIsEmpty )
        {
          return false;
        }

        item = m_buffer[ currentTail ];
        m_tail.store( next( currentTail ), std::memory_order_release );
        return true;
      }

    private:
      size_t next( size_t index )
      {
        return ++index % size;
      }

      std::atomic<size_t> m_head;
      std::atomic<size_t> m_tail;
      std::array< T, size > m_buffer;
  };

}

