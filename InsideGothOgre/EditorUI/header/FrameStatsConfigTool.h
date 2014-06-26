#ifndef FRAME_STATS_CONFIG_TOOL_H
#define FRAME_STATS_CONFIG_TOOL_H


namespace GUISystem
{
	class FrameStatsConfigDialog;

	//-----------------------------------------------------------------------
	class FrameStatsConfigTool : public Tool
	{
	public:
		FrameStatsConfigTool();
		~FrameStatsConfigTool();
		ToolOperation* activate(const Any&);
	};

	//-----------------------------------------------------------------------
	class FrameStatsConfigToolOperation : public ToolOperation
	{
	public:
		FrameStatsConfigToolOperation();
		~FrameStatsConfigToolOperation();

	private:
		void notifyEndDialog();

	private:
		FrameStatsConfigDialog* mDialog;
	};

	//-----------------------------------------------------------------------
	class FrameStatsConfigToolFactory : public ToolFactory
	{
	public:
		FrameStatsConfigToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // FRAME_STATS_TOOL_H