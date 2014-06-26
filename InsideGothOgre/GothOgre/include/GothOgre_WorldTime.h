#ifndef GOTHOGRE_WORLD_TIME_H
#define GOTHOGRE_WORLD_TIME_H

#include "GothOgre_Time.h"
#include "GothOgre_WorldTimeListener.h"
#include "GothOgre_SafeOrderedList.h"
#include "GothOgre_FrameListener.h"

namespace GothOgre
{
	/** The class to operate with world time. */
	class GOTHOGRE_EXPORT WorldTime : public Singleton<WorldTime>
	{
	public:
		WorldTime();
		~WorldTime();

		/** Returns the current time. */
		const Time& getCurrentTime() const {return mCurrentTime;}

		/** Returns the current day. */
		int getCurrentDay() const {return mCurrentDay;}

		/** Returns the current hour. */
		int getCurrentHour() const {return mCurrentHour;}

		/** Returns the current minute. */
		int getCurrentMinute() const {return mCurrentMinute;}

		/** Returns the current second. */
		double getCurrentSecond() const {return mCurrentSecond;}

		/** Replace day in a specified daytime with (getCurrentDay() - 1). */
		Time getYesterdayTime(const Time& _time) const {Time r = _time; r.setDay(getCurrentDay() - 1); return r;}

		/** Sets the current time. The function can be used
		to setup the start time when the game begins;
		or to skip some amount of world time when the hero is sleeping. */
		void setCurrentTime(const Time& _time);

		/** Returns the default world time's speed.
		This value is a constant. */
		double getDefaultTimeSpeed() const {return mDefaultTimeSpeed;}

		/** Returns the current world time's speed.
		It's number of world seconds per one real seconds. 
		The speed is a value what can change. */
		double getTimeSpeed() const {return mTimeSpeed;}

		/** Changes the world time's speed. 
		The function can be used to create different effects,
		for example, a time dilation spell. */
		void setTimeSpeed(double _timeSpeed);

		/** Pauses the world time. */
		void pause(bool _pause = true);

		/** Unpauses the world time. */
		void unpause() {pause(false);}

		/** Is the world time paused? */
		bool isPaused() const {return mPaused;}

	private:
		void fireWorldTimeCame();

		GOTHOGRE_DECLARE_FRAME_STARTED( WorldTime );

	private:
		friend class WorldTimeListener;
		void _initListener(WorldTimeListener* _listener);
		void _addListener(WorldTimeListener* _listener, const Time& _firstActivation, const Time& _period);
		void _removeListener(WorldTimeListener* _listener);

	private:
		typedef SafeOrderedList<Time, WorldTimeListener*> Listeners;;

		Listeners  mListeners;
		double     mDefaultTimeSpeed;
		double     mTimeSpeed;
		bool       mPaused;
		Time       mCurrentTime;
		int        mCurrentDay;
		int        mCurrentHour;
		int        mCurrentMinute;
		double     mCurrentSecond;
	};
}

#endif // GOTHOGRE_WORLD_TIME_H