#include "Precompiled.h"
#include "SceneManagerExImplPlugin.h"
#include "SceneManagerExImplFactory.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(SceneManagerExImplPlugin)
	GOTHOGRE_SET_MODULE_NAMES("ExtSceneManager", "ExtSceneMgr")

	SceneManagerExImplPlugin::SceneManagerExImplPlugin()
	{
		mSceneManagerExImplFactory = nullptr;
	}
	//-------------------------------------------------------------------------------
	const String& SceneManagerExImplPlugin::getName() const
	{
		static String sPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sPluginName;
	}
	//-------------------------------------------------------------------------------
	void SceneManagerExImplPlugin::install()
	{
	}
	//-------------------------------------------------------------------------------
	void SceneManagerExImplPlugin::initialise()
	{
		GOTHOGRE_INFO("Initialising the \"" << getName() << "\" plugin.");

		// Create objects
		mSceneManagerExImplFactory = new SceneManagerExImplFactory;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin initialised successfully.");
	}
	//-------------------------------------------------------------------------------
	void SceneManagerExImplPlugin::shutdown()
	{
		GOTHOGRE_INFO("Shutting down the \"" << getName() << "\" plugin.");

		if(mSceneManagerExImplFactory)
		{
			delete mSceneManagerExImplFactory;
			mSceneManagerExImplFactory = nullptr;
		}

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin shutted down successfully.");
	}
	//-------------------------------------------------------------------------------
	void SceneManagerExImplPlugin::uninstall()
	{
	}

} // namespace GothOgre