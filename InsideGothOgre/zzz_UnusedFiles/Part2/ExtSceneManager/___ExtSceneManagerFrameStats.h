#ifndef EXTSCENEMANAGER_FRAME_STATS_H
#define EXTSCENEMANAGER_FRAME_STATS_H

namespace GothOgre
{
	class ExtSceneManager;
	class CurrentSectorFrameStatsValue;

	class ExtSceneManagerFrameStats
	{
	public:
		ExtSceneManagerFrameStats(ExtSceneManager* _manager);
		~ExtSceneManagerFrameStats();

	private:
		CurrentSectorFrameStatsValue* mCurrentSector;
	};
}

#endif // EXTSCENEMANAGER_FRAME_STATS_H