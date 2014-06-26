#include "Precompiled.h"
#include "InputOISPlugin.h"
#include "OISInputSupport.h"
#include "WinOISInputSupport.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(InputOISPlugin)
	GOTHOGRE_SET_MODULE_NAMES("InputSystem_OIS", "InputOIS")

	InputOISPlugin::InputOISPlugin()
	{
		mInputSupport = nullptr;
	}
	
	const String& InputOISPlugin::getName() const
	{
		static String sName = GOTHOGRE_LONG_MODULE_NAME;
		return sName;
	}

	void InputOISPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		mInputSupport = new WinOISInputSupport;
#else
		mInputSupport = new OISInputSupport;
#endif

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void InputOISPlugin::initialise()
	{
	}

	void InputOISPlugin::shutdown()
	{
	}

	void InputOISPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		if(mInputSupport)
		{
			delete mInputSupport;
			mInputSupport = nullptr;
		}

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre