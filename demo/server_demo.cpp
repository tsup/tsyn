#include <Network.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <tsyn/Types.hpp>


int main()
{
  tsyn::ReceiveQueue receiveQueue;
  tsyn::SendQueue sendQueue;
  tsyn::Network net( receiveQueue, sendQueue );
  net.listenTcp( 8081 );
  net.listenTcp( 8082 );
  net.listenTcp( 8083 );

  net.listenUdp( 9001 );
  net.listenUdp( 9002 );
  net.listenUdp( 9003 );
  net.run();
  while ( true )
  {
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    std::unique_ptr< tsyn::QueueData > message;
    if ( receiveQueue.pop( message ) )
    {
      std::cout << *message;
    }
    sendQueue.push( std::unique_ptr< tsyn::QueueData >( new tsyn::QueueData{ 0, "appletree", "some payload" } ) );
  }
  net.stop();
  return 0;
}

