#pragma once

#include <memory>
#include <map>
#include <tsyn/Peer.hpp>
#include <cassert>

namespace tsyn
{

  template< typename ValueType >
  class PeerIndexedContainer
  {
    public:
      typedef std::unique_ptr< ValueType > value_type;

      PeerIndexedContainer() = default;
      PeerIndexedContainer( const PeerIndexedContainer& ) = delete;
      PeerIndexedContainer& operator=( const PeerIndexedContainer& ) = delete;

      void insert( const Peer::Id& id, value_type value )
      {
        assert( !hasKey( id ) );
        m_container.insert( std::make_pair( id, std::move( value ) ) );
      };


      bool hasKey( const Peer::Id& id ) const
      {
        return m_container.find( id )!=m_container.end();
      };


      template < typename Func >
      void on( const Peer::Id& id, Func func ) const
      {
        typename Container::const_iterator element( m_container.find( id ) );
        if ( element == m_container.end() )
        {
          return;
        }

        func( *( element->second ) );
      }


      template < typename Func >
      void each( Func func ) const
      {
        for ( auto const& pair : m_container )
        {
          func( *pair.second );
        }
      }

    private:
      typedef std::map< Peer::Id, value_type > Container;
      Container m_container;
  };
}

