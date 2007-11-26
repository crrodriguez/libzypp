/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/sat/Pool.h
 *
*/
#ifndef ZYPP_SAT_POOL_H
#define ZYPP_SAT_POOL_H

#include <iosfwd>

#include "zypp/sat/detail/PoolMember.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  class Pathname;

  ///////////////////////////////////////////////////////////////////
  namespace sat
  { /////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : Pool
    //
    /** Global sat-pool.
     *
     * Explicitly shared singleton \ref Pool::instance.
     */
    class Pool : protected detail::PoolMember
    {
      public:
        typedef detail::SolvableIterator SolvableIterator;
        typedef detail::RepoIterator     RepoIterator;

      public:
        /** Singleton ctor. */
        static Pool instance()
        { return Pool(); }

        /** Ctor from \ref PoolMember. */
        Pool( const detail::PoolMember & )
        {}

      public:
        /** Whether \ref Pool contains repos. */
        bool reposEmpty() const;

        /** Number of repos in \ref Pool. */
        unsigned reposSize() const;

        /** Iterator to the first \ref Repo. */
        RepoIterator reposBegin() const;

        /** Iterator behind the last \ref Repo. */
        RepoIterator reposEnd() const;

        /** Return a \ref Repo named \c name_r.
         * It a such a \ref Repo does not already exist
         * a new empty \ref Repo is created.
         */
        Repo reposInsert( const std::string & name_r );

        /** Find a \ref Repo named \c name_r.
         * Returns \ref norepo if there is no such \ref Repo.
         */
        Repo reposFind( const std::string & name_r ) const;

        /** Remove a \ref Repo named \c name_r.
         * \see \ref Repo::eraseFromPool
         */
        void reposErase( const std::string & name_r );

      public:
        /** Load \ref Solvables from a solv-file into a \ref Repo named \c name_r.
         * In case of an exception the \ref Repo is removed from the \ref Pool.
         * \throws Exception if loading the solv-file fails.
         * \see \ref Repo::EraseFromPool
        */
        Repo addRepoSolv( const Pathname & file_r, const std::string & name_r );
        /** \overload Using the files basename as \ref Repo name. */
        Repo addRepoSolv( const Pathname & file_r );

      public:
        /** Whether \ref Pool contains solvables. */
        bool solvablesEmpty() const;

        /** Number of solvables in \ref Pool. */
        unsigned solvablesSize() const;

        /** Iterator to the first \ref Solvable. */
        SolvableIterator solvablesBegin() const;

        /** Iterator behind the last \ref Solvable. */
        SolvableIterator solvablesEnd() const;

      public:
        /** Expert backdoor. */
        ::_Pool * get() const;
      private:
        /** Default ctor */
        Pool() {}
    };
    ///////////////////////////////////////////////////////////////////

    /** \relates Pool Stream output */
    std::ostream & operator<<( std::ostream & str, const Pool & obj );

    /** \relates Pool */
    inline bool operator==( const Pool & lhs, const Pool & rhs )
    { return lhs.get() == rhs.get(); }

    /** \relates Pool */
    inline bool operator!=( const Pool & lhs, const Pool & rhs )
    { return lhs.get() != rhs.get(); }

    /////////////////////////////////////////////////////////////////
  } // namespace sat
  ///////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_SAT_POOL_H