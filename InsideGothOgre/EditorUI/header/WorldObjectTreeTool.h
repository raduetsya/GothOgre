#ifndef WORLD_OBJECT_TREE_TOOL_H
#define WORLD_OBJECT_TREE_TOOL_H


namespace GUISystem
{
	//-----------------------------------------------------------------------
	class WorldObjectTreeTool : public Tool
	{
	public:
		WorldObjectTreeTool();
		~WorldObjectTreeTool();
		ToolOperation* activate(const Any&);

	private:
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED( WorldObjectTreeTool );
	};


	//-----------------------------------------------------------------------
	class WorldObjectTreeToolFactory : public ToolFactory
	{
	public:
		WorldObjectTreeToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // WORLD_OBJECT_TREE_TOOL_H