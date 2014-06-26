#include "Precompiled.h"
#include "FileFormatsPlugin.h"
#include "3dsFileLoader.h"
#include "MshFileLoader.h"
#include "MeshAndBspLoader.h"
#include "MrmFileLoader.h"
#include "DefaultTextureFileFormats.h"
#include "MatLibIniLoader.h"
#include "SkyFileLoader.h"


namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(FileFormatsPlugin)
	GOTHOGRE_SET_MODULE_NAMES("FileFormats", "FileFormats")

	FileFormatsPlugin::FileFormatsPlugin()
	{
		m3dsFileLoader = nullptr;
		mMshFileLoader = nullptr;
		mMeshAndBspLoader = nullptr;
		mMrmFileLoader = nullptr;
		mTgaFileLoader = nullptr;
		mMatLibIniLoader = nullptr;
		mSkyFileLoader = nullptr;
	}
	
	const String& FileFormatsPlugin::getName() const
	{
		static String sPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sPluginName;
	}

	void FileFormatsPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		m3dsFileLoader = new _3dsFileLoader;
		mMshFileLoader = new MshFileLoader;
		mMeshAndBspLoader = new MeshAndBspLoader;
		mMrmFileLoader = new MrmFileLoader;
		mTgaFileLoader = new TgaFileLoader;
		mMatLibIniLoader = new MatLibIniLoader;
		mSkyFileLoader = new SkyFileLoader;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void FileFormatsPlugin::initialise()
	{
	}

	void FileFormatsPlugin::shutdown()
	{
	}

	void FileFormatsPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		if(m3dsFileLoader)
		{
			delete m3dsFileLoader;
			m3dsFileLoader = nullptr;
		}
		if(mMshFileLoader)
		{
			delete mMshFileLoader;
			mMshFileLoader = nullptr;
		}
		if(mMeshAndBspLoader)
		{
			delete mMeshAndBspLoader;
			mMeshAndBspLoader = nullptr;
		}
		if(mMrmFileLoader)
		{
			delete mMrmFileLoader;
			mMrmFileLoader = nullptr;
		}
		if(mTgaFileLoader)
		{
			delete mTgaFileLoader;
			mTgaFileLoader = nullptr;
		}

		if(mMatLibIniLoader)
		{
			delete mMatLibIniLoader;
			mMatLibIniLoader = nullptr;
		}

		if(mSkyFileLoader)
		{
			delete mSkyFileLoader;
			mSkyFileLoader = nullptr;
		}

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre