#ifndef LOAD_WORLD_TOOL_H
#define LOAD_WORLD_TOOL_H


namespace GUISystem
{
	class ZenFileDialog;

	//---------------------------------------------------------------------
	class LoadWorldTool : public Tool
	{
	public:
		LoadWorldTool();
		~LoadWorldTool();

		// Tool parameter is used: if it is string "<Merge>"
		// then the tool will merge worlds
		ToolOperation* activate(const Any& _toolParameter);

		struct ToolParam
		{
			bool    merge;
			size_t  recentlyFileIndex;
		};

	private:
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED(LoadWorldTool);
		void updateMenuItems();
	};


	//-------------------------------------------------------------------
	class LoadWorldToolOperation : public ToolOperation
	{
	public:
		LoadWorldToolOperation(const LoadWorldTool::ToolParam& _toolParam);
		~LoadWorldToolOperation();

	private:
		void notifyEndDialog(const String& _filename);
		void processFileSelect(const String& _filename, bool _merge);

	private:
		bool            mMerge;
		ZenFileDialog*  mDialog;
	};


	//-------------------------------------------------------------------
	class LoadWorldToolFactory : public ToolFactory
	{
	public:
		LoadWorldToolFactory();
		Tool* createTool();
	};

} // namespace GUISystem
	
#endif // LOAD_WORLD_TOOL_H
