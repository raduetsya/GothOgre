#include "Precompiled.h"
#include "WorldObjectTreeTool.h"
#include "WorldObjectTreeWindowManager.h"

namespace GUISystem
{
	WorldObjectTreeTool::WorldObjectTreeTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Windows", MenuItemType::Popup, "#{Windows}");
		
		mb->insertItem("WorldObjectTree", MenuItemType::Normal, "", "Windows");
		mb->setItemDescription("WorldObjectTree", "#{WorldObjectTree_Desc}");
		mb->attachTool("WorldObjectTree", this);
		
		registerUISettingsListener();
	}
	//------------------------------------------------------------------------------------
	WorldObjectTreeTool::~WorldObjectTreeTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("WorldObjectTree");
		mb->removeItem("Windows");
	}
	//------------------------------------------------------------------------------------
	void WorldObjectTreeTool::uiSettingsChanged( const ConfigSettingsEvent& _evt)
	{
		bool value = WorldObjectTreeWindowManager::getSingleton().isVisible();
		String caption;
		if(value)
			caption = "#{WorldObjectTree_Shown}";
		else
			caption = "#{WorldObjectTree_Hidden}";

		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemCaption("WorldObjectTree", caption);
	}
	//------------------------------------------------------------------------------------
	ToolOperation* WorldObjectTreeTool::activate(const Any&)
	{
		bool value = WorldObjectTreeWindowManager::getSingleton().isVisible();
		value = !value;
		WorldObjectTreeWindowManager::getSingleton().setVisible(value);
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	WorldObjectTreeToolFactory::WorldObjectTreeToolFactory()
		: ToolFactory("WorldObjectTreeTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* WorldObjectTreeToolFactory::createTool()
	{
		return new WorldObjectTreeTool;
	}
	
} // namespace GUISystem
