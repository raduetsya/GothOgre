#ifndef GOTHOGRE_ROOT_WORLD_OBJECT_H
#define GOTHOGRE_ROOT_WORLD_OBJECT_H

#include "GothOgre_BaseWorldObject_0.h"

namespace GothOgre
{
	class ZenFactory;

	/** World object to attach to the root scene node. */
	class GOTHOGRE_EXPORT RootWorldObject : public BaseWorldObject_0
	{
	public:
		/** Constructor */
		RootWorldObject();

		/** Destructor */
		virtual ~RootWorldObject();
	};

	/** Factory to create an object of class WorldRoot. */
	class GOTHOGRE_EXPORT RootWorldObjectFactory : public ZenFactory
	{
	public:
		RootWorldObjectFactory();
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // GOTHOGRE_ROOT_WORLD_OBJECT_H