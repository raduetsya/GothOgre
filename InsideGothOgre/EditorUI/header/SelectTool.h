#ifndef SELECT_TOOL_H
#define SELECT_TOOL_H

#include "CameraMovingWithKeys.h"
#include "SelectingObjectsWithMouse.h"


namespace GUISystem
{
	class SelectTool : public Tool
	{
	public:
		SelectTool();
		~SelectTool();

		ToolOperation* activate(const Any&);

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
	class SelectToolOperation : public ToolOperation,
		CameraMovingWithKeys, SelectingObjectsWithMouse
	{
	public:
		SelectToolOperation();
		~SelectToolOperation();
	};


	//------------------------------------------------------------------------
	class SelectToolFactory : public ToolFactory
	{
	public:
		SelectToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // SELECT_MOVE_TOOL_H