#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundSettings.h"


//-------------------------------------------------------------------------
GothOgre::SoundSettings* 
	Ogre::Singleton<GothOgre::SoundSettings>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//---------------------------------------------------------------------
	SoundSettings::SoundSettings(const String& _filename)
		: ConfigSettings(_filename)
	{
	}

} // namespace GothOgre