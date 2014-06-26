#ifndef GOTHOGRE_DAY_TIME_SYSTEM_H
#define GOTHOGRE_DAY_TIME_SYSTEM_H

#include "GothOgre_DayTime.h"
#include "GothOgre_SafeList.h"
#include "GothOgre_FrameListener.h"

namespace GothOgre
{
	class DayTimeFrameStats;

	class GOTHOGRE_EXPORT DayTimeSystem : public Singleton<DayTimeSystem>, 
		                                  public AutoFrameListener
	{
	public:
		DayTimeSystem();
		~DayTimeSystem();

		void setCurrentDayTime(const DayTime& _dayTime);
		DayTime getCurrentDayTime() const;

		void setPaused(bool _paused);
		void pause() {setPaused(true);}
		void unpause() {setPaused(false);}
		bool isPaused() const;

		void setTimeSpeed(Real _timeSpeed);
		Real getTimeSpeed() const;

		Real getDefaultTimeSpeed() const;

	private:
		void fireEvent();
		void frameStarted(const FrameEvent& _evt);

	private:
		friend class BaseDayTimeListener;
		void addDayTimeListener(BaseDayTimeListener* _listener);
		void removeDayTimeListener(BaseDayTimeListener* _listener);

	private:
		typedef SafeList<BaseDayTimeListener*> Listeners;
		Listeners  mListeners;
		
		DayTime    mCurrentDayTime;
		Real       mOverSeconds;
		Real       mTimeSpeed; //How many "Game"seconds must be in one "Real"second.
		Real       mDefaultTimeSpeed;
		bool       mPaused;

		DayTimeFrameStats* mFrameStats;
	};
}

#endif // GOTHOGRE_DAY_TIME_SYSTEM_H