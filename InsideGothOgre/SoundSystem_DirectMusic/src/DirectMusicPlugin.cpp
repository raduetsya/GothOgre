#include "Precompiled.h"
#include "DirectMusicPlugin.h"
#include "DirectMusicPlayer.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(DirectMusicPlugin)
	GOTHOGRE_SET_MODULE_NAMES("SoundSystem_DirectMusic", "SoundDM")
	
	DirectMusicPlugin::DirectMusicPlugin()
	{
		mDirectMusicPlayer = nullptr;
	}
	
	const String& DirectMusicPlugin::getName() const
	{
		static String sPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sPluginName;
	}

	void DirectMusicPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		mDirectMusicPlayer = new DirectMusicPlayer;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void DirectMusicPlugin::initialise()
	{
	}

	void DirectMusicPlugin::shutdown()
	{
	}

	void DirectMusicPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		delete mDirectMusicPlayer;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre