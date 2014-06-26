#ifndef GOTHOGRE_MATERIAL_SAVER_H
#define GOTHOGRE_MATERIAL_SAVER_H

#include "GothOgre_ResourceSaver.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT MaterialSaver : public ResourceSaver
	{
	public:
		MaterialSaver(const String& _name, int _order = Order::Normal);
	};
}

#endif // GOTHOGRE_MATERIAL_SAVER_H