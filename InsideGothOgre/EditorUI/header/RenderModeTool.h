#ifndef RENDER_MODE_TOOL_H
#define RENDER_MODE_TOOL_H


namespace GUISystem
{
	class RenderModeTool : public Tool
	{
	public:
		RenderModeTool();
		~RenderModeTool();
		ToolOperation* activate(const Any&);

	private:
		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED(RenderModeTool);
		void updateMenuItem();
	};


	class RenderModeToolFactory : public ToolFactory
	{
	public:
		RenderModeToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // RENDER_MODE_TOOL_H