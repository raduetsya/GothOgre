#include "GothOgre_Precompiled.h"
#include "GothOgre_DayTimeFrameStats.h"
#include "GothOgre_DayTimeSystem.h"
#include "GothOgre_FrameStats.h"
#include "GothOgre_StringConverter.h"

namespace GothOgre
{
	class DayTimeFrameStatsValue : public FrameStatsValue
	{
	public:
		DayTimeFrameStatsValue()
			: FrameStatsValue("CurrentDayTime", true)
		{
		}

		String getValue()
		{
			DayTime dayTime = DayTimeSystem::getSingleton().getCurrentDayTime();
			return StringConverter::toString(dayTime);
		}
	};

	//-------------------------------------------------------------------
	DayTimeFrameStats::DayTimeFrameStats()
	{
		mDayTime = new DayTimeFrameStatsValue;
	}

	DayTimeFrameStats::~DayTimeFrameStats()
	{
		delete mDayTime;
	}

}