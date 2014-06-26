#include "GUISystem_Precompiled.h"
#include "GUISystem_InputPriority.h"
#include "GUISystem_MyGUIInput.h"
#include "GUISystem_MyGUIInputConv.h"


namespace GUISystem
{
	//------------------------------------------------------------------------
	MyGUIInput::MyGUIInput()
		: mMouseCaptured(false), 
		  mProgressStarted(false)
		  
	{
	}
	//------------------------------------------------------------------------
	MyGUIInput::~MyGUIInput()
	{
	}
	//------------------------------------------------------------------------
	void MyGUIInput::initialise()
	{
		// Register as listener
		registerKeyListener(InputPriority::GUI_WIDGET);
		registerMouseListener(InputPriority::GUI_WIDGET);
		registerProgressListener();
	}
	//--------------------------------------------------------------------------
	void MyGUIInput::shutdown()
	{
		// Release mouse if it's captured yet
		releaseMouse();

		// Unregister as listener
		unregisterKeyListener();
		unregisterMouseListener();
		unregisterProgressListener();
	}
	//-------------------------------------------------------------------------------
	MouseEventResult MyGUIInput::mouseMoved( const MouseEvent& _evt )
	{
		MyGUI::PointerManager::getInstance().setVisible(true);

		if(MyGUI::InputManager::getInstance().injectMouseMove(
			_evt.getAbsX(), _evt.getAbsY(), _evt.getAbsZ()))
		{
			MyGUI::Widget* focusWidget = MyGUI::InputManager::getInstance().getMouseFocusWidget();
			if(focusWidget && canWidgetProcessMouse(focusWidget))
				return MouseEventResult::PROCESSED;
		}

		if(MyGUI::InputManager::getInstance().isModalAny())
			return MouseEventResult::PROCESSED;

		return MouseEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------------
	MouseEventResult MyGUIInput::mouseLeft( const MouseEvent& _evt )
	{
		MyGUI::PointerManager::getInstance().setVisible(false);
		return MouseEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------------
	MouseEventResult MyGUIInput::mousePressed( const MouseEvent& _evt )
	{
		if(MyGUI::InputManager::getInstance().injectMousePress(
			_evt.getAbsX(), _evt.getAbsY(), 
			MyGUIInputConv::mouseButtonToMyGUI(_evt.getMouseButton())))
		{
			MyGUI::Widget* focusWidget = MyGUI::InputManager::getInstance().getMouseFocusWidget();
			if(focusWidget && canWidgetProcessMouse(focusWidget))
			{
				captureMouseIfNecessary();
				return MouseEventResult::PROCESSED;
			}
		}

		if(MyGUI::InputManager::getInstance().isModalAny())
			return MouseEventResult::PROCESSED;

		return MouseEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------------
	MouseEventResult MyGUIInput::mouseReleased( const MouseEvent& _evt )
	{
		if(MyGUI::InputManager::getInstance().injectMouseRelease(
			_evt.getAbsX(), _evt.getAbsY(), 
			MyGUIInputConv::mouseButtonToMyGUI(_evt.getMouseButton())))
		{
			MyGUI::Widget* focusWidget = MyGUI::InputManager::getInstance().getMouseFocusWidget();
			if(focusWidget && canWidgetProcessMouse(focusWidget))
			{
				releaseMouse();
				return MouseEventResult::PROCESSED;
			}
		}

		if(MyGUI::InputManager::getInstance().isModalAny())
			return MouseEventResult::PROCESSED;

		return MouseEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------------
	void MyGUIInput::captureMouseIfNecessary()
	{
		if(!mMouseCaptured 
			&& MyGUI::InputManager::getInstance().isCaptureMouse() 
			&& !mProgressStarted)
		{
			InputSystem::getSingleton().clipCursor(0, 0,
				InputSystem::getSingleton().getWidth(),
				InputSystem::getSingleton().getHeight());
			mMouseCaptured = true;
		}
	}
	//-------------------------------------------------------------------------------
	void MyGUIInput::releaseMouse()
	{
		if(mMouseCaptured)
		{
			InputSystem::getSingleton().freeCursor();
			mMouseCaptured = false;
		}		
	}
	//-------------------------------------------------------------------------------
	void MyGUIInput::progressStarted()
	{
		mProgressStarted = true;
		releaseMouse();
	}
	//-------------------------------------------------------------------------------
	void MyGUIInput::progressEnded()
	{
		mProgressStarted = false;
	}
	//-------------------------------------------------------------------------------
	KeyEventResult MyGUIInput::keyPressed( const KeyEvent& _evt )
	{
		MyGUI::Widget* focusWidget = MyGUI::InputManager::getInstance().getKeyFocusWidget();
		if(focusWidget && canWidgetProcessKeys(focusWidget))
		{
			if(MyGUI::InputManager::getInstance().injectKeyPress(
				MyGUIInputConv::keyCodeToMyGUI(_evt.getKeyCode()), _evt.getText()))
				return KeyEventResult::PROCESSED;
		}

		if(MyGUI::InputManager::getInstance().isModalAny())
			return KeyEventResult::PROCESSED;

		return KeyEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------------
	KeyEventResult MyGUIInput::keyReleased( const KeyEvent& _evt )
	{
		MyGUI::Widget* focusWidget = MyGUI::InputManager::getInstance().getKeyFocusWidget();
		if(focusWidget && canWidgetProcessKeys(focusWidget))
		{
			if(MyGUI::InputManager::getInstance().injectKeyRelease(
				MyGUIInputConv::keyCodeToMyGUI(_evt.getKeyCode())))
				return KeyEventResult::PROCESSED;
		}

		if(MyGUI::InputManager::getInstance().isModalAny())
			return KeyEventResult::PROCESSED;

		return KeyEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------------
	bool MyGUIInput::canWidgetProcessMouse(MyGUI::Widget* _widget)
	{
		if(_widget->isType<MyGUI::Canvas>())
			return false;

		return true;
	}
	//-------------------------------------------------------------------------------
	bool MyGUIInput::canWidgetProcessKeys(MyGUI::Widget* _widget)
	{
		if(_widget->isType<MyGUI::Edit>() 
		|| _widget->isType<MyGUI::ListBox>()
		|| _widget->isType<MyGUI::ComboBox>() )
			return true;

		return false;
	}

} // namespace GUISystem