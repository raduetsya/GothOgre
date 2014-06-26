#ifndef DELETE_TOOL_H
#define DELETE_TOOL_H


namespace GUISystem
{
	class DeleteTool : public Tool
	{
	public:
		DeleteTool();
		~DeleteTool();
		ToolOperation* activate(const Any&);

	private:
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED( DeleteTool );
		GOTHOGRE_DECLARE_WORLD_OBJECT_SELECT_CHANGED( DeleteTool );
		void updateMenuItems();

	private:
		void initConstants();

		String ROOT_MENU_ID;
		String ROOT_MENU_CAPTION;
		String MENU_ITEM_ID;
		String MENU_ITEM_CAPTION;
		String MENU_ITEM_DESC;
		KeyList DEFAULT_HOTKEY;
		String HOTKEY_CONFIG_KEY_NAME;
	};


	//------------------------------------------------------------------------
	class DeleteToolOperation : public ToolOperation
	{
	public:
		DeleteToolOperation();
		~DeleteToolOperation();

	private:
		void notifyMessageBoxChoice(MessageBoxButton _button);
		void performDelete();

	private:
		World*  mWorld;
	};


	//------------------------------------------------------------------------
	class DeleteToolFactory : public ToolFactory
	{
	public:
		DeleteToolFactory();
		Tool* createTool();
	};

} // namespace GUISystem

#endif // DELETE_TOOL_H