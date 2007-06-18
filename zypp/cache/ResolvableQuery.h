
#ifndef ZYPP_CACHE_RESOLVABLE_QUERY_H
#define ZYPP_CACHE_RESOLVABLE_QUERY_H

#include <set>
#include <list>
#include "zypp/base/Function.h"
#include "zypp/Pathname.h"
#include "zypp/data/ResolvableData.h"
#include "zypp/data/RecordId.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  namespace cache
  { /////////////////////////////////////////////////////////////////
 
   /**
    * The resolvable query class allows you to query for resolvable
    * data and properties from the cache.
    */
    struct ResolvableQuery
    {
    public:
     /**
      * Callback definition
      * first parameter is the resolvable id.
      * second parameter is a \ref data::ResObjectData object with the resource
      */
      typedef function<bool( const data::RecordId &, 
                             data::ResObject_Ptr )> ProcessResolvable;
      
      /**
       * Constructor
       *
       * \param dbdir Cache location path
       */
      ResolvableQuery( const Pathname &dbdir );
      
      /**
      * Query by record id
      * \param record_id Resolvable id to query
      * \param fnc callback to send the data to. (Will be called once or none)
      */
      void query( const data::RecordId &record_id,
                  ProcessResolvable fnc  );
      
      /**
      * Query by matching text
      * \param text text to match
      * \param fnc callback to send the data to. (Will be called once per result)
      */
      void query( const std::string &text,
                  ProcessResolvable fnc  );
      
      /**
       * Queries a specifc attribute for a resolvable
       *
       * \param record_id Resolvable cache id
       * \param klass Attribute Class
       * \param name Attribute Name
       *
       * \return The attribute or -1 if
       * no record is found.
       */
      int queryNumericAttribute( const data::RecordId &record_id,
                                 const std::string &klass,
                                 const std::string &name,
                                 int default_value = -1 );


      /**
       * Queries a specifc attribute for a resolvable
       *
       * \param record_id Resolvable cache id
       * \param klass Attribute Class
       * \param name Attribute Name
       *
       * \return The bool value of the attribute or <tt>false</tt>
       *         if no record is found.
       */
      bool queryBooleanAttribute( const data::RecordId &record_id,
                                  const std::string &klass,
                                  const std::string &name,
                                  bool default_value = false );


      /**
       * Queries a specifc attribute for a resolvable
       *
       * \param record_id Resolvable cache id
       * \param klass Attribute Class
       * \param name Attribute Name
       *
       * \return The attribute or a empty string if
       * no record is found.
       */
      std::string queryStringAttribute( const data::RecordId &record_id,
                                        const std::string &klass,
                                        const std::string &name,
                                        const std::string &default_value = std::string() );
      
      /**
       * Queries a specifc attribute translation
       * for a resolvable.
       *
       * \param record_id Resolvable cache id
       * \param locale Locale of the translation
       * \param klass Attribute Class
       * \param name Attribute Name
       *
       * \return The attribute or a empty string if
       * no record is found.
       */
      std::string queryStringAttributeTranslation( const data::RecordId &record_id,
                                                   const Locale &locale,
                                                   const std::string &klass,
                                                   const std::string &name,
                                                   const std::string &default_value = std::string() );
      
      /**
       * Queries all translations for a specific attribute
       * in a resolvable.
       *
       * \param record_id Resolvable cache id
       * \param klass Attribute Class
       * \param name Attribute Name
       *
       * \return all attribute translations or a empty 
       * \ref TranslatedString if no record is found.
       */
      TranslatedText queryTranslatedStringAttribute( const data::RecordId &record_id,
                                                     const std::string &klass,
                                                     const std::string &name,
                                                     const TranslatedText &default_vaue = TranslatedText() );
      
      /**
       * Queries for a specific container attribute
       * in a resolvable.
       *
       * \param record_id Resolvable cache id
       * \param klass Attribute Class
       * \param name Attribute Name
       *
       * The results are filled to the provided
       * \ref _OutputIterator
       */
      template<class _OutputIterator>
      void queryStringContainerAttribute( const data::RecordId &record_id,
                                    const std::string &klass,
                                    const std::string &name,
                                    _OutputIterator result )
      {
        
        std::string all = queryStringAttribute( record_id, klass, name);
        //FIXME use zypp separator
        str::split( all, result );
      }
      
      
      
    private:
      /** Implementation. */
      class Impl;
      /** Pointer to implementation. */
      RW_pointer<Impl> _pimpl;
    };
  } //NS cache
} //NS zypp

#endif