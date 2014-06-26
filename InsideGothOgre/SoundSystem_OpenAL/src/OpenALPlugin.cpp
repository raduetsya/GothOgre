#include "Precompiled.h"
#include "OpenALPlugin.h"
#include "PcmWavOpenALPlayer.h"
#include "LibSndFileOpenALPlayer.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(OpenALPlugin)
	GOTHOGRE_SET_MODULE_NAMES("SoundSystem_OpenAL", "SoundAL")
	
	OpenALPlugin::OpenALPlugin()
	{
		mOpenALPlayer = nullptr;
	}
	
	const String& OpenALPlugin::getName() const
	{
		static String sPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sPluginName;
	}

	void OpenALPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		//mOpenALPlayer = new PcmWavOpenALPlayer;
		mOpenALPlayer = new LibSndFileOpenALPlayer;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void OpenALPlugin::initialise()
	{
	}

	void OpenALPlugin::shutdown()
	{
	}

	void OpenALPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		delete mOpenALPlayer;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre