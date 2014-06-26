#include "Precompiled.h"
#include "WorldObjectPropertiesTool.h"
#include "WorldObjectPropertiesWindowManager.h"

namespace GUISystem
{
	WorldObjectPropertiesTool::WorldObjectPropertiesTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Windows", MenuItemType::Popup, "#{Windows}");
		
		mb->insertItem("WorldObjectProperties", MenuItemType::Normal, "", "Windows");
		mb->setItemDescription("WorldObjectProperties", "#{WorldObjectProperties_Desc}");
		mb->attachTool("WorldObjectProperties", this);
		
		registerUISettingsListener();
	}
	//------------------------------------------------------------------------------------
	WorldObjectPropertiesTool::~WorldObjectPropertiesTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("WorldObjectProperties");
		mb->removeItem("WorldObjectPropertiesSep");
		mb->removeItem("Windows");
	}
	//------------------------------------------------------------------------------------
	void WorldObjectPropertiesTool::uiSettingsChanged( const ConfigSettingsEvent& _evt)
	{
		bool value = WorldObjectPropertiesWindowManager::getSingleton().isVisible();
		String caption;
		if(value)
			caption = "#{WorldObjectProperties_Shown}";
		else
			caption = "#{WorldObjectProperties_Hidden}";

		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemCaption("WorldObjectProperties", caption);
	}
	//------------------------------------------------------------------------------------
	ToolOperation* WorldObjectPropertiesTool::activate(const Any&)
	{
		bool value = WorldObjectPropertiesWindowManager::getSingleton().isVisible();
		value = !value;
		WorldObjectPropertiesWindowManager::getSingleton().setVisible(value);
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	WorldObjectPropertiesToolFactory::WorldObjectPropertiesToolFactory()
		: ToolFactory("WorldObjectPropertiesTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* WorldObjectPropertiesToolFactory::createTool()
	{
		return new WorldObjectPropertiesTool;
	}
	
} // namespace GUISystem
