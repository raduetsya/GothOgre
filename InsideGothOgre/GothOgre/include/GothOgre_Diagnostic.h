#ifndef GOTHOGRE_DIAGNOSTIC_H
#define GOTHOGRE_DIAGNOSTIC_H

#include "GothOgre_LogSystem.h"
#include "GothOgre_Exception.h"

namespace GothOgre
{
	
	class LogStream
	{
	public:
		LogStream() {}
		String str() {return mStringStream.str();}

		template<typename T>
		friend LogStream& operator <<(LogStream& _strm, T _val)
		{
			_strm.mStringStream << _val;
			return _strm;
		}
			
	private:
		StrStream mStringStream;
	};

	//------------------------------------------------------------------------------
	// Every module defines its own implementation of these functions.
	extern "C" const String& getShortModuleName();
	extern "C" const String& getLongModuleName();

} // namespace GothOgre


// Helpful macroses
#define GOTHOGRE_SHORT_MODULE_NAME           (getShortModuleName())
#define GOTHOGRE_LONG_MODULE_NAME            (getLongModuleName())

#define GOTHOGRE_SET_MODULE_NAMES(_longname, _shortname) \
	extern "C" const GothOgre::String& getLongModuleName() \
	{ \
		static const GothOgre::String s = (_longname); \
		return s; \
	} \
	extern "C" const GothOgre::String& getShortModuleName() \
	{ \
		static const GothOgre::String s = (_shortname); \
		return s; \
	}

#define GOTHOGRE_LOG(_level, _message)       GothOgre::LogSystem::getSingleton().logMessage( GOTHOGRE_SHORT_MODULE_NAME, (GothOgre::LogLevel:: _level), (GothOgre::LogStream() << _message).str())
#define GOTHOGRE_INFO(_message)              GOTHOGRE_LOG(INFO, _message)
#define GOTHOGRE_WARNING(_message)           GOTHOGRE_LOG(WARNING, _message)
#define GOTHOGRE_ERROR(_message)             GOTHOGRE_LOG(ERROR, _message)
#define GOTHOGRE_CRITICAL(_message)          GOTHOGRE_LOG(CRITICAL, _message)

#define GOTHOGRE_EXCEPT2(_desc, _file, _line) throw GothOgre::Exception( (GothOgre::LogStream() << _desc).str(), GOTHOGRE_SHORT_MODULE_NAME, _file, _line)
#define GOTHOGRE_EXCEPT(_desc)                GOTHOGRE_EXCEPT2(_desc, __FILE__, __LINE__)
#define GOTHOGRE_ASSERT(_expr, _desc)         do { if( !(_expr) ) GOTHOGRE_EXCEPT(_desc); } while(false)

#endif // GOTHOGRE_DIAGNOSTIC_H