#include "Precompiled.h"
#include "WinUnicodeSupport.h"
#include "UnicodeWinPlugin.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(UnicodeWinPlugin)
	GOTHOGRE_SET_MODULE_NAMES("UnicodeSupport_Win", "UnicodeWin")

	UnicodeWinPlugin::UnicodeWinPlugin()
	{
		mWinUnicodeSupport = nullptr;
	}
	
	const String& UnicodeWinPlugin::getName() const
	{
		static String sName = GOTHOGRE_LONG_MODULE_NAME;
		return sName;
	}

	void UnicodeWinPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		mWinUnicodeSupport = new WinUnicodeSupport;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void UnicodeWinPlugin::initialise()
	{
	}

	void UnicodeWinPlugin::shutdown()
	{
	}

	void UnicodeWinPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		if(mWinUnicodeSupport)
			delete mWinUnicodeSupport;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre