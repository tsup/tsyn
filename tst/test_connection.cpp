#include <igloo/igloo_alt.h>
using namespace igloo;

#include <iomanip>
#include <random>

#include <Connection.hpp>
#include <Message.hpp>
#include <Sender.hpp>
#include <Types.hpp>
#include <RingBuffer.hpp>

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

  tsyn::ReceiveQueue receiveQueue;
  const tsyn::Clock::Time bigTimeStamp = 17046888542305855661ULL;
  const static uint32_t HEADER_LENGTH = 9;
  std::random_device randomDevice;

  void SetUp()
  {
    senderStubRef.reset( new SenderStub );
    sender = senderStubRef.get();
  }

  void TearDown()
  {
    tsyn::QueueData::Ref dummy;
    while( receiveQueue.pop(dummy) );
  }

  It(can_be_instantiated)
  {
    tsyn::Connection conn( std::move(senderStubRef), receiveQueue );
  }

  It(can_send_bytes_to_the_sender)
  {
    // Arrange
    tsyn::Connection conn( std::move(senderStubRef), receiveQueue );

    tsyn::Data payload = generateRandomPayloadWithLength( 300 );
    tsyn::Data expectedSentBytes = generateUserMessage( payload, bigTimeStamp );

    // Act
    conn.send( payload, bigTimeStamp );

    // Assert
    AssertThat( sender->sentDatas.back(), Equals( expectedSentBytes ) );
  }

  It(should_push_received_messages_to_queue)
  {
    tsyn::Connection connection( std::move(senderStubRef), receiveQueue );

    tsyn::Data payload = generateRandomPayloadWithLength( 10 );
    tsyn::Data incomingMessage = generateUserMessage(payload,bigTimeStamp).substr(4); // cut off length ( 4 bytes )

    connection.receive( incomingMessage );

    auto receiveQueueContent = transformReceiveQueue();

    AssertThat( receiveQueueContent, HasLength(1) );
    AssertThat( receiveQueueContent.back()->payload, Equals(payload) );
    AssertThat( receiveQueueContent.back()->timestamp, Equals(bigTimeStamp) );
  }

  void dumpHex( const tsyn::Data& data, const std::string & label = "UNKNOWN" )
  {
    std::cout << "data (HEX) " << label << " = [" << std::endl;
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

  tsyn::Data generateUserMessage( const tsyn::Data & payload, tsyn::Clock::Time ts )
  {
    uint32_t msgLength = HEADER_LENGTH + payload.length();
    tsyn::Data message = extractBytes( msgLength, 4 );
    message += extractBytes( ts, 8 );
    message += tsyn::MessageType::USER;
    message += payload;
    return message;
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

  std::vector<tsyn::QueueData::Ref> transformReceiveQueue()
  {
    tsyn::QueueData::Ref data;
    std::vector<tsyn::QueueData::Ref> ret;
    while( receiveQueue.pop(data) )
    {
      ret.push_back( std::move(data) );
    }
    return ret;
  }
};
