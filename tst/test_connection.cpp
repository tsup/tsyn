#include <igloo/igloo_alt.h>
using namespace igloo;

#include <Connection.hpp>
#include <Sender.hpp>
#include <Types.hpp>

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

  void SetUp()
  {
    senderStubRef.reset( new SenderStub );
    sender = senderStubRef.get();
  }

  It(can_be_instantiated)
  {
    tsyn::Connection conn( std::move(senderStubRef) );
  }
};
