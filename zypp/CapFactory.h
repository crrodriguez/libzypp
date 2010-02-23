/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/CapFactory.h
 *
*/
#ifndef ZYPP_CAPFACTORY_H
#define ZYPP_CAPFACTORY_H

#include <iosfwd>

#include "zypp/base/PtrTypes.h"

#include "zypp/Capability.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : CapFactory
  //
  /** Factory for creating Capability.
   *
   * Besides parsing, CapFactory asserts that equal capabilites
   * share the same implementation.
   *
   * \todo define EXCEPTIONS
   * \todo Parser needs improvement in speed and accuracy.
  */
  class CapFactory
  {
    friend std::ostream & operator<<( std::ostream & str, const CapFactory & obj );

  public:
    /** Default ctor */
    CapFactory();

    /** Dtor */
    ~CapFactory();

  public:
    /** Parse Capability from string providing Resolvable::Kind.
     * \a strval_r is expected to define a valid Capability.
     * \throw EXCEPTION on parse error.
    */
    Capability parse( const Resolvable::Kind & refers_r,
                      const std::string & strval_r ) const;


    /** Parse Capability providing Resolvable::Kind, name, Rel and Edition as strings.
     * \throw EXCEPTION on parse error.
    */
    Capability parse( const Resolvable::Kind & refers_r,
                      const std::string & name_r,
                      const std::string & op_r,
                      const std::string & edition_r ) const;

    /** Parse Capability providing Resolvable::Kind, name, Rel and Edition.
     * \throw EXCEPTION on parse error.
    */
    Capability parse( const Resolvable::Kind & refers_r,
                      const std::string & name_r,
                      Rel op_r,
                      const Edition & edition_r ) const;

    /** Special Capability, triggering evaluation of Hal
     * capabilities when matched.
    */
    Capability halEvalCap() const;

    /** Special Capability, triggering evaluation of modalias
     * capabilities when matched.
    */
    Capability modaliasEvalCap() const;

  public:
    /** Provide a parsable string representation of \a cap_r. */
    std::string encode( const Capability & cap_r ) const;

  private:
    /** Implementation */
    struct Impl;
    /** Pointer to implementation */
    RW_pointer<Impl> _pimpl;
  };
  ///////////////////////////////////////////////////////////////////

  /** \relates CapFactory Stream output */
  extern std::ostream & operator<<( std::ostream & str, const CapFactory & obj );

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_CAPFACTORY_H