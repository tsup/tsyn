#include <igloo/igloo_alt.h>
using namespace igloo;

#include <iomanip>
#include <random>

#include <Connection.hpp>
#include <Sender.hpp>
#include <Types.hpp>
#include <tsyn/Clock.hpp>

struct SenderStub : public tsyn::Sender
{
  typedef std::unique_ptr<SenderStub> Ref;
  virtual void send( const tsyn::Data& data ) override
  {
    sentDatas.push_back( data );
  }
  std::vector< tsyn::Data > sentDatas;
};

Describe(AConnection)
{
  SenderStub::Ref senderStubRef;
  SenderStub * sender;
  const tsyn::Clock::Time bigTimeStamp = 17046888542305855661ULL;
  const static uint32_t HEADER_LENGTH = 9;
  std::random_device randomDevice;

  void SetUp()
  {
    senderStubRef.reset( new SenderStub );
    sender = senderStubRef.get();
  }

  It(can_be_instantiated)
  {
    tsyn::Connection conn( std::move(senderStubRef) );
  }

  It(can_send_bytes_to_the_sender)
  {
    // Arrange
    tsyn::Connection conn( std::move(senderStubRef) );

    tsyn::Data payload = generateRandomPayloadWithLength( 300 );
    uint32_t msgLength = HEADER_LENGTH + payload.size();
    tsyn::Data expectedSentBytes = extractBytes( msgLength, 4 );
    expectedSentBytes += extractBytes( bigTimeStamp, 8 );
    expectedSentBytes += tsyn::MessageType::USER;
    expectedSentBytes += payload;

    // Act
    conn.send( payload, bigTimeStamp );

    // Assert
    AssertThat( sender->sentDatas.back(), Equals( expectedSentBytes ) );
  }

  It(should_push_received_messages_to_queue)
  {
  }

  void dumpHex( const tsyn::Data& data )
  {
    std::cout << "data (HEX) = [" << std::endl;
    std::cout << std::setfill('0') << std::setw(2) << std::hex;
    for ( auto x : data )
    {
      std::cout << std::setfill('0') << std::setw(2) << std::hex <<
        (int(x) & 0xff) << ", ";
    }
    std::cout << "] " << std::endl;
  }

  std::string generateRandomPayloadWithLength( size_t len )
  {
    std::uniform_int_distribution<> distribution( 0, 0xff );

    std::string ret;
    ret.resize( len );
    std::generate( ret.begin(), ret.end(),
        std::bind(distribution, std::ref(randomDevice)) );
    return ret;
  }

  std::string extractBytes( uint64_t value, size_t numberOfBytes )
  {
    std::string extracted;
    for ( int i = numberOfBytes-1; i >= 0; --i )
    {
      extracted += ( value >> i * 8 ) & 0xff;
    }
    return extracted;
  }
};
