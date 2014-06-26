#include "Precompiled.h"
#include "FreeLookTool.h"

namespace GUISystem
{
	//------------------------------------------------------------------------
	void FreeLookTool::initConstants()
	{
		ROOT_MENU_ID      = "Tools";
		ROOT_MENU_CAPTION = "#{Tools}";
		MENU_ITEM_ID      = "FreeLook";
		MENU_ITEM_CAPTION = "#{FreeLook}";
		MENU_ITEM_DESC    = "#{FreeLook_Desc}";
		DEFAULT_HOTKEY    = KeyList(KeyCode::F3);
		HOTKEY_CONFIG_KEY_NAME = "FreeLook";
	}
	//------------------------------------------------------------------------
	FreeLookTool::FreeLookTool()
	{
		initConstants();

		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem( ROOT_MENU_ID, MenuItemType::Popup, ROOT_MENU_CAPTION );
		mb->insertItem( MENU_ITEM_ID, MenuItemType::Normal, MENU_ITEM_CAPTION, ROOT_MENU_ID );
		mb->setItemDescription( MENU_ITEM_ID, MENU_ITEM_DESC );
		mb->insertHotkey( MENU_ITEM_ID, HOTKEY_CONFIG_KEY_NAME, "Editor", DEFAULT_HOTKEY);
		mb->attachTool( MENU_ITEM_ID, this);

		updateMenuItems();
		registerWorldListener();
	}
	//------------------------------------------------------------------------
	FreeLookTool::~FreeLookTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem( MENU_ITEM_ID );
		mb->removeItem( ROOT_MENU_ID );
	}
	//------------------------------------------------------------------------
	ToolOperation* FreeLookTool::activate(const Any&)
	{
		return new FreeLookToolOperation;
	}
	//------------------------------------------------------------------------------------
	void FreeLookTool::worldActiveChanged(const WorldEvent& _evt)
	{
		updateMenuItems();
	}
	//------------------------------------------------------------------------------------
	void FreeLookTool::updateMenuItems()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemEnabled( MENU_ITEM_ID, WorldManager::getSingleton().getActiveWorld() != nullptr);
	}
	//------------------------------------------------------------------------
	FreeLookToolOperation::FreeLookToolOperation()
	{
		setCrossHairLayout("FreeLookCrossHair.layout");
		registerControlSettingsListener();
		registerKeyListener(InputPriority::TOOL);
	}
	//------------------------------------------------------------------------
	FreeLookToolOperation::~FreeLookToolOperation()
	{
	}
	//---------------------------------------------------------------------------
	KeyEventResult FreeLookToolOperation::keyPressed( const KeyEvent& _evt)
	{
		InputSystem& in = InputSystem::getSingleton();
		if( in.isKeyDown(mKeyExit) )
		{
			deactivateTool();
			return KeyEventResult::PROCESSED;
		}
		return KeyEventResult::NOT_PROCESSED;
	}
	//------------------------------------------------------------------------
	void FreeLookToolOperation::controlSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		ControlSettings& cs = ControlSettings::getSingleton();
		mKeyExit = cs.getSetting("FreeLook", "Editor", KeyList(KeyCode::F3));
	};
	//---------------------------------------------------------------------------
	FreeLookToolFactory::FreeLookToolFactory()
		: ToolFactory("FreeLookTool", "EditorToolManager")
	{
	}
	//---------------------------------------------------------------------------
	Tool* FreeLookToolFactory::createTool()
	{
		return new FreeLookTool;
	}

} // namespace GUISystem
