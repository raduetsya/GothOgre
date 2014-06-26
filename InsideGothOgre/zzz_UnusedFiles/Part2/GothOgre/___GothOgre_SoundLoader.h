#ifndef GOTHOGRE_SOUND_LOADER_H
#define GOTHOGRE_SOUND_LOADER_H

#include "GothOgre_ResourceLoader.h"
#include "GothOgre_Order.h"

namespace GothOgre
{
	// Загрузчик для звуковых ресурсов
	class GOTHOGRE_EXPORT SoundLoader : public ResourceLoader
	{
	public:
		SoundLoader(const String& _name, int _order = Order::Normal);
	};
}

#endif // GOTHOGRE_SOUND_LOADER_H