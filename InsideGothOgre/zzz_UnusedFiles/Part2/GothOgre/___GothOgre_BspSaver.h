#ifndef GOTHOGRE_BSP_SAVER_H
#define GOTHOGRE_BSP_SAVER_H

#include "GothOgre_ResourceSaver.h"
#include "GothOgre_Order.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT BspSaver : public ResourceSaver
	{
	public:
		BspSaver(const String& _name, int _order = Order::Normal);
	};
}

#endif // GOTHOGRE_BSP_SAVER_H