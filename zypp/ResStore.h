/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/ResStore.h
 *
*/
#ifndef ZYPP_RESSTORE_H
#define ZYPP_RESSTORE_H

#include <iosfwd>
#include <set>

#include "zypp/base/PtrTypes.h"
#include "zypp/Package.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : ResStore
  //
  /**
   *
   *
  */
  class ResStore
  {
    friend std::ostream & operator<<( std::ostream & str, const ResStore & obj );

    typedef std::set<ResObject::Ptr> StorageT;

  public:
    /** Implementation  */
    class Impl;

    typedef StorageT::size_type      size_type;
    typedef StorageT::iterator       iterator;
    typedef StorageT::const_iterator const_iterator;

  public:
    /** Default ctor */
    ResStore();
    /** Dtor */
    ~ResStore();

  public:
    /**  */
    iterator begin()
    { return store().begin(); }
    /**  */
    iterator end()
    { return store().end(); }
    /**  */
    const_iterator begin() const
    { return store().begin(); }
    /**  */
    const_iterator end() const
    { return store().end(); }

    /**  */
    bool empty() const
    { return store().empty(); }
    /**  */
    size_type size() const
    { return store().size(); }

    // insert/erase
    /**  */
    iterator insert( const ResObject::Ptr & ptr_r )
    { return store().insert( ptr_r ).first; }
    /**  */
    template <class _InputIterator>
      void insert( _InputIterator first_r, _InputIterator last_r )
      { store().insert( first_r, last_r ); }
    /**  */
    size_type erase( const ResObject::Ptr & ptr_r )
    { return store().erase( ptr_r ); }
    /**  */
    void erase( iterator first_r, iterator last_r )
    { store().erase( first_r, last_r ); }
    /**  */
    void clear()
    { store().clear(); }

    /** Query inerface.
     * Both, \a filter_r and \a fnc_r are expected to be
     * functions or functors taking a <tt>ResObject::Ptr<\tt>
     * as argument and return a \c bool.
     *
     * forEach iterates over all ResObjects and invokes \a fnc_r,
     * iff \a filter_r returned \c true. If \a fnc_r returnes
     * \c false the loop is aborted.
     *
     * forEach returns the number of \a fnc_r invocations. Positive
     * if the loop succeeded. Negative if some call to \a fnc_r
     * returned \c false.
     +
     * \see ResFilters for a collection of predefined filters.
     */
    template <class _Function, class _Filter>
      int forEach( _Filter filter_r, _Function fnc_r ) const
      {
        int cnt = 0;
        for ( ResStore::const_iterator it = _store.begin(); it != _store.end(); ++it )
          {
            if ( filter_r( *it ) )
              {
                ++cnt;
                if ( ! fnc_r( *it ) )
                  return -cnt;
              }
          }
        return cnt;
      }

    template <class _Function>
      int forEach( _Function fnc_r ) const
      {
        int cnt = 0;
        for ( ResStore::const_iterator it = _store.begin(); it != _store.end(); ++it )
          {
            ++cnt;
            if ( ! fnc_r( *it ) )
              return -cnt;
          }
        return cnt;
      }

  private:
    /**  */
    StorageT _store;
    /**  */
    StorageT & store()
    { return _store; }
    /**  */
    const StorageT & store() const
    { return _store; }

  private:
    /** Pointer to implementation */
    RW_pointer<Impl> _pimpl; // currently unsused
  };
  ///////////////////////////////////////////////////////////////////

  /** \relates ResStore Stream output */
  std::ostream & operator<<( std::ostream & str, const ResStore & obj );

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_RESSTORE_H
