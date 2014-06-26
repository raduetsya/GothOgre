#ifndef WORLD_OBJECT_PROPERTIES_TOOL_H
#define WORLD_OBJECT_PROPERTIES_TOOL_H


namespace GUISystem
{
	//-----------------------------------------------------------------------
	class WorldObjectPropertiesTool : public Tool
	{
	public:
		WorldObjectPropertiesTool();
		~WorldObjectPropertiesTool();
		ToolOperation* activate(const Any&);

	private:
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED( WorldObjectPropertiesTool );
	};


	//-----------------------------------------------------------------------
	class WorldObjectPropertiesToolFactory : public ToolFactory
	{
	public:
		WorldObjectPropertiesToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // WORLD_OBJECT_PROPERTIES_TOOL_H