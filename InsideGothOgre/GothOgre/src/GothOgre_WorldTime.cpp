#include "GothOgre_Precompiled.h"
#include "GothOgre_WorldTime.h"

//-------------------------------------------------------------------------
GothOgre::WorldTime* 
	Ogre::Singleton<GothOgre::WorldTime>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	WorldTime::WorldTime()
	{
		registerFrameListener();

		// Measured Gothic I value:
		// twenty-four hours in game / 100 minutes in real life = 
		// 24 * 60 / 100 = 14.4 game minutes per one real minute.
		mDefaultTimeSpeed = 14.4f;
		mTimeSpeed = mDefaultTimeSpeed;
		mPaused = false;
		setCurrentTime(Time(1, 8, 0, 0)); // Day 1, 8:00
	}
	//----------------------------------------------------------------------
	WorldTime::~WorldTime()
	{
	}
	//----------------------------------------------------------------------
	void WorldTime::setCurrentTime(const Time& _time)
	{
		mCurrentTime = _time;
		mCurrentTime.unpack(mCurrentDay, mCurrentHour, mCurrentMinute, mCurrentSecond);
		fireWorldTimeCame();
	}
	//----------------------------------------------------------------------
	void WorldTime::pause(bool _pause)
	{
		mPaused = _pause;
	}
	//----------------------------------------------------------------------
	void WorldTime::setTimeSpeed(double _timeSpeed)
	{
		mTimeSpeed = _timeSpeed;
	}
	//----------------------------------------------------------------------
	void WorldTime::_initListener(WorldTimeListener* _listener)
	{
		_listener->mElementHandle = mListeners.getNullHandle();
	}
	//----------------------------------------------------------------------
	void WorldTime::_addListener(WorldTimeListener* _listener, const Time& _firstActivation, const Time& _period)
	{
		_removeListener(_listener);
		_listener->mPeriod = _period;
		_listener->mElementHandle = mListeners.add(_listener, _firstActivation);
	}
	//----------------------------------------------------------------------
	void WorldTime::_removeListener(WorldTimeListener* _listener)
	{
		if(_listener->mElementHandle != mListeners.getNullHandle())
		{
			mListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mListeners.getNullHandle();
		}
	}
	//----------------------------------------------------------------------
	void WorldTime::frameStarted(const FrameEvent& _evt)
	{
		// Advance the current world time.
		double t = _evt.timeSinceLastFrame;
		if(!mPaused && mTimeSpeed != 0)
		{
			setCurrentTime(getCurrentTime() + t * mTimeSpeed);
		}
	}
	//----------------------------------------------------------------------
	void WorldTime::fireWorldTimeCame()
	{
		while(true)
		{
			// Process the first listener - the listener 
			// with earliest activation time.
			Listeners::Iterator it = mListeners.getIterator();
			if(!it.hasMoreElements())
				break; // no listeners to process

			Time activationTime = it.peekNextKey();
			if(activationTime > mCurrentTime)
				break; // all required listeners have been processed

			WorldTimeListener* listener = it.peekNextValue();
			Time period = listener->mPeriod;
			Time nextActivationTime;
			if(period != Time::INFINITY)
				nextActivationTime = activationTime + period;
			else
				nextActivationTime = Time::INFINITY; // next activation time = never

			// Call the listener.
			WorldTimeEvent evt(activationTime, nextActivationTime);
			listener->worldTimeCame(evt);
			nextActivationTime = evt.getNextActivationTime();

			// The worldTriggered function could remove the listener, 
			// so we check if the iterator points to the same listener yet.
			if(!it.hasMoreElements() || it.peekNextValue() != listener)
				continue; // process next listener

			// Remove the listener from the ordered list,
			// increase the activation time by period,
			// and insert the listener to the ordered list again.
			_removeListener(listener);
			if(nextActivationTime != Time::INFINITY)
			{
				_addListener(listener, nextActivationTime, period);
			}
		}
	}

} // namespace GothOgre