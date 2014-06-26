#include "GothOgre_Precompiled.h"
#include "GothOgre_SystemTime.h"

//--------------------------------------------------------------------------------
GothOgre::SystemTime* 
	Ogre::Singleton<GothOgre::SystemTime>::ms_Singleton 
	= nullptr;
//--------------------------------------------------------------------------------


namespace GothOgre
{
	//--------------------------------------------------------------------------------
	SystemTime::SystemTime()
	{
	}
	//--------------------------------------------------------------------------------
	SystemTime::~SystemTime()
	{
	}
	//--------------------------------------------------------------------------------
	Time SystemTime::getCurrentTime() const
	{
		Time t;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		SYSTEMTIME st;
		::GetLocalTime(&st);
		t.assign( st.wDay, st.wHour, st.wMinute, st.wSecond + st.wMilliseconds / 1000.);
#else
	    time_t ctTime; 
		time(&ctTime);
		struct tm loctm;
		localtime_s(&loctm, &ctTime );
		t.assign( loctm.tm_mday, loctm.tm_hour, loctm.tm_min, loctm.tm_sec);
#endif	
		return t;
	}

} // namespace GothOgre