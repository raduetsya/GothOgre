#ifndef FREE_LOOK_TOOL_H
#define FREE_LOOK_TOOL_H

#include "CameraMovingFreeLook.h"
#include "SelectingObjectsWithMouse.h"

namespace GUISystem
{
	class FreeLookTool : public Tool
	{
	public:
		FreeLookTool();
		~FreeLookTool();
		ToolOperation* activate(const Any&);

	private:
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED(FreeLookTool);
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
	class FreeLookToolOperation : public ToolOperation,
		CameraMovingFreeLook, SelectingObjectsWithMouse
	{
	public:
		FreeLookToolOperation();
		~FreeLookToolOperation();
	
	private:
		GOTHOGRE_DECLARE_KEY_PRESSED( FreeLookToolOperation );
		GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED( FreeLookToolOperation );

	private:
		KeyList	mKeyExit;
	};


	//------------------------------------------------------------------------
	class FreeLookToolFactory : public ToolFactory
	{
	public:
		FreeLookToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // FREE_LOOK_TOOL_H