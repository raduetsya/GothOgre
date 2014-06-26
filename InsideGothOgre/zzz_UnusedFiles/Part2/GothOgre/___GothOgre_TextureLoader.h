#ifndef GOTHOGRE_TEXTURE_LOADER_H
#define GOTHOGRE_TEXTURE_LOADER_H

#include "GothOgre_ResourceLoader.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT TextureLoader : public ResourceLoader
	{
	public:
		TextureLoader(const String& _name, int _order = Order::Normal);
	};
}

#endif // GOTHOGRE_TEXTURE_LOADER_H