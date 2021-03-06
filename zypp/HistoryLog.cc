/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file zypp/target/HistoryLog.cc
 *
 */
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "zypp/ZConfig.h"
#include "zypp/base/String.h"
#include "zypp/base/Logger.h"

#include "zypp/PathInfo.h"
#include "zypp/Date.h"

#include "zypp/PoolItem.h"
#include "zypp/Package.h"
#include "zypp/RepoInfo.h"

#include "zypp/HistoryLog.h"
#include "zypp/HistoryLogData.h"

using std::endl;
using std::string;

namespace
{
  inline string timestamp()
  { return zypp::Date::now().form( HISTORY_LOG_DATE_FORMAT ); }

  inline string userAtHostname()
  {
    static char buf[256];
    string result;
    char * tmp = ::cuserid(buf);
    if (tmp)
    {
      result = string(tmp);
      if (!::gethostname(buf, 255))
        result += "@" + string(buf);
    }
    return result;
  }

  static std::string pidAndAppname()
  {
    static std::string _val;
    if ( _val.empty() )
    {
      pid_t mypid = getpid();
      zypp::Pathname p( "/proc/"+zypp::str::numstring(mypid)+"/exe" );
      zypp::Pathname myname( zypp::filesystem::readlink( p ) );

      _val += zypp::str::numstring(mypid);
      _val += ":";
      _val += myname.basename();
    }
    return _val;
  }
}

namespace zypp
{
  namespace
  {
    const char		_sep = '|';
    std::ofstream 	_log;
    unsigned		_refcnt = 0;
    Pathname		_fname;
    Pathname		_fnameLastFail;

    inline void openLog()
    {
      if ( _fname.empty() )
        _fname = ZConfig::instance().historyLogFile();

      _log.clear();
      _log.open( _fname.asString().c_str(), std::ios::out|std::ios::app );
      if( !_log && _fnameLastFail != _fname )
      {
        ERR << "Could not open logfile '" << _fname << "'" << endl;
	_fnameLastFail = _fname;
      }
    }

    inline void closeLog()
    {
      _log.clear();
      _log.close();
    }

    inline void refUp()
    {
      if ( !_refcnt )
        openLog();
      ++_refcnt;
    }

    inline void refDown()
    {
      --_refcnt;
      if ( !_refcnt )
        closeLog();
    }
  }

  ///////////////////////////////////////////////////////////////////
  //
  //    CLASS NAME : HistoryLog
  //
  ///////////////////////////////////////////////////////////////////

  HistoryLog::HistoryLog( const Pathname & rootdir )
  {
    setRoot( rootdir );
    refUp();
  }

  HistoryLog::~HistoryLog()
  {
    refDown();
  }

  void HistoryLog::setRoot( const Pathname & rootdir )
  {
    if ( ! rootdir.absolute() )
      return;

    if ( _refcnt )
      closeLog();

    _fname = rootdir / ZConfig::instance().historyLogFile();
    filesystem::assert_dir( _fname.dirname() );
    MIL << "installation log file " << _fname << endl;

    if ( _refcnt )
      openLog();
  }

  const Pathname & HistoryLog::fname()
  {
    if ( _fname.empty() )
      _fname = ZConfig::instance().historyLogFile();
    return _fname;
  }

  /////////////////////////////////////////////////////////////////////////

  void HistoryLog::comment( const string & comment, bool timestamp )
  {
    if (comment.empty())
      return;

    _log << "# ";
    if ( timestamp )
      _log << ::timestamp() << " ";

    const char * s = comment.c_str();
    const char * c = s;
    unsigned size = comment.size();

    // ignore the last newline
    if (comment[size-1] == '\n')
      --size;

    for ( unsigned i = 0; i < size; ++i, ++c )
      if ( *c == '\n' )
      {
        _log << string( s, c + 1 - s ) << "# ";
        s = c + 1;
      }

    if ( s < c )
      _log << std::string( s, c-s );

    _log << endl;
  }

  /////////////////////////////////////////////////////////////////////////

  void HistoryLog::install( const PoolItem & pi )
  {
    const Package::constPtr p = asKind<Package>(pi.resolvable());
    if (!p)
      return;

    _log
      << timestamp()                                   // 1 timestamp
      << _sep << HistoryActionID::INSTALL.asString(true) // 2 action
      << _sep << p->name()                             // 3 name
      << _sep << p->edition()                          // 4 evr
      << _sep << p->arch();                            // 5 arch

    // ApplLow is what the solver selected on behalf of the user.
    if (pi.status().isByUser() || pi.status().isByApplLow() )
      _log << _sep << userAtHostname();                // 6 reqested by
    else if (pi.status().isByApplHigh())
      _log << _sep << pidAndAppname();
    else
      _log << _sep;

    _log
      << _sep << p->repoInfo().alias()                 // 7 repo alias
      << _sep << p->checksum().checksum();             // 8 checksum

    _log << endl;

    //_log << pi << endl;
  }


  void HistoryLog::remove( const PoolItem & pi )
  {
    const Package::constPtr p = asKind<Package>(pi.resolvable());
    if (!p)
      return;

    _log
      << timestamp()                                   // 1 timestamp
      << _sep << HistoryActionID::REMOVE.asString(true) // 2 action
      << _sep << p->name()                             // 3 name
      << _sep << p->edition()                          // 4 evr
      << _sep << p->arch();                            // 5 arch

    // ApplLow is what the solver selected on behalf of the user.
    if ( pi.status().isByUser() || pi.status().isByApplLow() )
      _log << _sep << userAtHostname();                // 6 reqested by
    else if (pi.status().isByApplHigh())
      _log << _sep << pidAndAppname();
    else
      _log << _sep;

    // we don't have checksum in rpm db
    //  << _sep << p->checksum().checksum();           // x checksum

    _log << endl;

    //_log << pi << endl;
  }

  /////////////////////////////////////////////////////////////////////////

  void HistoryLog::addRepository(const RepoInfo & repo)
  {
    _log
      << timestamp()                                   // 1 timestamp
      << _sep << HistoryActionID::REPO_ADD.asString(true) // 2 action
      << _sep << str::escape(repo.alias(), _sep)       // 3 alias
      // what about the rest of the URLs??
      << _sep << *repo.baseUrlsBegin()                 // 4 primary URL
      << endl;
  }


  void HistoryLog::removeRepository(const RepoInfo & repo)
  {
    _log
      << timestamp()                                   // 1 timestamp
      << _sep << HistoryActionID::REPO_REMOVE.asString(true) // 2 action
      << _sep << str::escape(repo.alias(), _sep)       // 3 alias
      << endl;
  }


  void HistoryLog::modifyRepository(
      const RepoInfo & oldrepo, const RepoInfo & newrepo)
  {
    if (oldrepo.alias() != newrepo.alias())
    {
      _log
        << timestamp()                                    // 1 timestamp
        << _sep << HistoryActionID::REPO_CHANGE_ALIAS.asString(true) // 2 action
        << _sep << str::escape(oldrepo.alias(), _sep)     // 3 old alias
        << _sep << str::escape(newrepo.alias(), _sep)     // 4 new alias
        << endl;
    }

    if (*oldrepo.baseUrlsBegin() != *newrepo.baseUrlsBegin())
    {
      _log
        << timestamp()                                             //1 timestamp
        << _sep << HistoryActionID::REPO_CHANGE_URL.asString(true) // 2 action
        << _sep << str::escape(oldrepo.alias(), _sep)              // 3 old url
        << _sep << *newrepo.baseUrlsBegin()                        // 4 new url
        << endl;
    }
  }

  ///////////////////////////////////////////////////////////////////

} // namespace zypp
