#ifndef LOAD_WORLD_3DS_TOOL_H
#define LOAD_WORLD_3DS_TOOL_H


namespace GUISystem
{
	class _3dsFileDialog;

	//-------------------------------------------------------------------
	class LoadWorld3dsTool : public Tool
	{
	public:
		LoadWorld3dsTool();
		~LoadWorld3dsTool();

		ToolOperation* activate(const Any& _toolParameter);

		struct ToolParam
		{
			bool    merge;
			size_t  recentlyFileIndex;
		};

	private:
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED(LoadWorld3dsTool);
		void updateMenuItems();
	};


	//-------------------------------------------------------------------
	class LoadWorld3dsToolOperation : public ToolOperation
	{
	public:
		LoadWorld3dsToolOperation(const LoadWorld3dsTool::ToolParam& _toolParam);
		~LoadWorld3dsToolOperation();

	private:
		void notifyEndDialog(const String& _filename);
		void processFileSelect(const String& _filename, bool _merge);

	private:
		bool             mMerge;
		_3dsFileDialog*  mDialog;
	};


	//-------------------------------------------------------------------
	class LoadWorld3dsToolFactory : public ToolFactory
	{
	public:
		LoadWorld3dsToolFactory();
		Tool* createTool();
	};

	
} // namespace GUISystem
	
#endif // LOAD_WORLD_3DS_TOOL_H
