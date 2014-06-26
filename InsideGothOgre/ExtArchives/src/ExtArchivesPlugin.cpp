#include "Precompiled.h"
#include "ExtArchivesPlugin.h"
#include "RFileSystem.h"
#include "RZip.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(ExtArchivesPlugin)
	GOTHOGRE_SET_MODULE_NAMES("ExtArchives", "ExtArchives")

	ExtArchivesPlugin::ExtArchivesPlugin()
	{
		mRFileSystemArchiveFactory = nullptr;
		mRZipArchiveFactory = nullptr;
	}
	//-------------------------------------------------------------------------------
	const String& ExtArchivesPlugin::getName() const
	{
		static String sExtArchivesPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sExtArchivesPluginName;
	}
	//-------------------------------------------------------------------------------
	void ExtArchivesPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		// Create objects
		mRFileSystemArchiveFactory		= new RFileSystemArchiveFactory;
		mRZipArchiveFactory				= new RZipArchiveFactory;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}
	//-------------------------------------------------------------------------------
	void ExtArchivesPlugin::initialise()
	{
		ArchiveManager& mgr = ArchiveManager::getSingleton();
		mgr.addArchiveFactory(mRFileSystemArchiveFactory);
		mgr.addArchiveFactory(mRZipArchiveFactory);
	}
	//-------------------------------------------------------------------------------
	void ExtArchivesPlugin::shutdown()
	{
		ArchiveManager& mgr = ArchiveManager::getSingleton();
		// There are no function ArchiveManager::removeArchiveFactory in Ogre3D!
		//mgr.removeArchiveFactory(mRFileSystemArchiveFactory);
		//mgr.removeArchiveFactory(mRZipArchiveFactory);
	}
	//-------------------------------------------------------------------------------
	void ExtArchivesPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		if(mRFileSystemArchiveFactory)
		{
			delete mRFileSystemArchiveFactory;
			mRFileSystemArchiveFactory = nullptr;
		}

		if(mRZipArchiveFactory)
		{
			delete mRZipArchiveFactory;
			mRZipArchiveFactory = nullptr;
		}

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre