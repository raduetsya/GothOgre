#ifndef GOTHOGRE_BSP_LOADER_H
#define GOTHOGRE_BSP_LOADER_H

#include "GothOgre_ResourceLoader.h"
#include "GothOgre_Order.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT BspLoader : public ResourceLoader
	{
	public:
		BspLoader(const String& _name, int _order = Order::Normal);
	};
}

#endif // GOTHOGRE_BSP_LOADER_H