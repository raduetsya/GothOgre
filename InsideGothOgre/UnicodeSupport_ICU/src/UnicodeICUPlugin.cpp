#include "Precompiled.h"
#include "ICUUnicodeSupport.h"
#include "UnicodeICUPlugin.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(UnicodeICUPlugin)
	GOTHOGRE_SET_MODULE_NAMES("UnicodeSupport_ICU", "UnicodeICU")

	UnicodeICUPlugin::UnicodeICUPlugin()
	{
		mICUUnicodeSupport = nullptr;
	}
	
	const String& UnicodeICUPlugin::getName() const
	{
		static String sName = GOTHOGRE_LONG_MODULE_NAME;
		return sName;
	}

	void UnicodeICUPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		mICUUnicodeSupport = new ICUUnicodeSupport;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void UnicodeICUPlugin::initialise()
	{
	}

	void UnicodeICUPlugin::shutdown()
	{
	}

	void UnicodeICUPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling \"" << getName() << "\" plugin.");
		
		if(mICUUnicodeSupport)
			delete mICUUnicodeSupport;
		
		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

} // namespace GothOgre
