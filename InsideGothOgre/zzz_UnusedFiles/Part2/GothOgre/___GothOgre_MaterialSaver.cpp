#include "GothOgre_Precompiled.h"
#include "GothOgre_MaterialSaver.h"
#include "GothOgre_MaterialManager.h"

namespace GothOgre
{
	MaterialSaver::MaterialSaver(const String& _name, int _order)
		: ResourceSaver(MaterialManager::getSingletonPtr(), _name, _order)
	{
	}

}