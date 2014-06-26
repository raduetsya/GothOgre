#include "GothOgre_Precompiled.h"
#include "GothOgre_GameTime.h"

#pragma warning(disable: 4996)

//-------------------------------------------------------------------------
GothOgre::GameTime* Ogre::Singleton<GothOgre::GameTime>::ms_Singleton = nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
//-------------------------------------------------------------------------------------------------
	GameTime::GameTime()
	{
		reset();
		start();

		time(&timer);
		timeinfo = (localtime(&timer));
		mLastSec = timeinfo->tm_sec;

		mCounter = 0;
	}
//-------------------------------------------------------------------------------------------------
	GameTime::~GameTime()
	{
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::init()
	{
		registerFrameListener();
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::shutdown()
	{
		unregisterFrameListener();
	}
//-------------------------------------------------------------------------------------------------
	size_t GameTime::getTimeSec()
	{
		return mSec;
	}
//-------------------------------------------------------------------------------------------------
	size_t GameTime::getTimeMin()
	{
		return mMin;
	}
//-------------------------------------------------------------------------------------------------
	size_t GameTime::getTimeHour()
	{
		return mHour;
	}
//-------------------------------------------------------------------------------------------------
	size_t GameTime::getDay()
	{
		return mDay;
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::setTimeMin(size_t _newMin)
	{
		mMin = _newMin;
		mSec = 0;
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::setTimeHour(size_t _newHour)
	{
		mHour = _newHour;
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::setDay(size_t _newDay)
	{
		mDay = _newDay;
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::reset()
	{
		mDay  = 1;
		mHour = 0;
		mMin  = 0;
		mSec  = 0;
		mTimeSpeed = 0;
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::start()
	{
		mTimeSpeed = STANDART_TIME_SPEED;
	}

//-------------------------------------------------------------------------------------------------
	void GameTime::stop()
	{
		mTimeSpeed = 0;
	}
//-------------------------------------------------------------------------------------------------
	void GameTime::frameEnded(const FrameEvent& evt)
	{
		time(&timer);
		timeinfo = (localtime(&timer));
		mCurSec = timeinfo->tm_sec;
		if (mCurSec != mLastSec)
		{
			mLastSec = mCurSec;
			mSec += mTimeSpeed;
			size_t mAddSec = mSec/60;
			if (mAddSec >= 1)
			{
				mMin += mAddSec;
				mSec -= mAddSec*60;
				if (mMin >= 60)
				{
					mHour++;
					mMin -=60;
					if (mHour >= 24)
					{
						mDay++;
						mHour -=24;
					}
				}
				fireEvent(nullptr, GameTimeEvent::TimeTick);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------
	void GameTime::addListener(GameTimeListener* _listener)
	{
		mListeners.add(_listener);
		
		// send the first event to initialize the listener
		fireEvent(_listener, GameTimeEvent::ListenerAdded);
	}
	//-------------------------------------------------------------------------------------------------
	void GameTime::removeListener(GameTimeListener* _listener)
	{
		mListeners.remove(_listener);
	}
	//-------------------------------------------------------------------------------------------------
	void GameTime::fireEvent(GameTimeListener* _listener, 
		GameTimeEvent::Reason _reason)
	{
		GameTimeEvent evt(_reason, mCounter++);

		if(_listener)
		{
//			GOTHOGRE_LOG(Info, "Sending event to 1 receiver");
			_listener->gameTimeUpdated(evt, mHour, mMin, mSec, mDay);
		}
		else if(!mListeners.empty())
		{
//			GOTHOGRE_LOG(Info, "Sending event to " << mListeners.size() << " receivers");
			Listeners::Iterator it = mListeners.getIterator();
			while(it.hasMoreElements())
			{
				GameTimeListener* listener = it.getNext();
				listener->gameTimeUpdated(evt, mHour, mMin, mSec, mDay);
			}
		}
	}
		

} // namespace GothOgre
