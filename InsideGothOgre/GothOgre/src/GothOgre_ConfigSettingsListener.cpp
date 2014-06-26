#include "GothOgre_Precompiled.h"
#include "GothOgre_ConfigSettings.h"
#include "GothOgre_ConfigSettingsListener.h"


namespace GothOgre
{
	//------------------------------------------------------------------------------
	ConfigSettingsListener::ConfigSettingsListener()
	{
		mConfigSettings = nullptr;
	}
	//------------------------------------------------------------------------------
	ConfigSettingsListener::~ConfigSettingsListener()
	{
		unregisterListener();
	}
	//------------------------------------------------------------------------------
	void ConfigSettingsListener::registerListener(ConfigSettings* _configSettings)
	{
		unregisterListener();
		mConfigSettings = _configSettings;
		mConfigSettings->_addListener(this);
	}
	//------------------------------------------------------------------------------
	void ConfigSettingsListener::unregisterListener()
	{
		if(mConfigSettings)
		{
			mConfigSettings->_removeListener(this);
			mConfigSettings = nullptr;
		}
	}

} // namespace GothOgre