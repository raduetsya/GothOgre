#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundLoader.h"
#include "GothOgre_SoundManager.h"

namespace GothOgre
{
	SoundLoader::SoundLoader(const String& _name, int _order)
		: ResourceLoader(SoundManager::getSingletonPtr(), _name, _order)
	{
	}

}