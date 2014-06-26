#include "Precompiled.h"
#include "NewWorldTool.h"

namespace GUISystem
{
	//------------------------------------------------------------------------------------
	NewWorldTool::NewWorldTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("File", MenuItemType::Popup, "#{File}");
		mb->insertItem("NewWorld", MenuItemType::Normal, "#{NewWorld}", "File");
		mb->setItemDescription("NewWorld", "#{NewWorld_Desc}");
		mb->attachTool("NewWorld", this);
	}
	//------------------------------------------------------------------------------------
	NewWorldTool::~NewWorldTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("NewWorld");
		mb->removeItem("File");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* NewWorldTool::activate(const Any&)
	{
		WorldManager::getSingleton().createWorld();
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	NewWorldToolFactory::NewWorldToolFactory()
		: ToolFactory("NewWorldTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* NewWorldToolFactory::createTool()
	{
		return new NewWorldTool;
	}
		
} // namespace GUISystem