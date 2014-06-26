#include "Precompiled.h"
#include "WorldObjectPropertiesWindow.h"
#include "WorldObjectPropertiesWindowManager.h"


namespace GUISystem
{
	WorldObjectPropertiesWindow::WorldObjectPropertiesWindow()
		: ZenObjectPropertiesWindow()
	{
		initialiseByAttributes(this);
		getWindow()->setCaptionWithReplacing("#{WorldObjectProperties}");

		updateSelectedObjectsProps();

		registerWorldListener();
		registerWorldObjectListener();
	}
	//---------------------------------------------------------------------------
	WorldObjectPropertiesWindow::~WorldObjectPropertiesWindow()
	{
	}
	//---------------------------------------------------------------------------
	void WorldObjectPropertiesWindow::worldActiveChanged(const WorldEvent& _evt)
	{
		updateSelectedObjectsProps();
	}
	//---------------------------------------------------------------------------
	void WorldObjectPropertiesWindow::updateSelectedObjectsProps()
	{
		removeAllObjects();
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(world)
		{
			World::SelectionIterator it = world->getSelectionIterator();
			while(it.hasMoreElements())
			{
				WorldObject* obj = it.getNext();
				addObject( obj );
			}
		}
	}
	//---------------------------------------------------------------------------
	void WorldObjectPropertiesWindow::worldObjectChanged(const WorldObjectEvent& _evt)
	{
		setNeedUpdateValues();
	}
	//---------------------------------------------------------------------------
	void WorldObjectPropertiesWindow::worldObjectSelectChanged(const WorldObjectEvent& _evt)
	{
		if(_evt.isSelected() && _evt.getWorld()->isActive())
			addObject( _evt.getWorldObject() );
		else
			removeObject( _evt.getWorldObject() );
	}
	//----------------------------------------------------------------------------
	void WorldObjectPropertiesWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
		{
			WorldObjectPropertiesWindowManager::getSingleton().setVisible(false);
		}
	}

} // namespace GUISystem
