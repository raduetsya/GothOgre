#ifndef ABOUT_GOTHOGRE_TOOL_H
#define ABOUT_GOTHOGRE_TOOL_H


namespace GUISystem
{
	class AboutGothOgreDialog;

	class AboutGothOgreTool : public Tool
	{
	public:
		AboutGothOgreTool();
		~AboutGothOgreTool();
		ToolOperation* activate(const Any&);
	};


	//----------------------------------------------------------------------
	class AboutGothOgreToolOperation : public ToolOperation
	{
	public:
		AboutGothOgreToolOperation();
		~AboutGothOgreToolOperation();

	private:
		void notifyEndDialog();

	private:
		AboutGothOgreDialog* mDialog;
	};


	//----------------------------------------------------------------------
	class AboutGothOgreToolFactory : public ToolFactory
	{
	public:
		AboutGothOgreToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // ABOUT_GOTHOGRE_TOOL_H
