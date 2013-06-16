#include <Network.hpp>
#include <thread>
#include <chrono>
#include <iostream>


std::ostream&
operator<<( std::ostream& output, tsyn::QueueData& message )
{
  output << "timestamp: " << message.timestamp << std::endl;
  output << "peer id: " << message.peerId << std::endl;
  output << "payload: " << message.payload<< std::endl;
  return output;
}



int main()
{
  tsyn::ReceiveQueue receiveQueue;
  tsyn::Network net( receiveQueue );
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
  }
  net.stop();
  return 0;
}

