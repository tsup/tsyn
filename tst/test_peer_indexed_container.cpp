#include <igloo/igloo_alt.h>
using namespace igloo;

#include <tsyn/Peer.hpp>
#include <PeerIndexedContainer.hpp>

Describe( peer_indexed_container )
{
  typedef tsyn::PeerIndexedContainer< int > Container;


  It ( should_be_able_to_check_if_it_contains_a_key )
  {
    Container container;
    container.insert( peerId1, Container::value_type( new int( 42 ) ) );
    AssertThat( container.hasKey( peerId1 ), Equals( true ) );
    AssertThat( container.hasKey( peerId2 ), Equals( false ) );
  }


  It ( should_be_able_to_execute_functors_on_element_with_certain_key )
  {
    Container container;
    const int expectedValue( 42 );
    container.insert( peerId1, Container::value_type( new int( expectedValue ) ) );
    container.insert( peerId2, Container::value_type( new int( expectedValue + 1 ) ) );

    size_t numberOfElements( 0 );
    container.on( peerId1,
        [ &numberOfElements ] ( int& element )
        {
          ++numberOfElements;
          AssertThat( element, Equals( expectedValue ) );
        } );
    AssertThat( numberOfElements, Equals( 1 ) );
  }


  It ( should_be_able_to_execute_functors_on_all_element )
  {
    Container container;
    const std::vector< std::pair< tsyn::Peer::Id, int > > expectedValues{ { { "1", 42 }, { "2", 43 } } };
    for ( const auto& pair : expectedValues )
    {
      container.insert( pair.first, Container::value_type( new int( pair.second ) ) );
    }

    std::vector< int > found;
    container.each(
        [ &found ] ( int& element )
        {
          found.push_back( element );
        } );

    AssertThat( found, HasLength( expectedValues.size() ) );
    for ( const auto& pair : expectedValues )
    {
      AssertThat( found, Has().Exactly(1).EqualTo( pair.second ) );
    }
  }

  tsyn::Peer::Id peerId1{ "peer1" };
  tsyn::Peer::Id peerId2{ "peer2" };
};

