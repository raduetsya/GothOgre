#ifndef NEW_WORLD_TOOL_H
#define NEW_WORLD_TOOL_H


namespace GUISystem
{
	class NewWorldTool : public Tool
	{
	public:
		NewWorldTool();
		~NewWorldTool();
	
	protected:
		ToolOperation* activate(const Any&);
	};

	//------------------------------------------------------------------------
	class NewWorldToolFactory : public ToolFactory
	{
	public:
		NewWorldToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // NEW_WORLD_TOOL_H