#include "GUISystem_Precompiled.h"
#include "GUISystem_ControlSettings.h"

//-------------------------------------------------------------------------
GUISystem::ControlSettings* 
	Ogre::Singleton<GUISystem::ControlSettings>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GUISystem
{
	//-------------------------------------------------------------------------------------------------
	ControlSettings::ControlSettings(const String& _filename)
		: ConfigSettings(_filename)
	{
	}
		
} // namespace GUISystem
