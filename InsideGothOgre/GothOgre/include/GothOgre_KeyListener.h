#ifndef GOTHOGRE_KEY_LISTENER_H
#define GOTHOGRE_KEY_LISTENER_H

#include "GothOgre_KeyCode.h"
#include "GothOgre_InputEventResult.h"
#include "GothOgre_SafeOrderedList.h"
#include "GothOgre_Priority.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------------
	/** Key event. */
	class KeyEvent
	{
	public:
		KeyEvent( KeyCode _kc, unsigned int _txt ) : mKey(_kc), mText(_txt) {}
		~KeyEvent() {}

		KeyCode getKeyCode() const {return mKey;}
		unsigned int getText() const {return mText;}

	private:
		//! KeyCode of event
		KeyCode mKey;

		//! Text character, depends on current TextTranslationMode
		unsigned int mText;
	};


	//--------------------------------------------------------------------------
	/** Base class for key listeners. Should not be used directly. 
	Use macroses GOTHOGRE_DECLARE_KEY_* instead. */
	class GOTHOGRE_EXPORT KeyListener
	{
	public:
		virtual KeyEventResult keyPressed( const KeyEvent& _evt ) {return KeyEventResult::NOT_PROCESSED;}
		virtual KeyEventResult keyReleased( const KeyEvent& _evt ) {return KeyEventResult::NOT_PROCESSED;}

	public:
		KeyListener();
		~KeyListener();

		/** Register this object as events' handler. */
		void registerListener(Priority _priority = Priority::LOWEST);

		/** Unregister this object as events' handler. */
		void unregisterListener();

	private:
		friend class InputSystem;
		typedef SafeOrderedList<Priority, KeyListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_KEY_BASE(keyPressedFunc, keyReleasedFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc(Priority _priority = Priority::LOWEST) \
		{ \
			mPrivateKeyListener_##registerFunc.registerListener(this, _priority); \
		} \
		\
		void unregisterFunc() \
		{ \
			mPrivateKeyListener_##registerFunc.unregisterListener(); \
		} \
	private: \
		KeyEventResult keyPressed_defaultImpl_##registerFunc(const KeyEvent& _evt) \
		{ \
			return KeyEventResult::NOT_PROCESSED; \
		} \
		KeyEventResult keyReleased_defaultImpl_##registerFunc(const KeyEvent& _evt) \
		{ \
			return KeyEventResult::NOT_PROCESSED; \
		} \
		\
		class PrivateKeyListener_##registerFunc : public KeyListener \
		{ \
		public: \
			KeyEventResult keyPressed(const KeyEvent& _evt) \
			{ \
				return mOwner->keyPressedFunc(_evt); \
			} \
			\
			KeyEventResult keyReleased(const KeyEvent& _evt) \
			{ \
				return mOwner->keyReleasedFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner, Priority _priority) \
			{ \
				mOwner = _owner; \
				KeyListener::registerListener(_priority); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateKeyListener_##registerFunc; \
		PrivateKeyListener_##registerFunc mPrivateKeyListener_##registerFunc;


	//----------------------------------------------------------------------------
	// KeyListener.
	// Macroses to inject key-listening functionality to any class.
	//----------------------------------------------------------------------------

#	define GOTHOGRE_DECLARE_KEY_PRESSED_EX( \
		keyPressedFunc, \
		registerFunc, unregisterFunc, classname) \
		KeyEventResult keyPressedFunc(const KeyEvent& _evt); \
		GOTHOGRE_DECLARE_KEY_BASE( \
			keyPressedFunc, \
			keyReleased_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_KEY_RELEASED_EX( \
		keyReleasedFunc, \
		registerFunc, unregisterFunc, classname) \
		KeyEventResult keyReleasedFunc(const KeyEvent& _evt); \
		GOTHOGRE_DECLARE_KEY_BASE( \
			keyPressed_defaultImpl_##registerFunc, \
			keyReleasedFunc, \
				registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED_EX( \
		keyPressedFunc, keyReleasedFunc, \
		registerFunc, unregisterFunc, classname) \
		KeyEventResult keyPressedFunc(const KeyEvent& _evt); \
		KeyEventResult keyReleasedFunc(const KeyEvent& _evt); \
		GOTHOGRE_DECLARE_KEY_BASE( \
			keyPressedFunc, \
			keyReleasedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_KEY_PRESSED( classname ) \
		GOTHOGRE_DECLARE_KEY_PRESSED_EX( \
			keyPressed, \
			registerKeyListener, unregisterKeyListener, classname)

#	define GOTHOGRE_DECLARE_KEY_RELEASED( classname ) \
		GOTHOGRE_DECLARE_KEY_RELEASED_EX( \
			keyReleased, \
			registerKeyListener, unregisterKeyListener, classname)

#	define GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED( classname ) \
		GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED_EX( \
			keyPressed, keyReleased, \
			registerKeyListener, unregisterKeyListener, classname)

	
	//------------------------------------------------------------------------
	/* Example of usage:
	class MyClass
	{
	public:
		MyClass();
		~MyClass()

	private:
		GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED( MyClass )
	};
	
	MyClass::MyClass()
	{
		registerKeyListener( Priority::NORMAL ); // Start handling of events
	}

	MyClass::~MyClass()
	{
		// It's not necessary to call unregisterKeyListener here
		// because it's called automatically.
	}

	KeyEventResult MyClass::keyPressed(const KeyEvent& _evt)
	{
		// event handling code
	}

	KeyEventResult MyClass::keyReleased(const KeyEvent& _evt)
	{
		// event handling code
	}

	*/

} // namespace GothOgre

#endif // GOTHOGRE_KEY_LISTENER_H