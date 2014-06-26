#ifndef EDITOR_INIT_TOOLS_H
#define EDITOR_INIT_TOOLS_H

namespace GUISystem
{
	class EditorToolManager;

	class EditorInitTools
	{
	public:
		EditorInitTools();
		~EditorInitTools();

	private:
		GOTHOGRE_DECLARE_UI_MODE_ENTERED_EXITED( EditorInitTools );

	private:
		EditorToolManager* mEditorToolManager;
	};
}

#endif // EDITOR_INIT_TOOLS_H