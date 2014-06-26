#include "Precompiled.h"
#include "WorldObjectPropertiesWindowManager.h"
#include "WorldObjectPropertiesWindow.h"


//------------------------------------------------------------------------------------
GUISystem::WorldObjectPropertiesWindowManager* 
	Ogre::Singleton<GUISystem::WorldObjectPropertiesWindowManager>::ms_Singleton 
	= nullptr;
//------------------------------------------------------------------------------------


namespace GUISystem
{
	WorldObjectPropertiesWindowManager::WorldObjectPropertiesWindowManager()
		: mWindow( nullptr )
	{
		registerUISettingsListener();
	}
	//------------------------------------------------------------------------------------
	WorldObjectPropertiesWindowManager::~WorldObjectPropertiesWindowManager()
	{
		if(mWindow)
			delete mWindow;
	}
	//------------------------------------------------------------------------------------
	void WorldObjectPropertiesWindowManager::setVisible(bool _visible)
	{
		UISettings::getSingleton().setSetting<bool>("Visible", "WorldObjectProperties", _visible);
	}
	//------------------------------------------------------------------------------------
	bool WorldObjectPropertiesWindowManager::isVisible() const
	{
		return (mWindow != nullptr);
	}
	//------------------------------------------------------------------------------------
	void WorldObjectPropertiesWindowManager::uiSettingsChanged( const ConfigSettingsEvent& _evt)
	{
		bool value = UISettings::getSingleton().getSetting<bool>("Visible", "WorldObjectProperties", true);
		if(value)
		{
			if(!mWindow)
			{
				mWindow = new WorldObjectPropertiesWindow;
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
