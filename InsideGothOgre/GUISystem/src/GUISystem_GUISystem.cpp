#include "GUISystem_Precompiled.h"
#include "GUISystem_GUISystem.h"
#include "GUISystem_UIInitPriority.h"
#include "GUISystem_MyGUIInput.h"
#include "GUISystem_ToolManagerList.h"
#include "GUISystem_HotkeyManagerList.h"
#include "GUISystem_MenuBarList.h"
#include "GUISystem_ProfilerViewport.h"
#include "GUISystem_ControlSettings.h"
#include "GUISystem_UISettings.h"
#include "GUISystem_WorldEditSettings.h"
#include "GUISystem_ProgressDialogManager.h"
#include "GUISystem_FrameStatsWindowManager.h"
#include "GUISystem_UserTagManager.h"
#include "GUISystem_TreeControl.h"
#include "GUISystem_TreeControlItem.h"


namespace GUISystem
{
	//-----------------------------------------------------------------------
	GOTHOGRE_SET_MODULE_NAMES("GUISystem", "GUISystem")
	static GUISystem sTheGUISystem; // The single instance of the class 
	                                // - it's created on loading of this dll.
	

	//-----------------------------------------------------------------------
	GUISystem::GUISystem()
		: mInitialised(false), mWindow(nullptr), 
		  mUISceneManager(nullptr), mUICamera(nullptr), mUIViewport(nullptr), 
		  mMyGUIInput(nullptr), mToolManagerList(nullptr),
		  mHotkeyManagerList(nullptr), mMenuBarList(nullptr),
		  mControlSettings(nullptr), mUISettings(nullptr), 
		  mWorldEditSettings(nullptr),
		  mProgressDialogManager(nullptr), mUserTagManager(nullptr),
		  mFrameStatsWindowManager(nullptr), mProfilerViewport(nullptr)
	{
		// Must be created first (other objects used them for registration)
		mControlSettings = new ControlSettings("controls.cfg");
		mUISettings = new UISettings("ui.cfg");
		mWorldEditSettings = new WorldEditSettings("worldedit.cfg");
		mToolManagerList = new ToolManagerList;
		mHotkeyManagerList = new HotkeyManagerList;
		mMenuBarList = new MenuBarList;

		// Creating objects for MyGUI work
		mMyGUIInput = new MyGUIInput;
		mUserTagManager = new UserTagManager;

		// Create some other objects
		mProgressDialogManager = new ProgressDialogManager;
		mFrameStatsWindowManager = new FrameStatsWindowManager;

		registerUIModeListener( UIInitPriority::GUISYSTEM );
	}
	//-----------------------------------------------------------------------
	GUISystem::~GUISystem()
	{
		// Destroying the rest of objects
		if(mProgressDialogManager)
			delete mProgressDialogManager;

		if(mFrameStatsWindowManager)
			delete mFrameStatsWindowManager;

		// Destroing objects for MyGUI work
		if(mUserTagManager)
			delete mUserTagManager;

		if(mMyGUIInput)
			delete mMyGUIInput;

		// Must be destroyed last
		if(mMenuBarList)
			delete mMenuBarList;

		if(mHotkeyManagerList)
			delete mHotkeyManagerList;

		if(mToolManagerList)
			delete mToolManagerList;

		if(mWorldEditSettings)
			delete mWorldEditSettings;

		if(mUISettings)
			delete mUISettings;

		if(mControlSettings)
			delete mControlSettings;
	}
	//-----------------------------------------------------------------------
	void GUISystem::uiModeEntered(const UIModeEvent& _evt)
	{
		if(_evt.getNewUIMode() != UIMode::SHUTDOWN)
		{
			initialise();
		}
	}
	//-----------------------------------------------------------------------
	void GUISystem::uiModeExited(const UIModeEvent& _evt)
	{
		if(_evt.getNewUIMode() == UIMode::SHUTDOWN)
		{
			shutdown();
		}
	}
	//-----------------------------------------------------------------------
	void GUISystem::initialise()
	{
		if(mInitialised)
			return;

		GOTHOGRE_INFO("Initialising GUISystem");
		initialise_before_MyGUI();
		initialise_MyGUI();
		initialise_after_MyGUI();
		mInitialised = true;
		GOTHOGRE_INFO("GUISystem initialised successfully.");
	}
	//-----------------------------------------------------------------------
	void GUISystem::shutdown()
	{
		GOTHOGRE_INFO("Shutting down GUISystem");
		shutdown_before_MyGUI();
		shutdown_MyGUI();
		shutdown_after_MyGUI();
		mInitialised = false;
		GOTHOGRE_INFO("GUISystem shutted down successfully.");
	}
	//-----------------------------------------------------------------------
	void GUISystem::initialise_before_MyGUI()
	{
		// Create simple scene manager, camera, and simpliest scene 
		// for using in additional viewports.
		mUISceneManager = Ogre::Root::getSingleton().createSceneManager(ST_GENERIC, "UISceneManager");
		mUICamera = mUISceneManager->createCamera("UICamera");

		mWindow = Root::getSingleton().getAutoCreatedWindow();
		mUIViewport = mWindow->addViewport(mUICamera, 1000);
		mUIViewport->setClearEveryFrame(false);

		// Initialise resource group used by GUI.
		ResourceLocationScanner::getSingleton().scan(ResourceGroup::GUIMEDIA, false);

		// Create the profiler viewport (if it's enabled).
		mProfilerViewport = new ProfilerViewport;
	}
	//-----------------------------------------------------------------------
	void GUISystem::initialise_MyGUI()
	{
		// Initialise MyGUI.
		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(mWindow, mUISceneManager, ResourceGroup::GUIMEDIA);
		mPlatform->getRenderManagerPtr()->setActiveViewport(getViewportIndex(mUIViewport));
		
		mGui = new MyGUI::Gui();
		mGui->initialise(StringUtil::BLANK);
	}
	//-------------------------------------------------------------------------------
	unsigned short GUISystem::getViewportIndex(Viewport* _viewport)
	{
		RenderTarget* target = _viewport->getTarget();
		for(unsigned short i = 0; i != target->getNumViewports(); ++i)
			if(target->getViewport(i) == _viewport)
				return i;

		return -1;
	}
	//-----------------------------------------------------------------------
	void GUISystem::initialise_after_MyGUI()
	{
		// Initialise input system for MyGUI
		// (it allows MyGUI to get input from mouse and keyboard).
		mMyGUIInput->initialise();

		// Register additional xml types
		mUserTagManager->initialise();

		// Register additional widget types
		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		factory.registerFactory<TreeControl>("Widget");
		factory.registerFactory<TreeControlItem>("Widget");

		// Load configuration files
		mControlSettings->initialise();
		mUISettings->initialise();
		mWorldEditSettings->initialise();

		// Initialise progress dialog manager
		mProgressDialogManager->initialise();
	}
	//-----------------------------------------------------------------------
	void GUISystem::shutdown_before_MyGUI()
	{
		// Shutdown progress dialog manager
		mProgressDialogManager->shutdown();

		// Save configuration files
		mControlSettings->shutdown();
		mUISettings->shutdown();
		mWorldEditSettings->shutdown();

		// Unregister additional widget types.
		if(MyGUI::FactoryManager::getInstancePtr())
		{
			MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
			factory.unregisterFactory<TreeControl>("Widget");
			factory.unregisterFactory<TreeControlItem>("Widget");
		}

		// Unregister additional xml types
		mUserTagManager->shutdown();

		// Shutdown input system for MyGUI
		mMyGUIInput->shutdown();
	}
	//-----------------------------------------------------------------------
	void GUISystem::shutdown_MyGUI()
	{
		// Shutdown MyGUI.
		if(mGui)
		{
			mGui->shutdown();
			delete mGui;
			mGui = nullptr;
		}

		if(mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}
	//-----------------------------------------------------------------------
	void GUISystem::shutdown_after_MyGUI()
	{
		// Destroy the profiler viewport
		if(mProfilerViewport)
		{
			delete mProfilerViewport;
			mProfilerViewport = nullptr;
		}

		// Destroy the additional viewport.
		if(mUIViewport)
		{
			mWindow->removeViewport(mUIViewport->getZOrder());
			mUIViewport = nullptr;
		}

		if(mUISceneManager)
		{
			Ogre::Root::getSingleton().destroySceneManager(mUISceneManager);
			mUISceneManager = nullptr;
			mUICamera = nullptr;
		}

		mWindow = nullptr;
	}


} // namespace GUISystem