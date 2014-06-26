#include "Precompiled.h"
#include "DeleteTool.h"


namespace GUISystem
{
	//------------------------------------------------------------------------
	// DeleteTool
	//------------------------------------------------------------------------
	void DeleteTool::initConstants()
	{
		ROOT_MENU_ID      = "Tools";
		ROOT_MENU_CAPTION = "#{Tools}";
		MENU_ITEM_ID      = "Delete";
		MENU_ITEM_CAPTION = "#{Delete}";
		MENU_ITEM_DESC    = "#{Delete_Desc}";
		DEFAULT_HOTKEY    = KeyList(KeyCode::DEL);
		HOTKEY_CONFIG_KEY_NAME = "Delete";
	}
	//------------------------------------------------------------------------
	DeleteTool::DeleteTool()
	{
		initConstants();

		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem( ROOT_MENU_ID, MenuItemType::Popup, ROOT_MENU_CAPTION );
		mb->insertItem( MENU_ITEM_ID, MenuItemType::Normal, MENU_ITEM_CAPTION, ROOT_MENU_ID );
		mb->setItemDescription( MENU_ITEM_ID, MENU_ITEM_DESC );
		mb->insertHotkey( MENU_ITEM_ID, HOTKEY_CONFIG_KEY_NAME, "Editor", DEFAULT_HOTKEY );
		mb->attachTool( MENU_ITEM_ID, this);

		updateMenuItems();
		registerWorldListener();
		registerWorldObjectListener();
	}
	//------------------------------------------------------------------------
	DeleteTool::~DeleteTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem( MENU_ITEM_ID );
		mb->removeItem( ROOT_MENU_ID );
	}
	//------------------------------------------------------------------------
	ToolOperation* DeleteTool::activate(const Any&)
	{
		return new DeleteToolOperation;
	}
	//------------------------------------------------------------------------
	void DeleteTool::worldActiveChanged(const WorldEvent& _evt)
	{
		updateMenuItems();
	}
	//------------------------------------------------------------------------
	void DeleteTool::worldObjectSelectChanged(const WorldObjectEvent& _evt)
	{
		updateMenuItems();
	}
	//------------------------------------------------------------------------
	void DeleteTool::updateMenuItems()
	{
		bool enable = false;
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(world)
		{
			World::SelectionIterator it = world->getSelectionIterator();
			if(it.hasMoreElements())
				enable = true;
		}
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemEnabled("Delete", enable);
	}




	//------------------------------------------------------------------------
	// DeleteToolOperation
	//------------------------------------------------------------------------
	DeleteToolOperation::DeleteToolOperation()
	{
		mWorld = WorldManager::getSingleton().getActiveWorld();
		if(!mWorld)
		{
			deactivateTool();
			return;
		}
		
		size_t numSelected = 0;
		World::SelectionIterator it = mWorld->getSelectionIterator();
		while(it.hasMoreElements())
		{
			WorldObject* obj = it.getNext();
			++numSelected;
		}

		if(numSelected == 0)
		{
			deactivateTool();
			return;
		}

		MessageBox* msgbox = new MessageBox;
		msgbox->addButton(MessageBoxButton::YES);
		msgbox->addButton(MessageBoxButton::NO);
		msgbox->setIcon(MessageBoxIcon::QUEST);
		msgbox->setMessage("#{ConfirmDeleteObjects}", "1", StrConv::toString(numSelected));
		msgbox->eventChoice = MyGUI::newDelegate(this, &DeleteToolOperation::notifyMessageBoxChoice);
	}
	//------------------------------------------------------------------------
	DeleteToolOperation::~DeleteToolOperation()
	{
	}
	//------------------------------------------------------------------------
	void DeleteToolOperation::notifyMessageBoxChoice(MessageBoxButton _button)
	{
		if(_button == MessageBoxButton::YES)
			performDelete();

		deactivateTool();
	}
	//------------------------------------------------------------------------
	void DeleteToolOperation::performDelete()
	{
		World::SelectionIterator it = mWorld->getSelectionIterator();
		while(it.hasMoreElements())
		{
			WorldObject* obj = it.getNext();
			obj->getParent()->removeChild(obj);
		}
	}



	//------------------------------------------------------------------------
	// DeleteToolFactory
	//------------------------------------------------------------------------
	DeleteToolFactory::DeleteToolFactory()
		: ToolFactory("DeleteTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------
	Tool* DeleteToolFactory::createTool()
	{
		return new DeleteTool;
	}

} // namespace GUISystem
