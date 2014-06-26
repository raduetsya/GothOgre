#ifndef FRAME_STATS_TOOL_H
#define FRAME_STATS_TOOL_H


namespace GUISystem
{
	//-----------------------------------------------------------------------
	class FrameStatsTool : public Tool
	{
	public:
		FrameStatsTool();
		~FrameStatsTool();
		ToolOperation* activate(const Any& _parameter);

	private:
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED( FrameStatsTool );
		void updateMenuItem();
	};

	//-----------------------------------------------------------------------
	class FrameStatsToolFactory : public ToolFactory
	{
	public:
		FrameStatsToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // FRAME_STATS_TOOL_H