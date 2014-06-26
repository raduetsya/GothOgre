#ifndef GOTHOGRE_STANDARD_FRAME_STATS_VALUES_H
#define GOTHOGRE_STANDARD_FRAME_STATS_VALUES_H

#include "GothOgre_FrameStats.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	/** This class contains some standard frame statistics' values:
	camera position, FPS, triangle count, and so on. */
	class StandardFrameStatsValues
	{
	public:
		/** The constructor adds values to the singleton 
		of the FrameStats class. */
		StandardFrameStatsValues();

		/** The destructor removes values from the singleton 
		of the FrameStats class. */
		~StandardFrameStatsValues();

	private:
		typedef list<FrameStatsValue*>::type List;
		List  mList;
	};

}

#endif // GOTHOGRE_STANDARD_FRAME_STATS_VALUES_H