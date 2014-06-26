#include "Precompiled.h"
#include "WorldObjectTreeWindowManager.h"
#include "WorldObjectTreeWindow.h"


//------------------------------------------------------------------------------------
GUISystem::WorldObjectTreeWindowManager* 
	Ogre::Singleton<GUISystem::WorldObjectTreeWindowManager>::ms_Singleton 
	= nullptr;
//------------------------------------------------------------------------------------


namespace GUISystem
{
	WorldObjectTreeWindowManager::WorldObjectTreeWindowManager()
		: mWindow( nullptr )
	{
		registerUISettingsListener();
	}
	//------------------------------------------------------------------------------------
	WorldObjectTreeWindowManager::~WorldObjectTreeWindowManager()
	{
		if(mWindow)
			delete mWindow;
	}
	//------------------------------------------------------------------------------------
	void WorldObjectTreeWindowManager::setVisible(bool _visible)
	{
		UISettings::getSingleton().setSetting<bool>("Visible", "WorldObjectTree", _visible);
	}
	//------------------------------------------------------------------------------------
	bool WorldObjectTreeWindowManager::isVisible() const
	{
		return (mWindow != nullptr);
	}
	//------------------------------------------------------------------------------------
	void WorldObjectTreeWindowManager::uiSettingsChanged( const ConfigSettingsEvent& _evt)
	{
		bool value = UISettings::getSingleton().getSetting<bool>("Visible", "WorldObjectTree", true);
		if(value)
		{
			if(!mWindow)
			{
				mWindow = new WorldObjectTreeWindow;
			}
		}
		else
		{
			if(mWindow)
			{
				delete mWindow;
				mWindow = nullptr;
			}
		}
	}
	
} // namespace GUISystem
