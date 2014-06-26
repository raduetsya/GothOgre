#include "Precompiled.h"
#include "OISInputSupport.h"
#include "OISConverter.h"

namespace GothOgre
{
	OISInputSupport::OISInputSupport()
		: mOISInputMgr(nullptr), mOISKeyboard(nullptr), 
		  mOISMouse(nullptr), mClipCursor(false)
	{
	}
	//------------------------------------------------------------------------------
	OISInputSupport::~OISInputSupport()
	{
	}
	//------------------------------------------------------------------------------
	const String& OISInputSupport::getName() const
	{
		static const String s = "OISInputSupport";
		return s;
	}
	//------------------------------------------------------------------------------
	void OISInputSupport::initialise(RenderWindow* _window, bool _useOISMouse)
	{
		createOISInputMgr();
		createOISKeyboard();
		if(_useOISMouse)
			createOISMouse();

		Ogre::Root::getSingleton().addFrameListener(this);
		Ogre::WindowEventUtilities::addWindowEventListener(_window, this);
	}
	//------------------------------------------------------------------------------
	void OISInputSupport::shutdown()
	{
		destroyOISMouse();
		destroyOISKeyboard();
		destroyOISInputMgr();

		Ogre::Root::getSingleton().removeFrameListener(this);
		Ogre::WindowEventUtilities::removeWindowEventListener(getRenderWindow(), this);
	}
	//------------------------------------------------------------------
	void OISInputSupport::createOISInputMgr()
	{
		if(!mOISInputMgr)
		{
			GOTHOGRE_INFO("OIS: Initialising");
			size_t winHandle;
			getRenderWindow()->getCustomAttribute("WINDOW", &winHandle);
			OIS::ParamList paramList;
			StrStream windowHndStr;
			windowHndStr << winHandle;
			paramList.insert(make_pair(String("WINDOW"), windowHndStr.str()));
	
			mOISInputMgr = OIS::InputManager::createInputSystem( paramList );
			if(!mOISInputMgr)
				GOTHOGRE_EXCEPT("OIS: Failed initialisation!");
		}
	}
	//------------------------------------------------------------------
	void OISInputSupport::destroyOISInputMgr()
	{
		if(mOISInputMgr)
		{
			GOTHOGRE_INFO("OIS: Shutting down");
			OIS::InputManager::destroyInputSystem(mOISInputMgr);
			mOISInputMgr = nullptr;
		}
	}
	//------------------------------------------------------------------
	void OISInputSupport::createOISKeyboard()
	{
		if(!mOISKeyboard)
		{
			GOTHOGRE_INFO("OIS: Creating a keyboard device");
			mOISKeyboard = static_cast<OIS::Keyboard*>(mOISInputMgr->createInputObject( OIS::OISKeyboard, true ));
			if(!mOISKeyboard)
				GOTHOGRE_ERROR("OIS: Failed to create a keyboard device");

			mOISKeyboard->setEventCallback(this);
		}
	}
	//------------------------------------------------------------------
	void OISInputSupport::destroyOISKeyboard()
	{
		if (mOISKeyboard) 
		{
			GOTHOGRE_INFO("OIS: Destroying a keyboard device");
			mOISInputMgr->destroyInputObject( mOISKeyboard );
			mOISKeyboard = nullptr;
		}
	}
	//------------------------------------------------------------------
	void OISInputSupport::createOISMouse()
	{
		if(!mOISMouse)
		{
			GOTHOGRE_INFO("OIS: Creating a mouse device");
			mOISMouse = static_cast<OIS::Mouse*>(mOISInputMgr->createInputObject( OIS::OISMouse, true ));
			if(!mOISMouse)
				GOTHOGRE_ERROR("OIS: Failed to create a mouse device");

			mOISMouse->setEventCallback(this);
			windowResized(getRenderWindow());
			int width = getRenderWindow()->getWidth();
			int height = getRenderWindow()->getHeight();
			setCursorPos(width / 2, height / 2);
		}
	}
	//------------------------------------------------------------------
	void OISInputSupport::destroyOISMouse()
	{
		if (mOISMouse) 
		{
			GOTHOGRE_INFO("OIS: Destroying a mouse device");
			mOISInputMgr->destroyInputObject( mOISMouse );
			mOISMouse = nullptr;
		}
	}
	//------------------------------------------------------------------------------
	void OISInputSupport::clipCursor(int _left, int _top, int _width, int _height)
	{
		mClipCursor = true;
		mClipLeft = _left;
		mClipTop = _top;
		mClipWidth = _width;
		mClipHeight = _height;
		clipCursorPos();
	}
	//------------------------------------------------------------------
	void OISInputSupport::clipCursorPos()
	{
		if(mClipCursor)
		{
			int x0, y0;
			getCursorPos(&x0,&y0);

			int x = x0, y = y0;
			if(x > mClipLeft + mClipWidth - 1)
				x = mClipLeft + mClipWidth - 1;
			if(x < mClipLeft)
				x = mClipLeft;
			if(y > mClipTop + mClipHeight - 1)
				y = mClipTop + mClipHeight - 1;
			if(y < mClipTop)
				y = mClipTop;

			if(x != x0 || y != y0)
				setCursorPos(x, y);
		}
	}
	//------------------------------------------------------------------------------
	void OISInputSupport::freeCursor()
	{
		mClipCursor = false;
	}
	//------------------------------------------------------------------------------
	void OISInputSupport::setCursorPos(int _x, int _y)
	{
		if(mOISMouse) 
		{
			OIS::MouseState& ms = const_cast<OIS::MouseState&>(mOISMouse->getMouseState());
			ms.X.rel = _x - ms.X.abs;
			ms.X.abs = _x;
			ms.Y.rel = _y - ms.Y.abs;
			ms.Y.abs = _y;
		}
	}
	//------------------------------------------------------------------------------
	void OISInputSupport::getCursorPos(int* _px, int* _py) const
	{
		if(mOISMouse) 
		{
			const OIS::MouseState& ms = mOISMouse->getMouseState();
			*_px = ms.X.abs;
			*_py = ms.Y.abs;
		}
	}
	//------------------------------------------------------------------------------
	bool OISInputSupport::isKeyDown(const KeyCode& _keyCode) const
	{
		if(mOISKeyboard)
			return mOISKeyboard->isKeyDown( OISConverter::keyCodeToOIS( _keyCode ));
		else
			return false;
	}
	//------------------------------------------------------------------------------
	bool OISInputSupport::isMouseDown(const MouseButton& _mouseButton) const
	{
		if(mOISMouse)
		{
			const OIS::MouseState& ms = mOISMouse->getMouseState();
			return ms.buttonDown( OISConverter::mouseButtonToOIS( _mouseButton ));
		}
		else
			return false;
	}
	//------------------------------------------------------------------------------
	bool OISInputSupport::mouseMoved( const OIS::MouseEvent& _evt )
	{
		clipCursorPos();
		const OIS::MouseState& ms = mOISMouse->getMouseState();
		MouseEvent evt2 (ms.X.abs, ms.Y.abs, ms.Z.abs);
		InputSystem::getSingleton()._injectMouseMove (evt2);
		return true;
	}
	//------------------------------------------------------------------------------
	bool OISInputSupport::mousePressed( const OIS::MouseEvent& _evt, OIS::MouseButtonID _id )
	{
		const OIS::MouseState& ms = mOISMouse->getMouseState();
		MouseEvent evt2 (ms.X.abs, ms.Y.abs, ms.Z.abs, OISConverter::mouseButtonFromOIS(_id));
		InputSystem::getSingleton()._injectMousePress (evt2);
		return true;
	}
	//------------------------------------------------------------------------------
	bool OISInputSupport::mouseReleased( const OIS::MouseEvent& _evt, OIS::MouseButtonID _id )
	{
		const OIS::MouseState& ms = mOISMouse->getMouseState();
		MouseEvent evt2 (ms.X.abs, ms.Y.abs, ms.Z.abs, OISConverter::mouseButtonFromOIS(_id));
		InputSystem::getSingleton()._injectMouseRelease (evt2);
		return true;
	}
	//------------------------------------------------------------------------------
	bool OISInputSupport::keyPressed( const OIS::KeyEvent& _evt )
	{
		KeyEvent evt2 (OISConverter::keyCodeFromOIS(_evt.key), _evt.text);
		InputSystem::getSingleton()._injectKeyPress (evt2);
		return true;
	}
	//------------------------------------------------------------------------------
	bool OISInputSupport::keyReleased( const OIS::KeyEvent& _evt )
	{
		KeyEvent evt2 (OISConverter::keyCodeFromOIS(_evt.key), _evt.text);
		InputSystem::getSingleton()._injectKeyRelease (evt2);
		return true;
	}
	//------------------------------------------------------------------------------
	void OISInputSupport::windowResized( RenderWindow* _window )
	{
		if (mOISMouse) 
		{
			const OIS::MouseState& ms = mOISMouse->getMouseState();
			ms.width = _window->getWidth();
			ms.height = _window->getHeight();
		}
	}
	//------------------------------------------------------------------
	bool OISInputSupport::frameStarted(const FrameEvent& _evt)
	{
		if(mOISKeyboard)
			mOISKeyboard->capture();

		if(mOISMouse)
			mOISMouse->capture();

		return true;
	}

} // namespace GothOgre
