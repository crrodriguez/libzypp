/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/FileChecker.h
 *
*/
#ifndef ZYPP_FILECHECKER_H
#define ZYPP_FILECHECKER_H

#include <iosfwd>
#include <list>
#include "zypp/base/Exception.h"
#include "zypp/base/Function.h"
#include "zypp/Pathname.h"
#include "zypp/CheckSum.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  /**
   * Functor signature used to check files.
   * \param file File to check.
   *
   * \throws FileCheckException when the file does not
   * validate and the user don't want to continue.
   */
  typedef function<void ( const Pathname &file )> FileChecker;
  
  class FileCheckException : public Exception
  {
  public:
    FileCheckException(const std::string &msg)
      : Exception(msg)
    {}
  };
  
  class CheckSumCheckException : public FileCheckException
  {
    //TODO
  };
  
  class SignatureCheckException : public FileCheckException
  {
    //TODO
  };
  
  /**
   * Built in file checkers
   */
  
  /**
   * \short Checks for a valid checksum and interacts with the user.
   */
   class ChecksumFileChecker
   {
   public:
     /**
      * Constructor.
      * \param checksum Checksum that validates the file
      */
     ChecksumFileChecker( const CheckSum &checksum );
     /**
      * \short Try to validate the file
      * \param file File to validate.
      *
      * \throws CheckSumCheckException if validation fails
      */
     void operator()( const Pathname &file ) const;
   private:
     CheckSum _checksum;
   };
   
   /**
    * \short Checks for the validity of a signature
    */
   class SignatureFileChecker
   {
     public:
      /**
      * Constructor.
      * \param signature Signature that validates the file
      */
      SignatureFileChecker( const Pathname &signature );
      
      /**
      * Default Constructor.
      * \short Signature for unsigned files
      * Use it when you dont have a signature but you want
      * to check the user to accept an unsigned file.
      */
      SignatureFileChecker();
      
      
      /**
       * add a public key to the list of known keys
       */
      void addPublicKey( const Pathname &publickey );
      /**
      * \short Try to validate the file
      * \param file File to validate.
      *
      * \throws SignatureCheckException if validation fails
      */
      void operator()( const Pathname &file ) const;
     
     private:
      Pathname _signature;
   };
   
   /**
   * \short Checks for nothing
   * Used as the default checker
   */
   class NullFileChecker
   {
   public:
     void operator()( const Pathname &file )  const;
   };
    
   /**
    * \short Checker composed of more checkers.
    * 
    * Allows to create a checker composed of various
    * checkers altothether. It will only
    * validate if all the checkers validate.
    *
    * \code
    * CompositeFileChecker com;
    * com.add(checker1);
    * com.add(checker2);
    * fetcher.enqueue(location, com);
    * \endcode
    */
   class CompositeFileChecker
   {
   public:
     void add( const FileChecker &checker );
    /**
     * \throws FileCheckException if validation fails
     */
     void operator()( const Pathname &file ) const;
   private:
     std::list<FileChecker> _checkers;
   };

  /** \relates FileChecker Stream output */
  std::ostream & operator<<( std::ostream & str, const FileChecker & obj );

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_FILECHECKER_H