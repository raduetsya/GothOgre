#ifndef GOTHOGRE_FRAME_LISTENER_H
#define GOTHOGRE_FRAME_LISTENER_H

#include "GothOgre_SafeList.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------
	// FrameListener.
	// Internal implementation.
	//----------------------------------------------------------------------------

	/** Base class for frame listeners. Should not be used directly. 
	Use macroses GOTHOGRE_DECLARE_FRAME_* instead. */
	class GOTHOGRE_EXPORT NewFrameListener
	{
	protected:
		/** Called when a frame is about to begin rendering. */
		virtual void frameStarted(const FrameEvent& _evt) = 0;

		/** Called just after a frame has been rendered. */
		virtual void frameEnded(const FrameEvent& _evt) = 0;

		/** Called after all render targets have had their rendering commands issued, 
		but before render windows have been asked to flip their buffers over. */
		virtual void frameRenderingQueued(const FrameEvent& _evt) = 0;

	public:
		/** Register this object as frame listener. 
		Must be called explicitly, because the constructor doesn't call the function. */
		void registerListener();

		/** Unregister this object as frame listener. 
		The destructor calls this function automatically. */
		void unregisterListener();

		NewFrameListener();
		~NewFrameListener();

	private:
		friend class InputSystem;
		typedef SafeList<NewFrameListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};

	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_FRAME_BASE(frameStartedFunc, frameEndedFunc, frameRenderingQueuedFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc() \
		{ \
			mPrivateFrameListener_##registerFunc.registerListener(this); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateFrameListener_##registerFunc.unregisterListener(); \
		} \
	\
	private: \
		void frameStarted_defaultImpl_##registerFunc(const FrameEvent& _evt) \
		{ \
		} \
		void frameEnded_defaultImpl_##registerFunc(const FrameEvent& _evt) \
		{ \
		} \
		void frameRenderingQueued_defaultImpl_##registerFunc(const FrameEvent& _evt) \
		{ \
		} \
		\
		class PrivateFrameListener_##registerFunc : public NewFrameListener \
		{ \
		public: \
			void frameStarted(const FrameEvent& _evt) \
			{ \
				mOwner->frameStartedFunc(_evt); \
			} \
			\
			void frameEnded(const FrameEvent& _evt) \
			{ \
				mOwner->frameEndedFunc(_evt); \
			} \
			\
			void frameRenderingQueued(const FrameEvent& _evt) \
			{ \
				mOwner->frameRenderingQueuedFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				NewFrameListener::registerListener(); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateFrameListener_##registerFunc; \
		PrivateFrameListener_##registerFunc mPrivateFrameListener_##registerFunc;




	//----------------------------------------------------------------------------
	// FrameListener.
	// Macroses to inject frame-listening functionality to any class.
	//----------------------------------------------------------------------------

#	define GOTHOGRE_DECLARE_FRAME_STARTED_EX( \
		frameStartedFunc, \
		registerFunc, unregisterFunc, classname) \
		void frameStartedFunc(const FrameEvent& _evt); \
		GOTHOGRE_DECLARE_FRAME_BASE( \
			frameStartedFunc, \
			frameEnded_defaultImpl_##registerFunc, \
			frameRenderingQueued_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_FRAME_ENDED_EX( \
		frameEndedFunc, \
		registerFunc, unregisterFunc, classname) \
		void frameEndedFunc(const FrameEvent& _evt); \
		GOTHOGRE_DECLARE_FRAME_BASE( \
			frameStarted_defaultImpl_##registerFunc, \
			frameEndedFunc, \
			frameRenderingQueued_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_FRAME_RENDERING_QUEUED_EX( \
		frameRenderingQueuedFunc, \
		registerFunc, unregisterFunc, classname) \
		void frameRenderingQueuedFunc(const FrameEvent& _evt); \
		GOTHOGRE_DECLARE_FRAME_BASE( \
			frameStarted_defaultImpl_##registerFunc, \
			frameEnded_defaultImpl_##registerFunc, \
			frameRenderingQueuedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_FRAME_STARTED_ENDED_EX( \
		frameStartedFunc, frameEndedFunc, \
		registerFunc, unregisterFunc, classname) \
		void frameStartedFunc(const FrameEvent& _evt); \
		void frameEndedFunc(const FrameEvent& _evt); \
		GOTHOGRE_DECLARE_FRAME_BASE( \
			frameStartedFunc, \
			frameEndedFunc, \
			frameRenderingQueued_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_FRAME_STARTED_ENDED_RENDERING_QUEUED_EX( \
		frameStartedFunc, frameEndedFunc, frameRenderingQueued, \
		registerFunc, unregisterFunc, classname) \
		void frameStartedFunc(const FrameEvent& _evt); \
		void frameEndedFunc(const FrameEvent& _evt); \
		void frameRenderingQueuedFunc(const FrameEvent& _evt); \
		GOTHOGRE_DECLARE_FRAME_BASE( \
			frameStartedFunc, \
			frameEndedFunc, \
			frameRenderingQueued, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_FRAME_STARTED( classname ) \
		GOTHOGRE_DECLARE_FRAME_STARTED_EX( \
			frameStarted, \
			registerFrameListener, unregisterFrameListener, classname)

#	define GOTHOGRE_DECLARE_FRAME_ENDED( classname ) \
		GOTHOGRE_DECLARE_FRAME_ENDED_EX( \
			frameEnded, \
			registerFrameListener, unregisterFrameListener, classname)

#	define GOTHOGRE_DECLARE_FRAME_RENDERING_QUEUED( classname ) \
		GOTHOGRE_DECLARE_FRAME_RENDERING_QUEUED_EX( \
			frameRenderingQueued, \
			registerFrameListener, unregisterFrameListener, classname)

#	define GOTHOGRE_DECLARE_FRAME_STARTED_ENDED( classname ) \
		GOTHOGRE_DECLARE_FRAME_STARTED_ENDED_EX( \
			frameStarted, frameEnded, \
			registerFrameListener, unregisterFrameListener, classname)

#	define GOTHOGRE_DECLARE_FRAME_STARTED_ENDED_RENDERING_QUEUED( classname ) \
		GOTHOGRE_DECLARE_FRAME_STARTED_ENDED_RENDERING_QUEUED_EX( \
			frameStarted, frameEnded, frameRenderingQueued, \
			registerFrameListener, unregisterFrameListener, classname )

	//------------------------------------------------------------------------
	/* Example of usage:
	class MyClass
	{
	public:
		MyClass();
		~MyClass()

	private:
		GOTHOGRE_DECLARE_FRAME_STARTED( MyClass )
	};
	
	MyClass::MyClass()
	{
		registerFrameListener(); // Start handling of events
	}

	MyClass::~MyClass()
	{
		// It's not necessary to call unregisterFrameListener here
		// because it's called automatically.
	}

	void MyClass::frameStarted(const FrameEvent& _evt)
	{
		// event handling code
	}

	*/
}

#endif // GOTHOGRE_FRAME_LISTENER_H