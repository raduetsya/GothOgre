#ifndef GUISYSTEM_MYGUI_INPUT_H
#define GUISYSTEM_MYGUI_INPUT_H

#include "GUISystem_InputPriority.h"


namespace GUISystem
{
	class MyGUIInput
	{
	public:
		MyGUIInput();
		~MyGUIInput();

		void initialise();
		void shutdown();

	private:
		GOTHOGRE_DECLARE_MOUSE_PRESSED_RELEASED_MOVED_LEFT( MyGUIInput );
		GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED( MyGUIInput );
		GOTHOGRE_DECLARE_PROGRESS_STARTED_ENDED( MyGUIInput );

		static bool canWidgetProcessKeys(MyGUI::Widget* _widget);
		static bool canWidgetProcessMouse(MyGUI::Widget* _widget);
		
		void captureMouseIfNecessary();
		void releaseMouse();

	private:
		Viewport*  mUIViewport;
		bool       mMouseCaptured;
		bool       mProgressStarted;
	};

}

#endif // GUISYSTEM_MYGUI_INPUT_H