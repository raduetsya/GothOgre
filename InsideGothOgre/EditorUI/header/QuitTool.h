#ifndef QUIT_TOOL_H
#define QUIT_TOOL_H


namespace GUISystem
{
	class QuitTool : public Tool
	{
	public:
		QuitTool();
		~QuitTool();
	
	protected:
		ToolOperation* activate(const Any&);
	};

	//------------------------------------------------------------------------
	class QuitToolFactory : public ToolFactory
	{
	public:
		QuitToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // QUIT_TOOL_H