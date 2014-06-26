#ifndef GOTHOGRE_GAMETIME_H
#define GOTHOGRE_GAMETIME_H

#include "GothOgre_SafeList.h"
#include "GothOgre_FrameListener.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT GameTimeEvent
	{
	public:
		enum Reason
		{
			// A new listener has been added; this event is sent to only added listener
			ListenerAdded,
			// time tick
			TimeTick
		};

		GameTimeEvent(Reason _reason, size_t _counter) {mReason = _reason; mCounter = _counter;}
		Reason getReason() const {return mReason;}
		size_t getCounter() const {return mCounter;}

	private:
		Reason mReason;
		size_t mCounter;
	};

	class GOTHOGRE_EXPORT GameTimeListener
	{
	public:
		virtual void gameTimeUpdated( const GameTimeEvent& _evt, size_t _hour, size_t _min, size_t _sec, size_t _day) = 0;
	};

	class GOTHOGRE_EXPORT GameTime : public Singleton<GameTime>, public ManualFrameListener
	{
	public:
		GameTime();
		~GameTime();
		void init();
		void shutdown();
		void reset();
		void start();
		void stop();
		size_t getTimeSec();
		size_t getTimeMin();
		size_t getTimeHour();
		size_t getDay();
		void setTimeSec(size_t _newSec);
		void setTimeMin(size_t _newMin);
		void setTimeHour(size_t _newHour);
		void setDay(size_t _newDay);

		void addListener(GameTimeListener* _listener);
		void removeListener(GameTimeListener* _listener);

	private:
		void fireEvent(GameTimeListener* _listener, 
			GameTimeEvent::Reason _reason);

		typedef SafeList<GameTimeListener*>	Listeners;
		Listeners							mListeners;
		size_t								mCounter;

	private:
		size_t mDay;
		size_t mHour;
		size_t mMin;
		size_t mSec;
//How many "Game"seconds must be in one "Real"second.
//0 <= mTimeSpeed < 3540 (59 "Game"min) 
		size_t mTimeSpeed; 

		static const size_t STANDART_TIME_SPEED = 10; //not confirmed

		time_t timer;
		struct tm * timeinfo;

		size_t mLastSec;
		size_t mCurSec;

		void frameEnded(const FrameEvent& evt);

	};

} // namespace GothOgre

#endif // GOTHOGRE_GAMETIME_H