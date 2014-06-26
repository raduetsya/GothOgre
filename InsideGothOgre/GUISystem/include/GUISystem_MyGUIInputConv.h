#ifndef GUISYSTEM_MYGUI_INPUT_CONV_H
#define GUISYSTEM_MYGUI_INPUT_CONV_H

namespace GUISystem
{
	struct GUISYSTEM_EXPORT MyGUIInputConv
	{
		static MyGUI::KeyCode keyCodeToMyGUI(KeyCode _kc);
		static KeyCode keyCodeFromMyGUI(MyGUI::KeyCode _myguiKc);

		static MyGUI::MouseButton mouseButtonToMyGUI(MouseButton _mb);
		static MouseButton mouseButtonFromMyGUI(MyGUI::MouseButton _myguiMb);
	};

} // namespace GUISystem

#endif // GUISYSTEM_MYGUI_INPUT_CONV_H
