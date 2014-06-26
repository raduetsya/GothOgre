#include "GothOgre_Precompiled.h"
#include "GothOgre_InputSystem.h"
#include "GothOgre_InputSupport.h"
#include "GothOgre_MouseListener.h"
#include "GothOgre_KeyListener.h"
#include "GothOgre_WindowEventListener.h"
#include "GothOgre_FrameListener.h"
#include "GothOgre_KeyList.h"
#include "GothOgre_ViewportAbsCoord.h"


//-------------------------------------------------------------------------
GothOgre::InputSystem* 
	Ogre::Singleton<GothOgre::InputSystem>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	InputSystem::InputSystem()
	{
		mInitialised = false;
		mClosed = false;
		mMouseAbsX = 0;
		mMouseAbsY = 0;
		mMouseAbsZ = 0;
	}
	//-------------------------------------------------------------------------
	InputSystem::~InputSystem()
	{
	}
	//-------------------------------------------------------------------------
	void InputSystem::initialise(RenderWindow* _window)
	{
		if(!mInitialised)
		{
			InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
			isupport->_setRenderWindow(_window);
			isupport->initialise(_window);
			Ogre::Root::getSingleton().addFrameListener(this);
			Ogre::WindowEventUtilities::addWindowEventListener(getRenderWindow(), this);
			mInitialised = true;
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::shutdown()
	{
		if(mInitialised)
		{
			InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
			isupport->shutdown();
			Ogre::WindowEventUtilities::removeWindowEventListener(getRenderWindow(), this);
			Ogre::Root::getSingleton().removeFrameListener(this);
			mInitialised = false;
		}
	}
	//-------------------------------------------------------------------------
	RenderWindow* InputSystem::getRenderWindow() const
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		return isupport->getRenderWindow();
	}
	//-------------------------------------------------------------------------
	int InputSystem::getWidth() const
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		return isupport->getWidth();
	}
	//-------------------------------------------------------------------------
	int InputSystem::getHeight() const
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		return isupport->getHeight();
	}
	//-------------------------------------------------------------------------
	void InputSystem::clipCursor(int _left, int _top, int _width, int _height)
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		isupport->clipCursor(_left, _top, _width, _height);
	}
	//-------------------------------------------------------------------------
	void InputSystem::clipCursor(const ViewportAbsCoord& _vpAbsCoord)
	{
		clipCursor(_vpAbsCoord.getLeft(), _vpAbsCoord.getTop(),
			_vpAbsCoord.getWidth(), _vpAbsCoord.getHeight());
	}
	//-------------------------------------------------------------------------
	void InputSystem::freeCursor()
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		isupport->freeCursor();
	}
	//-------------------------------------------------------------------------
	void InputSystem::setCursorPos(int _x, int _y, bool _sendMouseMovedEvent)
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		isupport->setCursorPos(_x, _y);
		if(_sendMouseMovedEvent)
		{
			_injectMouseMove( MouseEvent(_x, _y, mMouseAbsZ) );
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::getCursorPos(int* _px, int* _py) const
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		isupport->getCursorPos(_px, _py);
	}
	//-------------------------------------------------------------------------
	bool InputSystem::isKeyDown(const KeyCode& _keyCode) const
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		return isupport->isKeyDown(_keyCode);
	}
	//-------------------------------------------------------------------------
	bool InputSystem::isMouseDown(const MouseButton& _mouseButton) const
	{
		InputSupport* isupport = InputSupportRegistration::getSingleton().getSupport();
		return isupport->isMouseDown(_mouseButton);
	}
	//-------------------------------------------------------------------------
	bool InputSystem::isKeyDown(const KeyComb& _keyComb) const
	{
		for(size_t m = 0; m != _keyComb.getNumMouseButtons(); ++m)
		{
			if(!isMouseDown(_keyComb.getMouseButton(m)))
				return false;
		}
		for(size_t k = 0; k != _keyComb.getNumKeyCodes(); ++k)
		{
			if(!isKeyDown(_keyComb.getKeyCode(k)))
				return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------
	bool InputSystem::isKeyDown(const KeyList& _keyList) const
	{
		for(size_t i = 0; i != _keyList.size(); ++i)
		{
			if(isKeyDown(_keyList[i]))
				return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------
	void InputSystem::setExitFlag()
	{
		if(windowClosing(getRenderWindow()))
		{
			windowClosed(getRenderWindow());
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_injectKeyPress(const KeyEvent& _evt)
	{
		KeyListeners::ReverseIterator it = mKeyListeners.getReverseIterator();
		while(it.hasMoreElements())
		{
			KeyListener* keyListener = it.getNext();
			if(keyListener->keyPressed(_evt) == KeyEventResult::PROCESSED)
				break;
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_injectKeyRelease(const KeyEvent& _evt)
	{
		KeyListeners::ReverseIterator it = mKeyListeners.getReverseIterator();
		while(it.hasMoreElements())
		{
			KeyListener* keyListener = it.getNext();
			if(keyListener->keyReleased(_evt) == KeyEventResult::PROCESSED)
				break;
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_injectMousePress(const MouseEvent& _evt)
	{
		MouseListeners::ReverseIterator it = mMouseListeners.getReverseIterator();
		while(it.hasMoreElements())
		{
			MouseListener* mouseListener = it.getNext();
			if(mouseListener->mousePressed(_evt) == MouseEventResult::PROCESSED)
				break;
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_injectMouseRelease(const MouseEvent& _evt)
	{
		MouseListeners::ReverseIterator it = mMouseListeners.getReverseIterator();
		while(it.hasMoreElements())
		{
			MouseListener* mouseListener = it.getNext();
			if(mouseListener->mouseReleased(_evt) == MouseEventResult::PROCESSED)
				break;
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_injectMouseMove(const MouseEvent& _evt)
	{
		mMouseAbsX = _evt.getAbsX();
		mMouseAbsY = _evt.getAbsY();
		mMouseAbsZ = _evt.getAbsZ();

		MouseListeners::ReverseIterator it = mMouseListeners.getReverseIterator();
		while(it.hasMoreElements())
		{
			MouseListener* mouseListener = it.getNext();
			if(mouseListener->mouseMoved(_evt) == MouseEventResult::PROCESSED)
				break;
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_injectMouseLeave(const MouseEvent& _evt)
	{
		mMouseAbsX = _evt.getAbsX();
		mMouseAbsY = _evt.getAbsY();
		mMouseAbsZ = _evt.getAbsZ();

		MouseListeners::ReverseIterator it = mMouseListeners.getReverseIterator();
		while(it.hasMoreElements())
		{
			MouseListener* mouseListener = it.getNext();
			if(mouseListener->mouseLeft(_evt) == MouseEventResult::PROCESSED)
				break;
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_injectFileDrop(const String& _filename)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowFileDropped(_filename);
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_initListener(KeyListener* _listener)
	{
		_listener->mElementHandle = mKeyListeners.getNullHandle();
	}
	//-------------------------------------------------------------------------
	void InputSystem::_addListener(KeyListener* _listener, Priority _priority)
	{
		_removeListener(_listener);
		_listener->mElementHandle = mKeyListeners.add(_listener, _priority);
	}
	//-------------------------------------------------------------------------
	void InputSystem::_removeListener(KeyListener* _listener)
	{
		if(_listener->mElementHandle != mKeyListeners.getNullHandle())
		{
			mKeyListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mKeyListeners.getNullHandle();
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_initListener(MouseListener* _listener)
	{
		_listener->mElementHandle = mMouseListeners.getNullHandle();
	}
	//-------------------------------------------------------------------------
	void InputSystem::_addListener(MouseListener* _listener, Priority _priority)
	{
		_removeListener(_listener);
		_listener->mElementHandle = mMouseListeners.add(_listener, _priority);
	}
	//-------------------------------------------------------------------------
	void InputSystem::_removeListener(MouseListener* _listener)
	{
		if(_listener->mElementHandle != mMouseListeners.getNullHandle())
		{
			mMouseListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mMouseListeners.getNullHandle();
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_initListener(NewWindowEventListener* _listener)
	{
		_listener->mElementHandle = mWindowEventListeners.getNullHandle();
	}
	//-------------------------------------------------------------------------
	void InputSystem::_addListener(NewWindowEventListener* _listener)
	{
		if(_listener->mElementHandle == mWindowEventListeners.getNullHandle())
		{
			_listener->mElementHandle = mWindowEventListeners.add(_listener);
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_removeListener(NewWindowEventListener* _listener)
	{
		if(_listener->mElementHandle != mWindowEventListeners.getNullHandle())
		{
			mWindowEventListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mWindowEventListeners.getNullHandle();
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_initListener(NewFrameListener* _listener)
	{
		_listener->mElementHandle = mFrameListeners.getNullHandle();
	}
	//-------------------------------------------------------------------------
	void InputSystem::_addListener(NewFrameListener* _listener)
	{
		if(_listener->mElementHandle == mFrameListeners.getNullHandle())
		{
			_listener->mElementHandle = mFrameListeners.add(_listener);
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::_removeListener(NewFrameListener* _listener)
	{
		if(_listener->mElementHandle != mFrameListeners.getNullHandle())
		{
			mFrameListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mFrameListeners.getNullHandle();
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::windowMoved(RenderWindow* rw)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowMoved();
		}
	}
	//-------------------------------------------------------------------------
	void InputSystem::windowResized(RenderWindow* rw)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowResized();
		}
	}
	//-------------------------------------------------------------------------
	bool InputSystem::windowClosing(RenderWindow* rw)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewWindowEventListener* windowEventListener = it.getNext();
			if(!windowEventListener->windowClosing())
				return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------
	void InputSystem::windowClosed(RenderWindow* rw)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowClosed();
		}
		mClosed = true;
	}
	//-------------------------------------------------------------------------
	void InputSystem::windowFocusChange(RenderWindow* rw)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowFocusChanged();
		}
	}
	//-------------------------------------------------------------------------
	bool InputSystem::frameStarted( const Ogre::FrameEvent& _evt )
	{
		if(mClosed)
			return false;

		OgreProfile("FrameListener::frameStarted");

		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewFrameListener* frameListener = it.getNext();
			frameListener->frameStarted(_evt);
		}
		return true;
	}
	//-------------------------------------------------------------------------
	bool InputSystem::frameRenderingQueued(const Ogre::FrameEvent& _evt)
	{
		OgreProfile("FrameListener::frameRenderingQueued");

		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewFrameListener* frameListener = it.getNext();
			frameListener->frameRenderingQueued(_evt);
		}
		return true;
	}
	//-------------------------------------------------------------------------
	bool InputSystem::frameEnded(const Ogre::FrameEvent& _evt)
	{
		OgreProfile("FrameListener::frameEnded");

		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
		{
			NewFrameListener* frameListener = it.getNext();
			frameListener->frameEnded(_evt);
		}
		return true;
	}

}