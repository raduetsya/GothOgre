#include "GothOgre_Precompiled.h"
#include "GothOgre_DefaultLogListener.h"
#include "GothOgre_LogSystem.h"
#include "GothOgre_SystemTime.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------------------------------------
	DefaultLogListener::DefaultLogListener(const String& _filename)
	{
		mfpLog.open(_filename.c_str());
		registerLogListener();
	}
	//--------------------------------------------------------------------------------------------------------
	DefaultLogListener::~DefaultLogListener()
	{
		mfpLog.close();
	}
	//--------------------------------------------------------------------------------------------------------
	void DefaultLogListener::messageLogged(const String& _module, LogLevel _lvl, const String& _message)
	{
		StrStream ss;
		ss.precision(3);
		ss << SystemTime::getSingleton().getCurrentTime();

		size_t numSpaces = (_module.length() < 13) ? (13 - _module.length()) : 0;
		ss << " | " << _module << String(numSpaces, ' ');
		ss << " | ";
		ss << _lvl;
		ss << " | " << _message << std::endl;

		// Flush to ensure it is written (in case of crash, we need log to be up to date)
		mfpLog << ss.str();
		mfpLog.flush();
	}
	
}