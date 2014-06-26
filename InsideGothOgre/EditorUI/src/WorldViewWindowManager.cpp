#include "Precompiled.h"
#include "WorldViewWindowManager.h"
#include "WorldViewWindow.h"


//------------------------------------------------------------------------------------
GUISystem::WorldViewWindowManager* 
	Ogre::Singleton<GUISystem::WorldViewWindowManager>::ms_Singleton 
	= nullptr;
//------------------------------------------------------------------------------------


namespace GUISystem
{
	//----------------------------------------------------------------------------
	WorldViewWindowManager::WorldViewWindowManager()
		: mActiveIndex(-1), mAppExitAfterWindowClosing(false),
		  mSaveWorldTool(nullptr)

	{
		WorldManager::WorldIterator it = WorldManager::getSingleton().getWorldIterator();
		while(it.hasMoreElements())
		{
			World* world = it.getNext();
			WorldViewWindow* window = new WorldViewWindow(world);
			mWindows.push_back(window);
			if(world == WorldManager::getSingleton().getActiveWorld())
				mActiveIndex = mWindows.size() - 1;
		}

		registerWorldListener();
		registerWindowEventListener();
	}
	//----------------------------------------------------------------------------
	WorldViewWindowManager::~WorldViewWindowManager()
	{
		for(size_t i = 0; i != mWindows.size(); ++i)
			delete mWindows[i];
	}
	//----------------------------------------------------------------------------
	size_t WorldViewWindowManager::getNumWindows() const
	{
		return mWindows.size();
	}
	//----------------------------------------------------------------------------
	WorldViewWindow* WorldViewWindowManager::getWindow(size_t _index) const
	{
		return mWindows[_index];
	}
	//----------------------------------------------------------------------------
	size_t WorldViewWindowManager::getActiveWindowIndex() const
	{
		return mActiveIndex;
	}
	//----------------------------------------------------------------------------
	WorldViewWindow* WorldViewWindowManager::getActiveWindow() const
	{
		return (mActiveIndex != -1) ? mWindows[mActiveIndex] : nullptr;
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::setActiveWindowIndex(size_t _index)
	{
		WorldViewWindow* window = getWindow(_index);
		setActiveWindow(window);
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::setActiveWindow(WorldViewWindow* _window)
	{
		World* world = _window ? _window->getWorld() : nullptr;
		WorldManager::getSingleton().setActiveWorld(world);
	}
	//----------------------------------------------------------------------------
	bool WorldViewWindowManager::windowClosing()
	{
		for(size_t i = 0; i != mWindows.size(); ++i)
		{
			WorldViewWindow* window = mWindows[i];
			window->setClosing(true);
		}

		mAppExitAfterWindowClosing = false;
		_processWindowClosing();
		
		if(mWindows.empty())
			return true;
		
		mAppExitAfterWindowClosing = true;
		return false;
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::_processWindowClosing()
	{
		World* changedWorld = nullptr;
		size_t i = mWindows.size();
		while(i != 0)
		{
			WorldViewWindow* window = mWindows[--i];
			if(window->isClosing())
			{
				World* world = window->getWorld();
				if(!world->isChanged())
					WorldManager::getSingleton().destroyWorld(world);
				else
				{
					changedWorld = world;
					break;
				}
			}
		}

		if(!changedWorld)
		{
			if(mAppExitAfterWindowClosing)
				InputSystem::getSingleton().setExitFlag();
			return;
		}

		WorldManager::getSingleton().setActiveWorld(changedWorld);

		MessageBox* msgbox = new MessageBox;
		msgbox->addButton(MessageBoxButton::BUTTON1, "#{Save}");
		msgbox->addButton(MessageBoxButton::BUTTON2, "#{DontSave}");
		msgbox->addButton(MessageBoxButton::CANCEL);
		msgbox->setIcon(MessageBoxIcon::QUEST);
		msgbox->setMessage("#{ConfirmSaveChanges}", "1", changedWorld->getName());
		msgbox->eventChoice = MyGUI::newDelegate(this, &WorldViewWindowManager::notifyMessageBoxChoice);
	}
	//---------------------------------------------------------------------------
	void WorldViewWindowManager::notifyMessageBoxChoice(MessageBoxButton _button)
	{
		switch(_button)
		{
			case MessageBoxButton::CANCEL:
			{
				cancelWindowClosing();
				break;
			}
			case MessageBoxButton::BUTTON1: // Save
			{
				if(!mSaveWorldTool)
				{
					ToolManager* toolMgr = ToolManagerList::getSingleton().getToolManager("EditorToolManager");
					mSaveWorldTool = toolMgr->getTool("SaveWorld");
				}
				if(mSaveWorldTool)
				{
					mSaveWorldTool->activateTool();
					registerFrameListener();
				}
				break;
			}
			case MessageBoxButton::BUTTON2: // Don't save
			{
				WorldManager::getSingleton().destroyWorld(getActiveWindow()->getWorld());
				_processWindowClosing();
				break;
			}
		}
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::frameStarted(const FrameEvent& _evt)
	{
		if(!mSaveWorldTool || !mSaveWorldTool->isActive())
		{
			unregisterFrameListener();
			if(getActiveWindow()->getWorld()->isChanged())
			{
				cancelWindowClosing();
			}
			else
			{
				WorldManager::getSingleton().destroyWorld(getActiveWindow()->getWorld());
				_processWindowClosing();
			}
		}
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::cancelWindowClosing()
	{
		for(size_t i = 0; i != mWindows.size(); ++i)
		{
			WorldViewWindow* window = mWindows[i];
			window->setClosing(false);
		}
		mAppExitAfterWindowClosing = false;
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::worldCreated(const WorldEvent& _evt)
	{
		World* world = _evt.getWorld();
		WorldViewWindow* window = new WorldViewWindow(world);
		mWindows.push_back(window);
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::worldDestroyed(const WorldEvent& _evt)
	{
		World* world = _evt.getWorld();
		for(WindowList::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			WorldViewWindow* window = *it;
			if(window->getWorld() == world)
			{
				mWindows.erase(it);
				delete window;
				break;
			}
		}
	}
	//----------------------------------------------------------------------------
	void WorldViewWindowManager::worldActiveChanged(const WorldEvent& _evt)
	{
		if(!_evt.isActivated())
		{
			mActiveIndex = -1;
			return;
		}

		World* world = _evt.getWorld();
		for(size_t i = 0; i != mWindows.size(); ++i)
		{
			WorldViewWindow* window = mWindows[i];
			if(window->getWorld() == world)
			{
				mActiveIndex = i;
				break;
			}
		}
	}

} // namespace GUISystem