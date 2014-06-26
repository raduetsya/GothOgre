#ifndef GOTHOGRE_BASE_WORLD_OBJECT_H
#define GOTHOGRE_BASE_WORLD_OBJECT_H

#include "GothOgre_BaseWorldObject_0.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT BaseWorldObject : public BaseWorldObject_0
	{
	public:
		/** Constructor */
		BaseWorldObject();

		/** Destructor */
		virtual ~BaseWorldObject();
	};

	/** Factory to create an object of class WorldRoot. */
	class GOTHOGRE_EXPORT BaseWorldObjectFactory : public ZenFactory
	{
	public:
		BaseWorldObjectFactory();
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // GOTHOGRE_BASE_WORLD_OBJECT_H