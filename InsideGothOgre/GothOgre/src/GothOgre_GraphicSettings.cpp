#include "GothOgre_Precompiled.h"
#include "GothOgre_GraphicSettings.h"


//-------------------------------------------------------------------------
GothOgre::GraphicSettings* 
	Ogre::Singleton<GothOgre::GraphicSettings>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//---------------------------------------------------------------------
	GraphicSettings::GraphicSettings(const String& _filename)
		: ConfigSettings(_filename)
	{
	}

} // namespace GothOgre