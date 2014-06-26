#ifndef GOTHOGRE_SKY_APP_BLOCK_H
#define GOTHOGRE_SKY_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_DayTimeSystem.h"
#include "GothOgre_SkySystem.h"


namespace GothOgre
{
	class SkyAppBlock : public AppBlock
	{
	public:
		SkyAppBlock()
			: mDayTimeSystem(nullptr), mSkySystem(nullptr)
		{
		}

		~SkyAppBlock()
		{
		}

		void initialise()
		{
			mDayTimeSystem = new DayTimeSystem;
			mSkySystem = new SkySystem;
		}

		void shutdown()
		{
			if(mSkySystem)
			{
				delete mSkySystem;
				mSkySystem = nullptr;
			}

			if(mDayTimeSystem)
			{
				delete mDayTimeSystem;
				mDayTimeSystem = nullptr;
			}
		}

	private:
		DayTimeSystem*              mDayTimeSystem;
		SkySystem*                  mSkySystem;
	};
}

#endif // GOTHOGRE_SKY_APP_BLOCK_H