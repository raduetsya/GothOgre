#include "GUISystem_Precompiled.h"
#include "GUISystem_UISettings.h"

//-------------------------------------------------------------------------
GUISystem::UISettings* 
	Ogre::Singleton<GUISystem::UISettings>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GUISystem
{
	//-------------------------------------------------------------------------------------------------
	UISettings::UISettings(const String& _filename)
		: ConfigSettings(_filename)
	{
	}
		
} // namespace GUISystem
