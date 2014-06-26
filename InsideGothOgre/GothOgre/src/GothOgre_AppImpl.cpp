#include "GothOgre_Precompiled.h"
#include "GothOgre_AppImpl.h"


namespace GothOgre
{
	//----------------------------------------------------------------------
	AppImpl::AppImpl()
	{
		mInitOk = false;

		mCommandLine = nullptr;
		mLogSystem = nullptr;
		mUnicodeSupportRegistration = nullptr;
		mEncodingSettings = nullptr;
		mGraphicSettings = nullptr;
		mSoundSettings = nullptr;
		mResourceLocationScanner = nullptr;
		mInputSupportRegistration = nullptr;
		mInputSystem = nullptr;
		mUIModeManager = nullptr;
		mProgress = nullptr;
		mFrameStats = nullptr;

		mSystemTime = nullptr;
		mWorldTime = nullptr;

		mSkyDefManager = nullptr;
		mSkyDefManagerEx = nullptr;
		mSkySystem = nullptr;
		mPortalSystem = nullptr;

		mSoundDefManager = nullptr;
		mSoundDefManagerEx = nullptr;
		mSoundSystem = nullptr;
		mSoundPlayerEnumerator = nullptr;

		mRoot = nullptr;
		mWindow = nullptr;

		mTextureManagerEx = nullptr;
		mMtlManager = nullptr;
		mMtlManagerEx = nullptr;
		mMeshManagerEx = nullptr;
		mSkyDefManager = nullptr;
		mSkyDefManagerEx = nullptr;

		mSceneManagerExEnumerator = nullptr;
		mZenManager = nullptr;
		mBaseWorldObjectFactory = nullptr;
		mRootWorldObjectFactory = nullptr;
		mWorldManager = nullptr;
	}
	//----------------------------------------------------------------------
	AppImpl::~AppImpl()
	{
	}
	//----------------------------------------------------------------------
	void AppImpl::initialise(int _argc, char** _argv)
	{
		initialise_CommandLine(_argc, _argv);
		initialise_after_CommandLine_before_Ogre();
		initialise_Ogre();
		initialise_after_Ogre();
		mInitOk = true;
	}
	//----------------------------------------------------------------------
	void AppImpl::initialise(const char* _cmdLine)
	{
		initialise_CommandLine(_cmdLine);
		initialise_after_CommandLine_before_Ogre();
		initialise_Ogre();
		initialise_after_Ogre();
		mInitOk = true;
	}
	//----------------------------------------------------------------------
	void AppImpl::initialise_CommandLine(int _argc, char** _argv)
	{
		mCommandLine = new CommandLine(_argc, _argv);
	}
	//----------------------------------------------------------------------
	void AppImpl::initialise_CommandLine(const char* _cmdLine)
	{
		mCommandLine = new CommandLine(_cmdLine);
	}
	//----------------------------------------------------------------------
	void AppImpl::initialise_after_CommandLine_before_Ogre()
	{
		// Must be first
		mSystemTime = new SystemTime;
		mLogSystem = new LogSystem("gothogre.log");
		
		// Initialising...
		GOTHOGRE_INFO("****************************************************************************");
		GOTHOGRE_INFO("*                   App: Initialising                                      *");
		GOTHOGRE_INFO("****************************************************************************");

		// Unicode support
		mUnicodeSupportRegistration = new UnicodeSupportRegistration;
		
		// Configuration files
		mEncodingSettings = new EncodingSettings("encoding.cfg");
		mGraphicSettings = new GraphicSettings("graphics.cfg");
		mSoundSettings = new SoundSettings("sound.cfg");
		mResourceLocationScanner = new ResourceLocationScanner("resources.int");
		
		// Input
		mInputSupportRegistration = new InputSupportRegistration;
		mInputSystem = new InputSystem;

		// New-styled resource managers
		mTextureManagerEx = new TextureManagerEx;
		mMtlManagerEx = new MtlManagerEx;
		mMeshManagerEx = new MeshManagerEx;
		mSkyDefManagerEx = new SkyDefManagerEx;
		mSoundDefManagerEx = new SoundDefManagerEx;

		// Sound
		mSoundPlayerEnumerator = new SoundPlayerEnumerator;
		mSoundSystem = new SoundSystem;

		// Skies
		mSkySystem = new SkySystem;

		// Portals
		mPortalSystem = new PortalSystem;

		// Scene & World
		mSceneManagerExEnumerator = new SceneManagerExEnumerator;
		mZenManager = new ZenManager;
		mBaseWorldObjectFactory = new BaseWorldObjectFactory;
		mRootWorldObjectFactory = new RootWorldObjectFactory;
		mWorldTime = new WorldTime;
		mWorldManager = new WorldManager;

		// User interface
		mUIModeManager = new UIModeManager(UIMode::INITIALISATION);
		mProgress = new Progress;
		mFrameStats = new FrameStats;
	}
	//----------------------------------------------------------------------
	void AppImpl::initialise_Ogre()
	{
		GOTHOGRE_INFO("Creating Ogre::Root and loading plugins, using pathes: ");

		String pluginsCfg = CommandLine::getSingleton().getAbsolutePath("plugins.int");
		String ogreCfg    = CommandLine::getSingleton().getAbsolutePath("ogre.cfg");
		String ogreLog    = CommandLine::getSingleton().getAbsolutePath("ogre.log");

		GOTHOGRE_INFO(pluginsCfg << " - path to plugins.cfg,");
		GOTHOGRE_INFO(ogreCfg << " - path to ogre.cfg.");

		mRoot = new Root(pluginsCfg, ogreCfg, ogreLog);
		GOTHOGRE_ASSERT(mRoot != nullptr, "Failed to create Ogre::Root.");
		GOTHOGRE_INFO("Ogre::Root created successfully, plugins loaded.");

		GOTHOGRE_INFO("Looking for a valid Ogre's saved configuration from a previous run:");
		if (mRoot->restoreConfig())
		{
			GOTHOGRE_INFO("Valid Ogre's configuration was found.");
		}
		else
		{
			GOTHOGRE_INFO("Valid Ogre's configuration was not found, displaying a dialog asking the user to choose system settings.");
			if (mRoot->showConfigDialog()) 
			{
				GOTHOGRE_INFO("The user clicked 'OK'.");
			}
			else
			{
				GOTHOGRE_INFO("The user clicked 'Cancel'.");
				return;
			}
		}

		GOTHOGRE_INFO("Creating a rendering window");
		mWindow = mRoot->initialise(true);
		GOTHOGRE_ASSERT(mWindow != nullptr, "Could not create RenderWindow");
		GOTHOGRE_INFO("Rendering window created successfully");

		setWindowIcon();
	}
	//----------------------------------------------------------------------
	void AppImpl::setWindowIcon()
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// load icon from the application's Win32 resources.
		// name of icon resource: "appicon"
		HINSTANCE instance = ::GetModuleHandleA(NULL);
		HICON hIcon = ::LoadIconA(instance, "appicon");
		if (hIcon)
		{
			HWND hwnd;
			mWindow->getCustomAttribute("WINDOW", &hwnd);
			::SendMessageA( hwnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon );
			::SendMessageA( hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon );
		}
#endif
	}
	//----------------------------------------------------------------------
	void AppImpl::initialise_after_Ogre()
	{
		// Load the configuration files
		mEncodingSettings->initialise();
		mGraphicSettings->initialise();
		mSoundSettings->initialise();
		mResourceLocationScanner->initialise();

		// Input system (required plugins to be loaded first).
		mInputSystem->initialise(mWindow);

		// Resource managers for new resource types
		// (must be created after Ogre because the resource managers 
		// must be registered in Ogre::ResourceGroupManager)
		mMtlManager = new MtlManager;
		mSoundDefManager = new SoundDefManager;
		mSkyDefManager = new SkyDefManager;
		
		// Register factories in Ogre for new movable object types
		mSoundSystem->initialise();
		mSkySystem->initialise();
		mPortalSystem->initialise();

		// Set UI mode.
		mUIModeManager->setUIMode(UIMode::EDITOR);

		// Initialised!
		GOTHOGRE_INFO("****************************************************************************");
		GOTHOGRE_INFO("*                   App: Successfully Initialised                          *");
		GOTHOGRE_INFO("****************************************************************************");

		// Test
		test();
	}
	//----------------------------------------------------------------------
	void AppImpl::shutdown()
	{
		shutdown_before_Ogre();
		shutdown_Ogre();
		shutdown_after_Ogre();
	}
	//----------------------------------------------------------------------
	void AppImpl::shutdown_before_Ogre()
	{
		// Shutting down...
		GOTHOGRE_INFO("****************************************************************************");
		GOTHOGRE_INFO("*                   App: Shutting down                                     *");
		GOTHOGRE_INFO("****************************************************************************");

		// Set UI mode.
		if(mUIModeManager)
			mUIModeManager->setUIMode(UIMode::SHUTDOWN);
		
		// Destroy all worlds.
		if(mWorldManager)
			mWorldManager->destroyAllWorlds();

		// Unregister movable object factories
		if(mSoundSystem)
			mSoundSystem->shutdown();
		if(mSkySystem)
			mSkySystem->shutdown();
		if(mPortalSystem)
			mPortalSystem->shutdown();

		// Destroy resource managers for new resource types.
		delete mMtlManager;
		delete mSkyDefManager;
		delete mSoundDefManager;

		// Shutdown input system
		if(mInputSystem)
			mInputSystem->shutdown();

		// Save the configuration files
		if(mEncodingSettings)
			mEncodingSettings->shutdown();

		if(mGraphicSettings)
			mGraphicSettings->shutdown();

		if(mSoundSettings)
			mSoundSettings->shutdown();

		if(mResourceLocationScanner)
			mResourceLocationScanner->shutdown();
	}
	//----------------------------------------------------------------------
	void AppImpl::shutdown_Ogre()
	{
		if(!mRoot)
			return;

		GOTHOGRE_INFO("Shutting down Ogre");
		
		// Unload archives to prevent destructor ArchiveManager::~ArchiveManager
		// from doint it. The destructor cannot safe delete archives
		// which uses each others.
		vector<Archive*>::type archives; 
		ArchiveManager::ArchiveMapIterator it = ArchiveManager::getSingleton().getArchiveIterator();
		while(it.hasMoreElements())
		{
			archives.push_back(it.getNext());
		}
		while(!archives.empty())
		{
			ArchiveManager::getSingleton().unload(archives.back());
			archives.pop_back();
		}

		// Destroy window and Root.
		mWindow = nullptr;
		OGRE_DELETE mRoot;
		mRoot = nullptr;
		GOTHOGRE_INFO("Ogre shutted down successfully");
	}
	//----------------------------------------------------------------------
	void AppImpl::shutdown_after_Ogre()
	{
		// User interface
		delete mProgress;
		delete mFrameStats;
		delete mUIModeManager;

		// Scene & World
		delete mWorldManager;
		delete mWorldTime;
		delete mBaseWorldObjectFactory;
		delete mRootWorldObjectFactory;
		delete mZenManager;
		delete mSceneManagerExEnumerator;
		
		// Sound
		delete mSoundSystem;
		delete mSoundPlayerEnumerator;

		// Skies
		delete mSkySystem;

		// Portals
		delete mPortalSystem;

		// New-styled resource managers
		delete mTextureManagerEx;
		delete mMtlManagerEx;
		delete mMeshManagerEx;
		delete mSkyDefManagerEx;
		delete mSoundDefManagerEx;

		// Input
		delete mInputSystem;
		delete mInputSupportRegistration;

		// Configuration files
		delete mEncodingSettings;
		delete mSoundSettings;
		delete mGraphicSettings;
		delete mResourceLocationScanner;

		// Unicode support
		delete mUnicodeSupportRegistration;

		// Shutted down!
		GOTHOGRE_INFO("****************************************************************************");
		GOTHOGRE_INFO("*                   App: Successfully shutted down                         *");
		GOTHOGRE_INFO("****************************************************************************");

		// Must be last
		delete mLogSystem;
		delete mSystemTime;
	}
	//------------------------------------------------------------------------
	void AppImpl::renderingLoop()
	{
		// initialise all render targets
		Root::getSingleton().getRenderSystem()->_initRenderTargets();

		// infinite loop
		while (true)
		{
			Ogre::WindowEventUtilities::messagePump();

			//RenderWindow* win = Ogre::Root::getSingleton().getAutoCreatedWindow();
			//if (win->isActive() == false)
			//	win->setActive(true);

			if (!Root::getSingleton().renderOneFrame())
			{
				GOTHOGRE_INFO("Ogre::Root::renderOneFrame returned false, exiting from the application");
				break;
			}

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			// sleep to prevent other threads from stopping
			::Sleep(0);
#endif
		}
	}

} // namespace GothOgre