#include "Precompiled.h"
#include "SelectTool.h"

namespace GUISystem
{
	//------------------------------------------------------------------------
	void SelectTool::initConstants()
	{
		ROOT_MENU_ID      = "Tools";
		ROOT_MENU_CAPTION = "#{Tools}";
		MENU_ITEM_ID      = "Select";
		MENU_ITEM_CAPTION = "#{Select}";
		MENU_ITEM_DESC    = "#{Select_Desc}";
		DEFAULT_HOTKEY    = KeyList();
		HOTKEY_CONFIG_KEY_NAME = "Select";
	}
	//------------------------------------------------------------------------
	SelectTool::SelectTool()
	{
		initConstants();

		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem( ROOT_MENU_ID, MenuItemType::Popup, ROOT_MENU_CAPTION );
		mb->insertItem( MENU_ITEM_ID, MenuItemType::Normal, MENU_ITEM_CAPTION, ROOT_MENU_ID );
		mb->setItemDescription( MENU_ITEM_ID, MENU_ITEM_DESC );
		mb->attachTool( MENU_ITEM_ID, this );
		activateTool();
	}
	//------------------------------------------------------------------------------------
	SelectTool::~SelectTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem( MENU_ITEM_ID );
		mb->removeItem( ROOT_MENU_ID );
	}
	//------------------------------------------------------------------------------------
	ToolOperation* SelectTool::activate(const Any&)
	{
		return new SelectToolOperation;
	}
	//------------------------------------------------------------------------------------
	SelectToolOperation::SelectToolOperation()
	{
	}
	//------------------------------------------------------------------------------------
	SelectToolOperation::~SelectToolOperation()
	{
	}
	//------------------------------------------------------------------------------------
	SelectToolFactory::SelectToolFactory()
		: ToolFactory("SelectTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* SelectToolFactory::createTool()
	{
		return new SelectTool;
	}
	
} // namespace GUISystem
