#include "GUISystem_Precompiled.h"
#include "GUISystem_WorldEditSettings.h"

//-------------------------------------------------------------------------
GUISystem::WorldEditSettings* 
	Ogre::Singleton<GUISystem::WorldEditSettings>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GUISystem
{
	//-------------------------------------------------------------------------------------------------
	WorldEditSettings::WorldEditSettings(const String& _filename)
		: ConfigSettings(_filename)
	{
	}
		
} // namespace GUISystem
