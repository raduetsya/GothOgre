#include "Precompiled.h"
#include "ScriptProcessingPlugin.h"
#include "MusicDatLoader.h"
#include "SfxDatLoader.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(ScriptProcessingPlugin)
	GOTHOGRE_SET_MODULE_NAMES("ScriptProcessing", "Scripting")
	
	ScriptProcessingPlugin::ScriptProcessingPlugin()
	{
		mMusicDatLoader = nullptr;
		mSfxDatLoader = nullptr;
	}
	
	const String& ScriptProcessingPlugin::getName() const
	{
		static String sPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sPluginName;
	}

	void ScriptProcessingPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		mMusicDatLoader = new MusicDatLoader;
		mSfxDatLoader = new SfxDatLoader;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void ScriptProcessingPlugin::initialise()
	{
	}

	void ScriptProcessingPlugin::shutdown()
	{
	}

	void ScriptProcessingPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		delete mMusicDatLoader;
		delete mSfxDatLoader;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre