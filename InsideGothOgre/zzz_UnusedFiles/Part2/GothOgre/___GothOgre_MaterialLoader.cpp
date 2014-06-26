#include "GothOgre_Precompiled.h"
#include "GothOgre_MaterialLoader.h"
#include "GothOgre_MaterialManager.h"

namespace GothOgre
{
	MaterialLoader::MaterialLoader(const String& _name, int _order)
		: ResourceLoader(MaterialManager::getSingletonPtr(), _name, _order)
	{
	}

}