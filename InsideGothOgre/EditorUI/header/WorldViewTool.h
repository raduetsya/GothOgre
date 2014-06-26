#ifndef WORLD_VIEW_TOOL_H
#define WORLD_VIEW_TOOL_H


namespace GUISystem
{
	//-----------------------------------------------------------------------------
	class WorldViewTool : public Tool
	{
	public:
		WorldViewTool();
		~WorldViewTool();
		
		ToolOperation* activate(const Any& _toolParameter);

		struct ToolParam
		{
			String worldName;
		};

	private:
		void addMenuItem(World* _world);
		void removeMenuItem(World* _world);
		void updateMenuItem(World* _world);

	private:
		GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_ACTIVE_CHANGED(WorldViewTool);
	};



	//----------------------------------------------------------------------------
	class WorldViewToolFactory : public ToolFactory
	{
	public:
		WorldViewToolFactory();
		Tool* createTool();
	};

} // namespace GUISystem

#endif // WORLD_VIEW_TOOL_H