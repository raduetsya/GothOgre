#ifndef EDITOR_INIT_BEFORE_TOOLS_H
#define EDITOR_INIT_BEFORE_TOOLS_H

namespace GUISystem
{
	class EditorMenuBar;
	class EditorBackground;
	class WorldViewWindowManager;
	class WorldObjectPropertiesWindowManager;
	class WorldObjectTreeWindowManager;


	//--------------------------------------------------------------------------
	class EditorInitBeforeTools
	{
	public:
		EditorInitBeforeTools();
		~EditorInitBeforeTools();

	private:
		GOTHOGRE_DECLARE_UI_MODE_ENTERED_EXITED( EditorInitBeforeTools );
		
		void initialise();
		void shutdown();

	private:
		EditorMenuBar*                      mEditorMenuBar;
		EditorBackground*                   mEditorBackground;
		WorldViewWindowManager*             mWorldViewWindowManager;
		WorldObjectPropertiesWindowManager* mWorldObjectPropertiesWindowManager;
		WorldObjectTreeWindowManager*       mWorldObjectTreeWindowManager;
	};

}

#endif // EDITOR_INIT_BEFORE_TOOLS_H