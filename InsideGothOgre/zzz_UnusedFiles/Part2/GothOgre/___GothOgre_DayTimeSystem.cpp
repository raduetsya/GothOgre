#include "GothOgre_Precompiled.h"
#include "GothOgre_DayTimeSystem.h"
#include "GothOgre_DayTimeListener.h"
#include "GothOgre_DayTime.h"
#include "GothOgre_DayTimeFrameStats.h"

//-------------------------------------------------------------------------
GothOgre::DayTimeSystem* 
	Ogre::Singleton<GothOgre::DayTimeSystem>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	DayTimeSystem::DayTimeSystem()
	{
		mFrameStats = new DayTimeFrameStats;

		mDefaultTimeSpeed = 10; //not confirmed
		mTimeSpeed = mDefaultTimeSpeed;
		mPaused = false;
		mCurrentDayTime = DayTime(1, 8, 0, 0); // Day 1, 8:00:00
		mOverSeconds = 0;
	}
	//----------------------------------------------------------------------
	DayTimeSystem::~DayTimeSystem()
	{
		delete mFrameStats;
	}
	//----------------------------------------------------------------------
	void DayTimeSystem::setCurrentDayTime(const DayTime& _dayTime)
	{
		if(mCurrentDayTime != _dayTime)
		{
			mCurrentDayTime = _dayTime;
			fireEvent();
		}
	}
	//----------------------------------------------------------------------
	DayTime DayTimeSystem::getCurrentDayTime() const
	{
		return mCurrentDayTime;
	}
	//----------------------------------------------------------------------
	void DayTimeSystem::setPaused(bool _paused)
	{
		mPaused = _paused;
	}
	//----------------------------------------------------------------------
	bool DayTimeSystem::isPaused() const 
	{
		return mPaused;
	}
	//----------------------------------------------------------------------
	void DayTimeSystem::setTimeSpeed(Real _timeSpeed)
	{
		mTimeSpeed = _timeSpeed;
	}
	//----------------------------------------------------------------------
	Real DayTimeSystem::getTimeSpeed() const 
	{
		return mTimeSpeed;
	}
	//----------------------------------------------------------------------
	Real DayTimeSystem::getDefaultTimeSpeed() const 
	{
		return mDefaultTimeSpeed;
	}
	//----------------------------------------------------------------------
	void DayTimeSystem::fireEvent()
	{
		if(!mListeners.empty())
		{
			Listeners::Iterator it = mListeners.getIterator();
			while(it.hasMoreElements())
			{
				BaseDayTimeListener* listener = it.getNext();
				listener->dayTimeChanged(mCurrentDayTime);
			}
		}			
	}
	//----------------------------------------------------------------------
	void DayTimeSystem::frameStarted(const FrameEvent& _evt)
	{
		if(!mPaused && mTimeSpeed != 0)
		{
			mOverSeconds += _evt.timeSinceLastFrame * mTimeSpeed;
			int iSeconds = Math::IFloor(mOverSeconds);
			mCurrentDayTime += iSeconds;
			mOverSeconds -= iSeconds;

			fireEvent();
		}
	}
	//----------------------------------------------------------------------
	void DayTimeSystem::addDayTimeListener(BaseDayTimeListener* _listener)
	{
		mListeners.add(_listener);
	}
	//----------------------------------------------------------------------
	void DayTimeSystem::removeDayTimeListener(BaseDayTimeListener* _listener)
	{
		mListeners.remove(_listener);
	}

}