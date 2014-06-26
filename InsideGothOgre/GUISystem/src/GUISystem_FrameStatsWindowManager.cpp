#include "GUISystem_Precompiled.h"
#include "GUISystem_FrameStatsWindowManager.h"
#include "GUISystem_FrameStatsWindow.h"
#include "GUISystem_UIInitPriority.h"

//------------------------------------------------------------------------------------
GUISystem::FrameStatsWindowManager* 
	Ogre::Singleton<GUISystem::FrameStatsWindowManager>::ms_Singleton 
	= nullptr;
//------------------------------------------------------------------------------------


namespace GUISystem
{
	//------------------------------------------------------------------------------------
	FrameStatsWindowManager::FrameStatsWindowManager()
	{
		mWindow = nullptr;
		mPanel = nullptr;
		mUIModeEntered = false;

		registerUIModeListener( UIInitPriority::AFTER_SKIN );
		registerUISettingsListener();
	}
	//------------------------------------------------------------------------------------
	FrameStatsWindowManager::~FrameStatsWindowManager()
	{
		destroyWidgets();
	}
	//------------------------------------------------------------------------------------
	void FrameStatsWindowManager::setVisible(bool _visible)
	{
		UISettings::getSingleton().setSetting<bool>("Visible", "FrameStatsPanel", _visible);
	}
	//------------------------------------------------------------------------------------
	bool FrameStatsWindowManager::isVisible() const
	{
		return mVisible;
	}
	//------------------------------------------------------------------------------------
	void FrameStatsWindowManager::setMovable(bool _movable)
	{
		UISettings::getSingleton().setSetting<bool>("Movable", "FrameStatsPanel", _movable);
	}
	//------------------------------------------------------------------------------------
	bool FrameStatsWindowManager::isMovable() const
	{
		return mMovable;
	}
	//------------------------------------------------------------------------------------
	void FrameStatsWindowManager::uiModeEntered(const UIModeEvent& _evt)
	{
		if(_evt.getNewUIMode() != UIMode::SHUTDOWN)
		{
			mUIModeEntered = true;
			createWidgets();
		}
	}
	//------------------------------------------------------------------------------------
	void FrameStatsWindowManager::uiModeExited(const UIModeEvent& _evt)
	{
		destroyWidgets();
		mUIModeEntered = false;
	}
	//------------------------------------------------------------------------------------
	void FrameStatsWindowManager::uiSettingsChanged( const ConfigSettingsEvent& _evt)
	{
		mVisible = UISettings::getSingleton().getSetting<bool>("Visible", "FrameStatsPanel", true);
		mMovable = UISettings::getSingleton().getSetting<bool>("Movable", "FrameStatsPanel", false);

		if(mUIModeEntered)
			createWidgets();
	}
	//------------------------------------------------------------------------------------
	void FrameStatsWindowManager::destroyWidgets()
	{
		if(mPanel)
		{
			delete mPanel;
			mPanel = nullptr;
		}

		if(mWindow) 
		{
			delete mWindow;
			mWindow = nullptr;
		}
	}
	//------------------------------------------------------------------------------------
	void FrameStatsWindowManager::createWidgets()
	{
		if(mVisible)
		{
			if(mMovable)
			{
				if(mPanel)
				{
					delete mPanel;
					mPanel = nullptr;
				}
				if(!mWindow)
				{
					mWindow = new FrameStatsWindow;
				}
			}
			else // not movable
			{
				if(mWindow)
				{
					delete mWindow;
					mWindow = nullptr;
				}
				if(!mPanel)
				{
					mPanel = new FrameStatsPanel;
				}
			}
		}
		else // not visible
		{
			if(mPanel)
			{
				delete mPanel;
				mPanel = nullptr;
			}
			if(mWindow)
			{
				delete mWindow;
				mWindow = nullptr;
			}
		}
	}

} // namespace GUISystem
