#ifndef GOTHOGRE_WORLD_CHANGED_STATE_FREEZER_H
#define GOTHOGRE_WORLD_CHANGED_STATE_FREEZER_H

#include "GothOgre_World.h"


namespace GothOgre
{
	/** This class is designed to temporarily "freeze"
	the "changed" internal variable of a world.
	It's useful 
	*/
	class GOTHOGRE_EXPORT WorldChangedStateFreezer
	{
	public:
		WorldChangedStateFreezer(World* _world)
		{
			mWorld = _world;
			mWorld->freezeChangedState();
		}

		~WorldChangedStateFreezer()
		{
			mWorld->unfreezeChangedState();
		}

	private:
		WorldChangedStateFreezer(const WorldChangedStateFreezer&) {}

	private:
		World* mWorld;
	};

} // namespace GothOgre

#endif // GOTHOGRE_WORLD_CHANGED_STATE_FREEZER_H