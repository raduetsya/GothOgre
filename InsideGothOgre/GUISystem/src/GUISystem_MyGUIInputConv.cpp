#include "GUISystem_Precompiled.h"
#include "GUISystem_MyGUIInputConv.h"

namespace GUISystem
{
	MyGUI::KeyCode MyGUIInputConv::keyCodeToMyGUI(KeyCode _kc)
	{
		return (MyGUI::KeyCode) (MyGUI::KeyCode::Enum) (int) _kc;
	}

	KeyCode MyGUIInputConv::keyCodeFromMyGUI(MyGUI::KeyCode _myguiKc)
	{
		return (KeyCode) _myguiKc.toValue();
	}

	MyGUI::MouseButton MyGUIInputConv::mouseButtonToMyGUI(MouseButton _mb)
	{
		return (MyGUI::MouseButton) (MyGUI::MouseButton::Enum) (int) _mb;
	}

	MouseButton MyGUIInputConv::mouseButtonFromMyGUI(MyGUI::MouseButton _myguiMb)
	{
		return (MouseButton) _myguiMb.toValue();
	}

} // namespace GUISystem
