#include <igloo/igloo_alt.h>
using namespace igloo;

#include <algorithm>
#include <array>
#include <thread>
#include <tsyn/RingBuffer.hpp>

namespace
{
  typedef tsyn::RingBuffer< int, 4 > BufferType;

  class ThreadRaii
  {
    public:
      ThreadRaii( std::function< void() > func )
        : m_thread( func )
      {
      }

      ~ThreadRaii()
      {
        m_thread.join();
      }

    private:
      std::thread m_thread;
  };

}


Describe( ringbuffer )
{
  Describe( basic_behavior_with_pod_type )
  {
    typedef tsyn::RingBuffer< int, 4 > BufferType;

    void asserThatPoppedElementIs( BufferType::value_type expected, BufferType& buffer )
    {
      BufferType::value_type item( 0 );
      AssertThat( buffer.pop( item ), Equals( true ) );
      AssertThat( item, Equals( expected ) );
    }


    template < class Iterator >
    void checkItemsInBuffer( Iterator begin, Iterator end, BufferType& buffer )
    {
      std::for_each( begin, end,
          [ this, &buffer ]( BufferType::value_type item )
          {
            asserThatPoppedElementIs( item, buffer );
          } );
    }


    template < class Iterator >
    void fillBuffer( Iterator begin, Iterator end, BufferType& buffer )
    {
      std::for_each( begin, end,
          [ &buffer ]( BufferType::value_type item )
          {
            AssertThat( buffer.push( item ), Equals( true ) );
          } );
    }


    It ( should_be_able_to_pop_pushed_elements )
    {
      const std::array< int, 3 > testData = { 1, 2, 4 };
      BufferType buffer;
      fillBuffer( begin( testData ), end( testData ), buffer );
      checkItemsInBuffer( begin( testData ), end( testData ), buffer );
    }


    It ( should_not_push_if_buffer_is_full )
    {
      const std::array< int, 3 > testData = { 1, 2, 4 };
      BufferType buffer;
      fillBuffer( begin( testData ), end( testData ), buffer );
      AssertThat( buffer.push( 1024 ), Equals( false ) );
      checkItemsInBuffer( begin( testData ), end( testData ), buffer );
      AssertThat( buffer.push( 1024 ), Equals( true ) );
      asserThatPoppedElementIs( 1024, buffer );
    }


    It ( should_not_pop_if_buffer_is_empty )
    {
      BufferType buffer;
      BufferType::value_type item( 123 );
      AssertThat( buffer.pop( item ), Equals( false ) );
      AssertThat( item, Equals( 123 ) );
    }

  };

  Describe( basic_behavior_with_moveable_only_type )
  {
    typedef tsyn::RingBuffer< std::unique_ptr<int>, 4 > BufferType;
    It ( can_push_to_and_pop_from_buffer )
    {
      BufferType buffer;
      static const int value( 15 );
      buffer.push( std::unique_ptr<int>( new int( value ) ) );
      std::unique_ptr<int> item( nullptr );
      buffer.pop( item );
      AssertThat( *item, Equals( value ) );
    }
  };

  Describe( multithreaded_behavior )
  {
    It( should_be_threadsafe_with_one_producer_and_one_consumer )
    {
      const int N( 20 );
      BufferType buffer;
      ThreadRaii producer(
          [ &buffer ]()
          {
            for ( int i( 0 ); i < N; ++i )
            {
              while ( !buffer.push( i ) );
            }
          } );

      for ( int i( 0 ); i < N; ++i )
      {
        int item( 0 );
        while ( !buffer.pop( item ) );
        AssertThat( item, Equals( i ) );
      }
    }
  };

};

