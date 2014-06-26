#ifndef EDITOR_TOOL_MANAGER_H
#define EDITOR_TOOL_MANAGER_H

namespace GUISystem
{
	class EditorToolManager : public Singleton<EditorToolManager>, public ToolManager
	{
	public:
		EditorToolManager();
	};

} // namespace GUISystem

#endif // EDITOR_TOOL_MANAGER_H