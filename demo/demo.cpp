#include <Network.hpp>
#include <thread>
#include <chrono>

int main()
{
  tsyn::Network net;
  net.listenTcp( 8081 );
  net.listenTcp( 8082 );
  net.listenTcp( 8083 );

  net.listenUdp( 9001 );
  net.listenUdp( 9002 );
  net.listenUdp( 9003 );
  net.run();
  std::this_thread::sleep_for( std::chrono::seconds( 30 ) );
  net.stop();
  return 0;
}

