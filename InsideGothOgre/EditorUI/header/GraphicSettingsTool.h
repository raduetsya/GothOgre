#ifndef GRAPHIC_SETTINGS_TOOL_H
#define GRAPHIC_SETTINGS_TOOL_H


namespace GUISystem
{
	class GraphicSettingsDialog;

	//---------------------------------------------------------------------
	class GraphicSettingsTool : public Tool
	{
	public:
		GraphicSettingsTool();
		~GraphicSettingsTool();
		ToolOperation* activate(const Any&);
	};

	//---------------------------------------------------------------------
	class GraphicSettingsToolOperation : public ToolOperation
	{
	public:
		GraphicSettingsToolOperation();
		~GraphicSettingsToolOperation();

	private:
		void notifyEndDialog();

	private:
		GraphicSettingsDialog* mDialog;
	};

	//---------------------------------------------------------------------
	class GraphicSettingsToolFactory : public ToolFactory
	{
	public:
		GraphicSettingsToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // GRAPHIC_SETTINGS_TOOL_H
