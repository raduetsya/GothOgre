#ifndef GOTHOGRE_MATERIAL_LOADER_H
#define GOTHOGRE_MATERIAL_LOADER_H

#include "GothOgre_ResourceLoader.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT MaterialLoader : public ResourceLoader
	{
	public:
		MaterialLoader(const String& _name, int _order = Order::Normal);
	};
}

#endif // GOTHOGRE_MATERIAL_LOADER_H