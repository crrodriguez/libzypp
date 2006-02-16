/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/pool/PoolTraits.h
 *
*/
#ifndef ZYPP_POOL_POOLTRAITS_H
#define ZYPP_POOL_POOLTRAITS_H

#include <set>
#include <map>

#include "zypp/PoolItem.h"
#include "zypp/Capability.h"
#include "zypp/CapAndItem.h"
#include "zypp/Dep.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  namespace pool
  { /////////////////////////////////////////////////////////////////

    class PoolImpl;

    /**  */
    struct PoolImplInserter
    {
      void operator()( ResObject::constPtr ptr_r );

      PoolImplInserter( PoolImpl & poolImpl_r, bool installed_r )
      : _poolImpl( poolImpl_r )
      , _installed( installed_r )
      {}
      PoolImpl & _poolImpl;
      bool       _installed;
    };

    /**  */
    struct PoolImplDeleter
    {
      void operator()( ResObject::constPtr ptr_r );

      PoolImplDeleter( PoolImpl & poolImpl_r )
      : _poolImpl( poolImpl_r )
      {}
      PoolImpl & _poolImpl;
    };

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : PoolTraits
    //
    /** */
    struct PoolTraits
    {
    public:
      /** */
      typedef PoolItem				Item;

      /** pure items  */
      typedef std::set<Item>				ItemContainerT;
      typedef ItemContainerT::iterator			iterator;
      typedef ItemContainerT::const_iterator		const_iterator;
      typedef ItemContainerT::size_type			size_type;

      /** hashed by name */
							// use same iterators as above
	// internal organization
      typedef std::map<std::string,ItemContainerT>	NameItemContainerT;

      /** hashed by capability index */
      typedef std::list<CapAndItem>			CapItemContainerT;	// (why,who) pairs
      typedef CapItemContainerT::iterator		capitemiterator;
      typedef CapItemContainerT::const_iterator		const_capitemiterator;
      typedef CapItemContainerT::size_type		capitemsize_type;

	// internal organization
      typedef std::map<std::string,CapItemContainerT>	CapItemStoreT;		// capability.index -> (why,who) pairs
      typedef std::map<Dep,CapItemStoreT>		DepCapItemContainerT;	// Dep -> (capability.index -> (why,who) pairs)

      typedef PoolImpl                   Impl;
      typedef shared_ptr<PoolImpl>       Impl_Ptr;
      typedef shared_ptr<const PoolImpl> Impl_constPtr;
      typedef PoolImplInserter           Inserter;
      typedef PoolImplDeleter            Deleter;
    };
    ///////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
  } // namespace pool
  ///////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_POOL_POOLTRAITS_H
