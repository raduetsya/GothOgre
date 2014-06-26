#include "Precompiled.h"
#include "ExtSceneManagerFrameStats.h"
#include "ExtSceneManager.h"

namespace GothOgre
{
	class CurrentSectorFrameStatsValue : public FrameStatsValue
	{
	public:
		CurrentSectorFrameStatsValue(ExtSceneManager* _manager)
			: FrameStatsValue("CurrentSector", false), mManager(_manager)
		{
			mManager = _manager;
		}

		String getValue()
		{
			Camera* camera = World::getSingleton().getCamera();
			String ret = mManager->getSector(camera);
			if(ret.empty() && mManager->isUnderwater(camera))
				ret = "Underwater";
			return ret;
		}

	private:
		ExtSceneManager* mManager;
	};


	//---------------------------------------------------------------------
	ExtSceneManagerFrameStats::ExtSceneManagerFrameStats(ExtSceneManager* _manager)
	{
		mCurrentSector = new CurrentSectorFrameStatsValue(_manager);
	}

	ExtSceneManagerFrameStats::~ExtSceneManagerFrameStats()
	{
		delete mCurrentSector;
	}

}