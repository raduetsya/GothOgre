#include "Precompiled.h"
#include "WorldViewTool.h"
#include "WorldViewWindow.h"


namespace GUISystem
{
	//------------------------------------------------------------------------------------
	WorldViewTool::WorldViewTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Windows", MenuItemType::Popup, "#{Windows}");
		mb->insertItem("WorldViewSep", MenuItemType::Separator, "", "Windows");

		WorldManager::WorldIterator it = WorldManager::getSingleton().getWorldIterator();
		while(it.hasMoreElements())
		{
			World* world = it.getNext();
			addMenuItem(world);
		}

		registerWorldListener();
	}
	//------------------------------------------------------------------------------------
	WorldViewTool::~WorldViewTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");

		WorldManager::WorldIterator it = WorldManager::getSingleton().getWorldIterator();
		while(it.hasMoreElements())
		{
			World* world = it.getNext();
			removeMenuItem(world);
		}

		mb->removeItem("WorldViewSep");
		mb->removeItem("Windows");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* WorldViewTool::activate(const Any& _toolParameter)
	{
		const ToolParam& tp = *any_cast<ToolParam>(&_toolParameter);
		WorldManager::WorldIterator it = WorldManager::getSingleton().getWorldIterator();
		while(it.hasMoreElements())
		{
			World* world = it.getNext();
			if(world->getName() == tp.worldName)
			{
				WorldManager::getSingleton().setActiveWorld(world);
				break;
			}
		}
		return nullptr;
	}
	//----------------------------------------------------------------------------
	void WorldViewTool::worldCreated(const WorldEvent& _evt)
	{
		World* world = _evt.getWorld();
		addMenuItem(world);
	}
	//----------------------------------------------------------------------------
	void WorldViewTool::worldDestroyed(const WorldEvent& _evt)
	{
		World* world = _evt.getWorld();
		removeMenuItem(world);
	}
	//----------------------------------------------------------------------------
	void WorldViewTool::worldActiveChanged(const WorldEvent& _evt)
	{
		World* world = _evt.getWorld();
		updateMenuItem(world);
	}
	//------------------------------------------------------------------------------------
	void WorldViewTool::addMenuItem(World* _world)
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		const String& id = "WorldView '" + _world->getName() + "'";
		String caption = WorldViewWindow::generateWindowCaption(_world);
		mb->insertItem(id, MenuItemType::Normal, caption, "Windows", "WorldViewSep");
		ToolParam tp;
		tp.worldName = _world->getName();
		mb->attachTool(id, this, (AnyLite) tp);
	}
	//------------------------------------------------------------------------------------
	void WorldViewTool::removeMenuItem(World* _world)
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		const String& id = "WorldView '" + _world->getName() + "'";
		mb->removeItem(id);
	}
	//------------------------------------------------------------------------------------
	void WorldViewTool::updateMenuItem(World* _world)
	{
		const String& id = "WorldView '" + _world->getName() + "'";
		String caption = WorldViewWindow::generateWindowCaption(_world);
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemCaption(id, caption);
	}
	//------------------------------------------------------------------------------------
	WorldViewToolFactory::WorldViewToolFactory()
		: ToolFactory("WorldViewTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* WorldViewToolFactory::createTool()
	{
		return new WorldViewTool;
	}
	
} // namespace GUISystem
