#include "Precompiled.h"
#include "EditorUIPlugin.h"

#include "EditorInitSkin.h"
#include "EditorInitBeforeTools.h"
#include "EditorInitTools.h"

#include "QuitTool.h"
#include "NewWorldTool.h"
#include "LoadWorldTool.h"
#include "LoadWorld3dsTool.h"
#include "FrameStatsTool.h"
#include "FrameStatsConfigTool.h"
#include "SelectTool.h"
#include "FreeLookTool.h"
#include "PlaySoundTool.h"
#include "RenderModeTool.h"
#include "WorldViewTool.h"
#include "WorldObjectPropertiesTool.h"
#include "WorldObjectTreeTool.h"
//#include "SunSkyTool.h"
//#include "EnvironmentTool.h"
#include "AboutGothOgreTool.h"
#include "CameraPositionTool.h"
#include "DeleteTool.h"
#include "GraphicSettingsTool.h"


namespace GUISystem
{
	//---------------------------------------------------------------------------
	GOTHOGRE_PLUGIN_ENTRY_POINT(EditorUIPlugin)
	GOTHOGRE_SET_MODULE_NAMES("EditorUI", "EditorUI")
	//---------------------------------------------------------------------------
	EditorUIPlugin::EditorUIPlugin()
	{
		mInitSkin = nullptr;
		mInitBeforeTools = nullptr;
		mInitTools  = nullptr;
	}
	//---------------------------------------------------------------------------
	const String& EditorUIPlugin::getName() const
	{
		static String sPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sPluginName;
	}
	//---------------------------------------------------------------------------
	void EditorUIPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		// Create initialisers
		mInitSkin = new EditorInitSkin;
		mInitBeforeTools = new EditorInitBeforeTools;
		mInitTools = new EditorInitTools;

		// Create tool factories

		// Menu "File"
		mToolFactories.push_back( new NewWorldToolFactory );
		mToolFactories.push_back( new LoadWorldToolFactory );
		mToolFactories.push_back( new LoadWorld3dsToolFactory );
		mToolFactories.push_back( new QuitToolFactory );

		// Menu "Tools"
		mToolFactories.push_back( new SelectToolFactory );
		mToolFactories.push_back( new FreeLookToolFactory );
		mToolFactories.push_back( new CameraPositionToolFactory );
		mToolFactories.push_back( new DeleteToolFactory );

		// Menu "Sounds"
		mToolFactories.push_back( new PlaySoundToolFactory );

		// Menu "Settings"
		mToolFactories.push_back( new RenderModeToolFactory );
		mToolFactories.push_back( new FrameStatsConfigToolFactory );
		mToolFactories.push_back( new GraphicSettingsToolFactory );

		// Menu "Windows"
		mToolFactories.push_back( new WorldViewToolFactory );
		mToolFactories.push_back( new FrameStatsToolFactory );
		mToolFactories.push_back( new WorldObjectPropertiesToolFactory );
		mToolFactories.push_back( new WorldObjectTreeToolFactory );

		// Menu "Help"
		mToolFactories.push_back( new AboutGothOgreToolFactory );

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}

	void EditorUIPlugin::initialise()
	{
	}
	//---------------------------------------------------------------------------
	void EditorUIPlugin::shutdown()
	{
	}
	//---------------------------------------------------------------------------
	void EditorUIPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		for(ToolFactories::iterator it = mToolFactories.begin();
			it != mToolFactories.end(); ++it)
		{
			ToolFactory* toolFactory = *it;
			delete toolFactory;
		}
		mToolFactories.clear();

		delete mInitTools;
		mInitTools = nullptr;

		delete mInitBeforeTools;
		mInitBeforeTools = nullptr;

		delete mInitSkin;
		mInitSkin = nullptr;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GUISystem
