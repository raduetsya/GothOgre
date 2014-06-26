#include "Precompiled.h"
#include "DefaultInputSystem.h"

namespace GothOgre
{
	DefaultInputSystem::DefaultInputSystem()
	{
		mWindow = nullptr;
		mExit = false;
		mOgreListener = false;
	}
	//------------------------------------------------------------------
	DefaultInputSystem::~DefaultInputSystem()
	{
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::initialise(RenderWindow* _window)
	{
		mExit = false;
		mWindow = _window;
		if(!mOgreListener)
		{
			Ogre::Root::getSingleton().addFrameListener(this);
			Ogre::WindowEventUtilities::addWindowEventListener(getRenderWindow(), this);
			mOgreListener = true;
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::shutdown()
	{
		if(mOgreListener)
		{
			Ogre::Root::getSingleton().removeFrameListener(this);
			Ogre::WindowEventUtilities::removeWindowEventListener(getRenderWindow(), this);
			mWindow = nullptr;
			mOgreListener = false;
		}
	}
	//------------------------------------------------------------------
	RenderWindow* DefaultInputSystem::getRenderWindow() const
	{
		return mWindow;
	}
	//-------------------------------------------------------------------------
	int DefaultInputSystem::getWidth() const 
	{
		RenderWindow* win = getRenderWindow();
		return win ? win->getWidth() : 0;
	}
	//-------------------------------------------------------------------------
	int DefaultInputSystem::getHeight() const 
	{
		RenderWindow* win = getRenderWindow();
		return win ? win->getHeight() : 0;
	}
	//-------------------------------------------------------------------------
	void DefaultInputSystem::clipCursorToViewport(Viewport* _viewport)
	{
		clipCursor(_viewport->getActualLeft(), _viewport->getActualTop(), 
			_viewport->getActualWidth(), _viewport->getActualHeight());
	}
	//-------------------------------------------------------------------------
	bool DefaultInputSystem::isKeyDown(const KeyComb& _keyComb) const
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
	bool DefaultInputSystem::isKeyDown(const KeyList& _keyList) const
	{
		for(size_t i = 0; i != _keyList.size(); ++i)
		{
			if(isKeyDown(_keyList[i]))
				return true;
		}
		return false;
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::addKeyListener(BaseKeyListener* _keyListener, int _priority)
	{
		mKeyListeners.add(_keyListener, _priority);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::removeKeyListener(BaseKeyListener* _keyListener)
	{
		mKeyListeners.remove(_keyListener);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::addMouseListener(BaseMouseListener* _mouseListener, int _priority)
	{
		mMouseListeners.add(_mouseListener, _priority);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::removeMouseListener(BaseMouseListener* _mouseListener)
	{
		mMouseListeners.remove(_mouseListener);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::addWindowEventListener(BaseWindowEventListener* _windowEventListener)
	{
		mWindowEventListeners.add(_windowEventListener);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::removeWindowEventListener(BaseWindowEventListener* _windowEventListener)
	{
		mWindowEventListeners.remove(_windowEventListener);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::addFrameListener(BaseFrameListener* _frameListener)
	{
		mFrameListeners.add(_frameListener);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::removeFrameListener(BaseFrameListener* _frameListener)
	{
		mFrameListeners.remove(_frameListener);
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::fireMouseMoved( const MouseEvent& _evt )
	{
		MouseListeners::Iterator it = mMouseListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseMouseListener* mouseListener = it.getNext();
			if(mouseListener->mouseMoved(_evt) == MouseEventResult::Processed)
				break;
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::fireMousePressed( const MouseEvent& _evt )
	{
		MouseListeners::Iterator it = mMouseListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseMouseListener* mouseListener = it.getNext();
			if(mouseListener->mousePressed(_evt) == MouseEventResult::Processed)
				break;
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::fireMouseReleased( const MouseEvent& _evt )
	{
		MouseListeners::Iterator it = mMouseListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseMouseListener* mouseListener = it.getNext();
			if(mouseListener->mouseReleased(_evt) == MouseEventResult::Processed)
				break;
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::fireKeyPressed( const KeyEvent& _evt )
	{
		KeyListeners::Iterator it = mKeyListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseKeyListener* keyListener = it.getNext();
			if(keyListener->keyPressed(_evt) == KeyEventResult::Processed)
				break;
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::fireKeyReleased( const KeyEvent& _evt )
	{
		KeyListeners::Iterator it = mKeyListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseKeyListener* keyListener = it.getNext();
			if(keyListener->keyReleased(_evt) == KeyEventResult::Processed)
				break;
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::fireFileDropped( const String& _filename )
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowFileDropped(_filename);
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::windowResized( RenderWindow* _window )
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowResized();
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::windowMoved(RenderWindow* _window)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowMoved();
		}
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::windowFocusChange( RenderWindow* _window )
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowFocusChange();
		}
	}
	//------------------------------------------------------------------
	bool DefaultInputSystem::windowClosing(RenderWindow* rw)
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseWindowEventListener* windowEventListener = it.getNext();
			if(!windowEventListener->windowClosing())
				return false;
		}
		return true;
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::windowClosed( RenderWindow* _window )
	{
		WindowEventListeners::Iterator it = mWindowEventListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseWindowEventListener* windowEventListener = it.getNext();
			windowEventListener->windowClosed();
		}
		sendExit();
	}
	//------------------------------------------------------------------
	void DefaultInputSystem::sendExit()
	{
		mExit = true;
	}
	//------------------------------------------------------------------
	bool DefaultInputSystem::frameStarted( const Ogre::FrameEvent& _evt )
	{
		if(mExit)
		{
			if(mOgreListener)
			{
				Ogre::Root::getSingleton().removeFrameListener(this);
				Ogre::WindowEventUtilities::removeWindowEventListener(getRenderWindow(), this);
				mOgreListener = false;
			}
			return false;
		}

		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseFrameListener* frameListener = it.getNext();
			frameListener->frameStarted(_evt);
		}
		return true;
	}
	//------------------------------------------------------------------
	bool DefaultInputSystem::frameRenderingQueued(const Ogre::FrameEvent& _evt)
	{
		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseFrameListener* frameListener = it.getNext();
			frameListener->frameRenderingQueued(_evt);
		}
		return true;
	}
	//------------------------------------------------------------------
	bool DefaultInputSystem::frameEnded(const Ogre::FrameEvent& _evt)
	{
		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
		{
			BaseFrameListener* frameListener = it.getNext();
			frameListener->frameEnded(_evt);
		}
		return true;
	}

} // namespace GothOgre
