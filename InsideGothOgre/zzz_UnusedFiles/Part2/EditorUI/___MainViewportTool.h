#ifndef MAIN_VIEWPORT_TOOL_H
#define MAIN_VIEWPORT_TOOL_H


namespace GUISystem
{
	class MainViewportWindow;


	class MainViewportTool : public Tool
	{
	public:
		MainViewportTool();
		~MainViewportTool();
		ToolOperation* activate(const String&);

	private:
		MainViewportWindow*  mWindow;
	};


	//---------------------------------------------------------------------
	class MainViewportToolFactory : public ToolFactory
	{
	public:
		MainViewportToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // MAIN_VIEWPORT_RESIZE_TOOL_H
