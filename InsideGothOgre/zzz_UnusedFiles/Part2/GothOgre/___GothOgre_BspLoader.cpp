#include "GothOgre_Precompiled.h"
#include "GothOgre_BspLoader.h"
#include "GothOgre_BspManager.h"

namespace GothOgre
{
	BspLoader::BspLoader(const String& _name, int _order)
		: ResourceLoader(BspManager::getSingletonPtr(), _name, _order)
	{
	}

}