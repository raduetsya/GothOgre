#ifndef GOTHOGRE_APP_BLOCK_SKY_H
#define GOTHOGRE_APP_BLOCK_SKY_H

#include "GothOgre_AppBlock.h
#include "GothOgre_DayTimeSystem.h"
#include "GothOgre_SkySystem.h"


namespace GothOgre
{
	class SkyAppBlock : public AppBlock
	{
	public:
		SkyAppBlock()
			: mEncodingSettings(nullptr), mGraphicsSettings(nullptr)
		{
			mDayTimeSystem = new DayTimeSystem;
			mSkySystem = new SkySystem;
		}

		~SkyAppBlock()
		{
			if(mSkySystem)
				delete mSkySystem;

			if(mDayTimeSystem)
				delete mDayTimeSystem;
		}

	private:
		DayTimeSystem*              mDayTimeSystem;
		SkySystem*                  mSkySystem;
	};
}

#endif // GOTHOGRE_APP_BLOCK_SKY_H