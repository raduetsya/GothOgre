#ifndef GOTHOGRE_DAY_TIME_FRAME_STATS_H
#define GOTHOGRE_DAY_TIME_FRAME_STATS_H

namespace GothOgre
{
	class DayTimeFrameStatsValue;

	// Класс для отображения текущего времени в окне статистики
	class DayTimeFrameStats
	{
	public:
		DayTimeFrameStats();
		~DayTimeFrameStats();

	private:
		DayTimeFrameStatsValue* mDayTime;
	};

}

#endif // GOTHOGRE_DAY_TIME_FRAME_STATS_H