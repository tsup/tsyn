#include <igloo/igloo_alt.h>
using namespace igloo;

#include <tsyn/Peer.hpp>
#include <tsyn/Types.hpp>
#include <tsyn/Clock.hpp>
#include <Types.hpp>
#include <EngineImpl.hpp>
#include <RingBuffer.hpp>

Describe( engine )
{
  void SetUp()
  {
    isCalled = false;
  }

  It ( should_call_a_starter_functor_when_start_is_called )
  {
    tsyn::EngineImpl engine( clk );
    engine.start(
        [ this ] ( tsyn::SendQueue&, tsyn::ReceiveQueue&  )
        {
          isCalled = true;
        } );
    AssertThat( isCalled, Equals( true ) );
  }


  It ( should_push_send_queue_data_to_the_sendqueue_when_send_is_called )
  {
    tsyn::EngineImpl engine( clk );
    AssertThat( engine.send( peerId1, tsyn::Data( expectedPayload ) ), Equals( true ) );

    engine.start(
        [ this ] ( tsyn::SendQueue& sendQueue, tsyn::ReceiveQueue&  )
        {
          tsyn::SendQueueData::Ref message;
          AssertThat( sendQueue.pop( message ), Equals( true ) );
          AssertThat( message->payload, Equals( expectedPayload ) );
          AssertThat( message->peerId, Equals( peerId1 ) );
          AssertThat( message->timestamp, Is().LessThan( clk.now() ) );
          isCalled = true;
        } );
    AssertThat( isCalled, Equals( true ) );
  }


  It ( should_receive_messages_from_the_receive_queue )
  {
    tsyn::EngineImpl engine( clk );
    const tsyn::Clock::Time expectedTimestamp( clk.now() );
    tsyn::QueueData* dataPointer(
        new tsyn::QueueData{ expectedTimestamp, peerId1, expectedPayload } );

    engine.start(
        [ this, &dataPointer ] ( tsyn::SendQueue&, tsyn::ReceiveQueue& receiveQueue )
        {
          AssertThat( receiveQueue.push( tsyn::QueueData::Ref( dataPointer ) ), Equals( true ) );
        } );

    tsyn::QueueData::Ref message;
    AssertThat( engine.receive( message ), Equals( true ) );
    AssertThat( message.get(), Equals( dataPointer ) );
  }

  bool isCalled;
  const tsyn::Data expectedPayload{ "appletree" };
  const tsyn::Peer::Id peerId1{ "peer1" };
  const tsyn::Clock clk;
};

