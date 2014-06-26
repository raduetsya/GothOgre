#include "Precompiled.h"
#include "WorldObjectTreeWindow.h"
#include "WorldObjectTreeWindowManager.h"


namespace GUISystem
{
	//----------------------------------------------------------------------------
	WorldObjectTreeWindow::WorldObjectTreeWindow()
	{
		initialiseByAttributes(this);
		fillTreeControl();
		mTreeControl->eventTreeNodeSelected = MyGUI::newDelegate(this, &WorldObjectTreeWindow::notifyTreeControlNodeSelected);
		
		registerWorldListener();
		registerWorldObjectListener();
	}
	//----------------------------------------------------------------------------
	WorldObjectTreeWindow::~WorldObjectTreeWindow()
	{
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
		{
			WorldObjectTreeWindowManager::getSingleton().setVisible(false);
		}
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::fillTreeControl()
	{
		mWorldObjectNodeMap.clear();
		mTreeControl->getRoot()->removeAll();
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(world)
			addNode(world->getRootWorldObject().get());
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::addNode(WorldObject* _worldObject)
	{
		WorldObject* parentObj = _worldObject->getParent();
		if(parentObj)
		{
			TreeControl::Node* parentNode = getNodeByWorldObject(parentObj);
			if(parentNode)
			{
				TreeControl::Node* node = new TreeControl::Node(_worldObject->getCombinedName(), parentNode);
				node->setData( (MyGUI::Any) _worldObject);
				mWorldObjectNodeMap.insert(std::make_pair(_worldObject, node));
			}
		}

		WorldObject::ChildIterator childIt = _worldObject->getChildIterator();
		while(childIt.hasMoreElements())
		{
			WorldObject* childObj = childIt.getNext().get();
			addNode(childObj);
		}
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::removeNode(WorldObject* _worldObject)
	{
		WorldObject::ChildIterator childIt = _worldObject->getChildIterator();
		while(childIt.hasMoreElements())
		{
			WorldObject* childObj = childIt.getNext().get();
			removeNode(childObj);
		}

		TreeControl::Node* node = getNodeByWorldObject(_worldObject);
		if(node && node != mTreeControl->getRoot())
		{
			node->getParent()->remove(node);
			mWorldObjectNodeMap.erase(_worldObject);
		}
	}
	//----------------------------------------------------------------------------
	WorldObject* WorldObjectTreeWindow::getWorldObjectByNode(TreeControl::Node* _node) const
	{
		return *(_node->getData<WorldObject*>());
	}
	//----------------------------------------------------------------------------
	TreeControl::Node* WorldObjectTreeWindow::getNodeByWorldObject(WorldObject* _worldObject) const
	{
		WorldObjectNodeMap::const_iterator it = mWorldObjectNodeMap.find(_worldObject);
		if(it != mWorldObjectNodeMap.end())
			return it->second;

		if(!_worldObject->getParent())
			return mTreeControl->getRoot();

		return nullptr;
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::worldObjectAdded(const WorldObjectEvent& _evt)
	{
		addNode(_evt.getWorldObject());
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::worldObjectRemoved(const WorldObjectEvent& _evt)
	{
		removeNode(_evt.getWorldObject());
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::worldObjectChanged(const WorldObjectEvent& _evt)
	{
		WorldObject* obj = _evt.getWorldObject();
		TreeControl::Node* node = getNodeByWorldObject(obj);
		if(node)
			node->setText(obj->getCombinedName());
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::worldObjectSelectChanged(const WorldObjectEvent& _evt)
	{
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::worldActiveChanged(const WorldEvent& _evt)
	{
		fillTreeControl();
	}
	//----------------------------------------------------------------------------
	void WorldObjectTreeWindow::notifyTreeControlNodeSelected(TreeControl* _sender, TreeControl::Node* _node)
	{
		if(_node)
		{
			WorldObject* obj = getWorldObjectByNode(_node);
			if(obj)
			{
				obj->getWorld()->deselectAll();
				obj->select();
			}
		}
	}

} // namespace GUISystem
