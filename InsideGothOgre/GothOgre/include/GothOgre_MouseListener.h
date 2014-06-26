#ifndef GOTHOGRE_MOUSE_LISTENER_H
#define GOTHOGRE_MOUSE_LISTENER_H

#include "GothOgre_MouseButton.h"
#include "GothOgre_InputEventResult.h"
#include "GothOgre_SafeOrderedList.h"
#include "GothOgre_Priority.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------------
	/** Mouse event. */
	class MouseEvent
	{
	public:
		MouseEvent( int _absX, int _absY, int _absZ, MouseButton _mb = MouseButton::NONE) : mAbsX(_absX), mAbsY(_absY), mAbsZ(_absZ), mMouseButton(_mb) {}
		~MouseEvent() {}

		MouseButton getMouseButton() const {return mMouseButton;}
		int getAbsX() const {return mAbsX;}
		int getAbsY() const {return mAbsY;}
		int getAbsZ() const {return mAbsZ;}

	private:
		//! MouseButton of event
		MouseButton mMouseButton;

		int mAbsX;
		int mAbsY;
		int mAbsZ;
	};


	//--------------------------------------------------------------------------
	/** Base class for mouse listeners. Should not be used directly. 
	Use macroses GOTHOGRE_DECLARE_MOUSE_* instead. */
	class GOTHOGRE_EXPORT MouseListener
	{
	public:
		virtual MouseEventResult mousePressed( const MouseEvent& _evt ) = 0;
		virtual MouseEventResult mouseReleased( const MouseEvent& _evt ) = 0;
		virtual MouseEventResult mouseMoved( const MouseEvent& _evt ) = 0;
		virtual MouseEventResult mouseLeft( const MouseEvent& _evt ) = 0;

	public:
		MouseListener();
		virtual ~MouseListener();

		/** Register this object as events' handler. */
		void registerListener(Priority _priority = Priority::LOWEST);

		/** Unregister this object as events' handler. */
		void unregisterListener();

	private:
		friend class InputSystem;
		typedef SafeOrderedList<Priority, MouseListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_MOUSE_BASE(mousePressedFunc, mouseReleasedFunc, mouseMovedFunc, mouseLeftFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc(Priority _priority = Priority::LOWEST) \
		{ \
			mPrivateMouseListener_##registerFunc.registerListener(this, _priority); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateMouseListener_##registerFunc.unregisterListener(); \
		} \
		\
	private: \
		MouseEventResult mousePressed_defaultImpl_##registerFunc(const MouseEvent& _evt) \
		{ \
			return MouseEventResult::NOT_PROCESSED; \
		} \
		MouseEventResult mouseReleased_defaultImpl_##registerFunc(const MouseEvent& _evt) \
		{ \
			return MouseEventResult::NOT_PROCESSED; \
		} \
		MouseEventResult mouseMoved_defaultImpl_##registerFunc(const MouseEvent& _evt) \
		{ \
			return MouseEventResult::NOT_PROCESSED; \
		} \
		MouseEventResult mouseLeft_defaultImpl_##registerFunc(const MouseEvent& _evt) \
		{ \
			return MouseEventResult::NOT_PROCESSED; \
		} \
		\
		class PrivateMouseListener_##registerFunc : public MouseListener \
		{ \
		public: \
			MouseEventResult mousePressed(const MouseEvent& _evt) \
			{ \
				return mOwner->mousePressedFunc(_evt); \
			} \
			MouseEventResult mouseReleased(const MouseEvent& _evt) \
			{ \
				return mOwner->mouseReleasedFunc(_evt); \
			} \
			MouseEventResult mouseMoved(const MouseEvent& _evt) \
			{ \
				return mOwner->mouseMovedFunc(_evt); \
			} \
			MouseEventResult mouseLeft(const MouseEvent& _evt) \
			{ \
				return mOwner->mouseLeftFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner, Priority _priority) \
			{ \
				mOwner = _owner; \
				MouseListener::registerListener(_priority); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateMouseListener_##registerFunc; \
		PrivateMouseListener_##registerFunc mPrivateMouseListener_##registerFunc;



	//----------------------------------------------------------------------------
	// MouseListener.
	// Macroses to inject mouse-listening functionality to any class.
	//----------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_MOUSE_PRESSED_EX( \
		mousePressedFunc, \
		registerFunc, unregisterFunc, classname) \
		MouseEventResult mousePressedFunc(const MouseEvent& _evt); \
		GOTHOGRE_DECLARE_MOUSE_BASE( \
			mousePressedFunc, \
			mouseReleased_defaultImpl_##registerFunc, \
			mouseMoved_defaultImpl_##registerFunc, \
			mouseLeft_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_MOUSE_RELEASED_EX( \
		mouseReleasedFunc, \
		registerFunc, unregisterFunc, classname) \
		MouseEventResult mouseReleasedFunc(const MouseEvent& _evt); \
		GOTHOGRE_DECLARE_MOUSE_BASE( \
			mousePressed_defaultImpl_##registerFunc, \
			mouseReleasedFunc, \
			mouseMoved_defaultImpl_##registerFunc, \
			mouseLeft_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_MOUSE_MOVED_EX( \
		mouseMovedFunc, \
		registerFunc, unregisterFunc, classname) \
		MouseEventResult mouseMovedFunc(const MouseEvent& _evt); \
		GOTHOGRE_DECLARE_MOUSE_BASE( \
			mousePressed_defaultImpl_##registerFunc, \
			mouseReleased_defaultImpl_##registerFunc, \
			mouseMovedFunc, \
			mouseLeft_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_EX( \
		mousePressedFunc, mouseReleasedFunc, \
		registerFunc, unregisterFunc, classname) \
		MouseEventResult mousePressedFunc(const MouseEvent& _evt); \
		MouseEventResult mouseReleasedFunc(const MouseEvent& _evt); \
		GOTHOGRE_DECLARE_MOUSE_BASE( \
			mousePressedFunc, \
			mouseReleasedFunc, \
			mouseMoved_defaultImpl_##registerFunc, \
			mouseLeft_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_MOUSE_MOVED_LEFT_EX( \
		mouseMovedFunc, mouseLeftFunc, \
		registerFunc, unregisterFunc, classname) \
		MouseEventResult mouseMovedFunc(const MouseEvent& _evt); \
		MouseEventResult mouseLeftFunc(const MouseEvent& _evt); \
		GOTHOGRE_DECLARE_MOUSE_BASE( \
			mousePressed_defaultImpl_##registerFunc, \
			mouseReleased_defaultImpl_##registerFunc, \
			mouseMovedFunc, \
			mouseLeftFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_MOVED_EX( \
		mousePressedFunc, mouseReleasedFunc, mouseMovedFunc, \
		registerFunc, unregisterFunc, classname) \
		MouseEventResult mousePressedFunc(const MouseEvent& _evt); \
		MouseEventResult mouseReleasedFunc(const MouseEvent& _evt); \
		MouseEventResult mouseMovedFunc(const MouseEvent& _evt); \
		GOTHOGRE_DECLARE_MOUSE_BASE( \
			mousePressedFunc, \
			mouseReleasedFunc, \
			mouseMovedFunc, \
			mouseLeft_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_MOVED_LEFT_EX( \
		mousePressedFunc, mouseReleasedFunc, mouseMovedFunc, mouseLeftFunc, \
		registerFunc, unregisterFunc, classname) \
		MouseEventResult mousePressedFunc(const MouseEvent& _evt); \
		MouseEventResult mouseReleasedFunc(const MouseEvent& _evt); \
		MouseEventResult mouseMovedFunc(const MouseEvent& _evt); \
		MouseEventResult mouseLeftFunc(const MouseEvent& _evt); \
		GOTHOGRE_DECLARE_MOUSE_BASE( \
			mousePressedFunc, \
			mouseReleasedFunc, \
			mouseMovedFunc, \
			mouseLeftFunc, \
			registerFunc, unregisterFunc, classname)


#	define GOTHOGRE_DECLARE_MOUSE_PRESSED( classname ) \
		GOTHOGRE_DECLARE_MOUSE_PRESSED_EX( \
			mousePressed, \
			registerMouseListener, unregisterMouseListener, classname)

#	define GOTHOGRE_DECLARE_MOUSE_RELEASED( classname ) \
		GOTHOGRE_DECLARE_MOUSE_RELEASED_EX( \
			mouseReleased, \
			registerMouseListener, unregisterMouseListener, classname)

#	define GOTHOGRE_DECLARE_MOUSE_MOVED( classname ) \
		GOTHOGRE_DECLARE_MOUSE_MOVED_EX( \
			mouseMoved, \
			registerMouseListener, unregisterMouseListener, classname)

#	define GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED( classname ) \
		GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_EX( \
			mousePressed, mouseReleased, \
			registerMouseListener, unregisterMouseListener, classname)

#	define GOTHOGRE_DECLARE_MOUSE_MOVED_LEFT( classname ) \
		GOTHOGRE_DECLARE_MOUSE_MOVED_LEFT_EX( \
			mouseMoved, mouseLeft, \
			registerMouseListener, unregisterMouseListener, classname)

#	define GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_MOVED( classname ) \
		GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_MOVED_EX( \
			mousePressed, mouseReleased, mouseMoved, \
			registerMouseListener, unregisterMouseListener, classname)

#	define GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_MOVED_LEFT( classname ) \
		GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_MOVED_LEFT_EX( \
			mousePressed, mouseReleased, mouseMoved, mouseLeft, \
			registerMouseListener, unregisterMouseListener, classname)


	//------------------------------------------------------------------------
	// See GothOgre_KeyListener.h if you need an example of usage.

} // namespace GothOgre

#endif // GOTHOGRE_KEY_LISTENER_H