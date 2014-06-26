#ifndef GOTHOGRE_WORLD_OBJECT_WITH_UNLINK_H
#define GOTHOGRE_WORLD_OBJECT_WITH_UNLINK_H

#include "GothOgre_WorldObject.h"


namespace GothOgre
{
	class WorldObjectWithUnlink : public WorldObject
	{
	protected:
		/** Remove references to the instance before destroying it.
		Called in destructors of classes BaseWorldObject, RootWorldObject 
		and DerivedWorldObject. */
		void unlink();
	};
}


#endif // GOTHOGRE_WORLD_OBJECT_WITH_UNLINK_H