#ifndef GOTHOGRE_WORLD_TIME_LISTENER_H
#define GOTHOGRE_WORLD_TIME_LISTENER_H

#include "GothOgre_SafeOrderedList.h"
#include "GothOgre_Time.h"


namespace GothOgre
{
	class WorldTimeEvent
	{
	public:
		WorldTimeEvent(const Time& _activationTime, const Time& _nextActivationTime) 
			: mActivationTime(_activationTime), mNextActivationTime(_nextActivationTime) {}

		const Time& getActivationTime() const {return mActivationTime;}
		const Time& getNextActivationTime() const {return mNextActivationTime;}
		void setNextActivationTime(const Time& _time) {mNextActivationTime = _time;}

	private:
		Time  mActivationTime;
		Time  mNextActivationTime;
	};


	//--------------------------------------------------------------------------
	/** Internal base class for listener. */
	class GOTHOGRE_EXPORT WorldTimeListener
	{
	public:
		/** Called when the required time came. */
		virtual void worldTimeCame(WorldTimeEvent& _evt) = 0;

	public:
		WorldTimeListener();
		virtual ~WorldTimeListener();

		/** Register this object as events' handler. */
		void registerListener(const Time& _firstActivation, const Time& _period = Time::INFINITY);

		/** Unregister this object as events' handler. */
		void unregisterListener();

	private:
		friend class WorldTime;
		typedef SafeOrderedList<Time, WorldTimeListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
		Time           mPeriod;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_WORLD_TIME_BASE(worldTimeCameFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc(const Time& _firstActivation, const Time& _period = Time::INFINITY) \
		{ \
			mPrivateWorldTimeListener_##registerFunc.registerListener(this, _firstActivation, _period); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateWorldTimeListener_##registerFunc.unregisterListener(); \
		} \
	\
	private: \
		void worldTimeCame_defaultImpl_##registerFunc(WorldTimeEvent& _evt) \
		{ \
		} \
		\
		class PrivateWorldTimeListener_##registerFunc : public WorldTimeListener \
		{ \
		public: \
			void worldTimeCame(WorldTimeEvent& _evt) \
			{ \
				return mOwner->worldTimeCameFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner, const Time& _firstActivation, const Time& _period) \
			{ \
				mOwner = _owner; \
				WorldTimeListener::registerListener(_firstActivation, _period); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateWorldTimeListener_##registerFunc; \
		PrivateWorldTimeListener_##registerFunc  mPrivateWorldTimeListener_##registerFunc;



	//----------------------------------------------------------------------------
	// WorldTimeListener.
	// Macroses to inject world time listening functionality to any class.
	//----------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_WORLD_TIME_CAME_EX( \
		worldTimeCameFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldTimeCameFunc(WorldTimeEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_TIME_BASE( \
			worldTimeCameFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_TIME_CAME( classname ) \
		GOTHOGRE_DECLARE_WORLD_TIME_CAME_EX( \
		worldTimeCame, \
		registerWorldTimeListener, unregisterWorldTimeListener, classname)

	//--------------------------------------------------------------------------
	/* Example of usage:
	class MyClass
	{
	public:
		MyClass();
		~MyClass()

	private:
		GOTHOGRE_DECLARE_WORLD_TIME_CAME( MyClass )
	};
	
	MyClass::MyClass()
	{
		registerWorldTimeListener(); // Start handling of events
	}

	MyClass::~MyClass()
	{
		// It's not necessary to call unregisterWorldTimeListener here
		// because it's called automatically.
	}

	void MyClass::worldTimeCame(WorldTimeEvent& _evt)
	{
		// time to go - event handling code
	}

	*/

}

#endif // GOTHOGRE_WORLD_TIME_LISTENER_H