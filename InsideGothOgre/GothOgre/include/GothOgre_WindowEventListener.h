#ifndef GOTHOGRE_WINDOW_EVENT_LISTENER_H
#define GOTHOGRE_WINDOW_EVENT_LISTENER_H

#include "GothOgre_SafeList.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	/** Base class for window event listeners. Should not be used directly. 
	Use macroses GOTHOGRE_DECLARE_WINDOW_* instead. */
	class GOTHOGRE_EXPORT NewWindowEventListener
	{
	public:
		/** Window has moved position. */
		virtual void windowMoved() = 0;
		
		/** Window has resized. */
		virtual void windowResized() = 0;
		
		/** Window is closing (Only triggered if user pressed the [X] button). */
		virtual bool windowClosing() = 0;
		
		/** Window has been closed (Only triggered if user pressed the [X] button). */
		virtual void windowClosed() = 0;
		
		/** Window has lost/gained focus. */
		virtual void windowFocusChanged() = 0;
		
		/** User transfers data to this application
		by dragging and dropping the file with the mouse. */
		virtual void windowFileDropped(const String& _filename) = 0;

	public:
		NewWindowEventListener();
		~NewWindowEventListener();

		/** Register this object as events' handler. */
		void registerListener();

		/** Unregister this object as events' handler. */
		void unregisterListener();

	private:
		friend class InputSystem;
		typedef SafeList<NewWindowEventListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_WINDOW_BASE(windowMovedFunc, windowResizedFunc, windowClosingFunc, windowClosedFunc, windowFocusChangedFunc, windowFileDroppedFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc() \
		{ \
			mPrivateWindowEventListener_##registerFunc.registerListener(this); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateWindowEventListener_##registerFunc.unregisterListener(); \
		} \
	\
	private: \
		void windowMoved_defaultImpl_##registerFunc() \
		{ \
		} \
		void windowResized_defaultImpl_##registerFunc() \
		{ \
		} \
		bool windowClosing_defaultImpl_##registerFunc() \
		{ \
			return true; \
		} \
		void windowClosed_defaultImpl_##registerFunc() \
		{ \
		} \
		void windowFocusChanged_defaultImpl_##registerFunc() \
		{ \
		} \
		void windowFileDropped_defaultImpl_##registerFunc(const String& _filename) \
		{ \
		} \
		\
		class PrivateWindowEventListener_##registerFunc : public NewWindowEventListener \
		{ \
		public: \
			void windowMoved() \
			{ \
				mOwner->windowMovedFunc(); \
			} \
			void windowResized() \
			{ \
				mOwner->windowResizedFunc(); \
			} \
			bool windowClosing() \
			{ \
				return mOwner->windowClosingFunc(); \
			} \
			void windowClosed() \
			{ \
				mOwner->windowClosedFunc(); \
			} \
			void windowFocusChanged() \
			{ \
				return mOwner->windowFocusChangedFunc(); \
			} \
			void windowFileDropped(const String& _filename) \
			{ \
				mOwner->windowFileDroppedFunc(_filename); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				NewWindowEventListener::registerListener(); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateWindowEventListener_##registerFunc; \
		PrivateWindowEventListener_##registerFunc mPrivateWindowEventListener_##registerFunc;


	//----------------------------------------------------------------------------
	// WindowEventListener.
	// Macroses to inject window event-listening functionality to any class.
	//----------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_WINDOW_MOVED_EX( \
		windowMovedFunc, \
		registerFunc, unregisterFunc, classname) \
		void windowMovedFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMovedFunc, \
		windowResized_defaultImpl_##registerFunc, \
		windowClosing_defaultImpl_##registerFunc, \
		windowClosed_defaultImpl_##registerFunc, \
		windowFocusChanged_defaultImpl_##registerFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_RESIZED_EX( \
		windowResizedFunc, \
		registerFunc, unregisterFunc, classname) \
		void windowResizedFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMoved_defaultImpl_##registerFunc, \
		windowResizedFunc, \
		windowClosing_defaultImpl_##registerFunc, \
		windowClosed_defaultImpl_##registerFunc, \
		windowFocusChanged_defaultImpl_##registerFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSING_EX( \
		windowClosingFunc, \
		registerFunc, unregisterFunc, classname) \
		bool windowClosingFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMoved_defaultImpl_##registerFunc, \
		windowResized_defaultImpl_##registerFunc, \
		windowClosingFunc, \
		windowClosed_defaultImpl_##registerFunc, \
		windowFocusChanged_defaultImpl_##registerFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSED_EX( \
		windowClosedFunc, \
		registerFunc, unregisterFunc, classname) \
		void windowClosedFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMoved_defaultImpl_##registerFunc, \
		windowResized_defaultImpl_##registerFunc, \
		windowClosing_defaultImpl_##registerFunc, \
		windowClosedFunc, \
		windowFocusChanged_defaultImpl_##registerFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_FOCUS_CHANGED_EX( \
		windowFocusChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void windowFocusChangedFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMoved_defaultImpl_##registerFunc, \
		windowResized_defaultImpl_##registerFunc, \
		windowClosing_defaultImpl_##registerFunc, \
		windowClosed_defaultImpl_##registerFunc, \
		windowFocusChangedFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_FILE_DROPPED_EX( \
		windowFileDroppedFunc, \
		registerFunc, unregisterFunc, classname) \
		void windowFileDroppedFunc(const String& _filename); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMoved_defaultImpl_##registerFunc, \
		windowResized_defaultImpl_##registerFunc, \
		windowClosing_defaultImpl_##registerFunc, \
		windowClosed_defaultImpl_##registerFunc, \
		windowFocusChanged_defaultImpl_##registerFunc, \
		windowFileDroppedFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_MOVED_RESIZED_EX( \
		windowMovedFunc, windowResizedFunc, \
		registerFunc, unregisterFunc, classname) \
		void windowMovedFunc(); \
		void windowResizedFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMovedFunc, \
		windowResizedFunc, \
		windowClosing_defaultImpl_##registerFunc, \
		windowClosed_defaultImpl_##registerFunc, \
		windowFocusChanged_defaultImpl_##registerFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSING_CLOSED_EX( \
		windowClosingFunc, windowClosedFunc, \
		registerFunc, unregisterFunc, classname) \
		bool windowClosingFunc(); \
		void windowClosedFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMoved_defaultImpl_##registerFunc, \
		windowResized_defaultImpl_##registerFunc, \
		windowClosingFunc, \
		windowClosedFunc, \
		windowFocusChanged_defaultImpl_##registerFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSED_FOCUS_CHANGED_EX( \
		windowClosedFunc, windowFocusChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void windowClosedFunc(); \
		void windowFocusChangedFunc(); \
		GOTHOGRE_DECLARE_WINDOW_BASE( \
		windowMoved_defaultImpl_##registerFunc, \
		windowResized_defaultImpl_##registerFunc, \
		windowClosing_defaultImpl_##registerFunc, \
		windowClosedFunc, \
		windowFocusChangedFunc, \
		windowFileDropped_defaultImpl_##registerFunc, \
		registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WINDOW_MOVED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_MOVED_EX( \
			windowMoved, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_RESIZED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_RESIZED_EX( \
			windowResized, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSING( classname ) \
		GOTHOGRE_DECLARE_WINDOW_CLOSING_EX( \
			windowClosing, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_CLOSED_EX( \
			windowClosed, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_FOCUS_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_FOCUS_CHANGED_EX( \
			windowFocusChanged, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_FILE_DROPPED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_FILE_DROPPED_EX( \
			windowFileDropped, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_MOVED_RESIZED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_MOVED_RESIZED_EX( \
			windowMoved, windowResized, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSING_CLOSED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_CLOSING_CLOSED_EX( \
			windowClosing, windowClosed, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

#	define GOTHOGRE_DECLARE_WINDOW_CLOSED_FOCUS_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WINDOW_CLOSED_FOCUS_CHANGED_EX( \
			windowClosed, windowFocusChanged, \
			registerWindowEventListener, unregisterWindowEventListener, \
			classname)

}

#endif // GOTHOGRE_WINDOW_EVENT_LISTENER_H