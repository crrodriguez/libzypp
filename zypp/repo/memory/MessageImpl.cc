/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/

#include "zypp/TranslatedText.h"
#include "zypp/base/String.h"
#include "zypp/base/Logger.h"
#include "zypp/repo/RepositoryImpl.h"
#include "MessageImpl.h"


using namespace std;
using namespace zypp::detail;
using namespace::zypp::repo;

///////////////////////////////////////////////////////////////////
namespace zypp { namespace repo { namespace memory {

///////////////////////////////////////////////////////////////////
//
//        CLASS NAME : MessageImpl
//
///////////////////////////////////////////////////////////////////

/** Default ctor
*/
MessageImpl::MessageImpl (const data::RecordId &id, memory::RepoImpl::Ptr repository_r)
    : _repository (repository_r),
      _id(id)
{}

Repository
MessageImpl::repository() const
{
  return _repository->selfRepository();
}

///////////////////////////////////////////////////
// ResObject Attributes
///////////////////////////////////////////////////

TranslatedText MessageImpl::summary() const
{
  return _summary;
}

TranslatedText MessageImpl::description() const
{
  return _description;
}

TranslatedText MessageImpl::insnotify() const
{
  return _insnotify;
}

TranslatedText MessageImpl::delnotify() const
{
  return _delnotify;
}

TranslatedText MessageImpl::licenseToConfirm() const
{
  return _license_to_confirm;
}

Vendor MessageImpl::vendor() const
{
  return _vendor;
}

ByteCount MessageImpl::size() const
{
  return _size;
}

ByteCount MessageImpl::archivesize() const
{
  return _archivesize;
}

bool MessageImpl::installOnly() const
{
  return _install_only;
}

Date MessageImpl::buildtime() const
{
  return _buildtime;
}

Date MessageImpl::installtime() const
{
  return _installtime;
}

unsigned MessageImpl::mediaNr() const
{
  return _media_nr;
}


//////////////////////////////////////////
// DEPRECATED
//////////////////////////////////////////

Source_Ref MessageImpl::source() const
{
  return Source_Ref::noSource;
}

unsigned MessageImpl::mediaNr() const
{
  return 1;
}

//////////////////////////////////////////
// MESSAGE
/////////////////////////////////////////

TranslatedText MessageImpl::text() const
{
  return _repository->resolvableQuery().queryTranslatedStringAttribute( _id, "Message", "text" );
}

Patch::constPtr MessageImpl::patch() const
{
  return 0;
}
    
/////////////////////////////////////////////////////////////////
} } } // namespace zypp::repo::memory
///////////////////////////////////////////////////////////////////
