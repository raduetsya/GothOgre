#include "GUISystem_Precompiled.h"
#include "GUISystem_ProfilerViewport.h"


namespace GUISystem
{
	//--------------------------------------------------------------------------
	// ProfilerSceneManager - a special scene manager to show profiler
	//--------------------------------------------------------------------------
	class ProfilerSceneManager : public SceneManager
	{
	public:
		ProfilerSceneManager(const String& _instanceName)
			: SceneManager(_instanceName), mScrollX(0), mScrollY(0) {}
		
		~ProfilerSceneManager() 
		{
			if(Profiler::getSingletonPtr())
			{
				// Enable/disable the Ogre profiler
				if(Profiler::getSingleton().getEnabled())
					Profiler::getSingleton().setEnabled(false);
			}
		}

		// Type of the scene manager
		const String& getTypeName() const
		{
			return FACTORY_TYPE_NAME;
		}

		void setScroll(Real _scrollX, Real _scrollY)
		{
			mScrollX = _scrollX;
			mScrollY = _scrollY;
		}

		// Overrides the "SceneManager::_renderScene" function.
		void _renderScene(Camera* _camera, Viewport* _vp, bool _includeOverlays)
		{
			showProfilerOverlay();
			SceneManager::_renderScene(_camera, _vp, _includeOverlays);
			hideProfilerOverlay();
		}

		// Shows the Ogre's standard profiler overlay
		void showProfilerOverlay()
		{
			Overlay* overlayProfiler = nullptr;
			if(Profiler::getSingletonPtr())
			{
				// Enable/disable the Ogre profiler
				if(!Profiler::getSingleton().getEnabled())
					Profiler::getSingleton().setEnabled(true);

				// Show the profiler's overlay.
				overlayProfiler = OverlayManager::getSingleton().getByName("Profiler");
				if(overlayProfiler)
				{
					overlayProfiler->show();
					overlayProfiler->setScroll(mScrollX, mScrollY);
				}
			}
		}

		// Hides the Ogre's standard profiler overlay
		void hideProfilerOverlay()
		{
			// Hide the profiler's overlay
			// (we must do it because otherwise the profiler's overlay
			// can be shown in another viewports.
			Overlay* overlayProfiler = OverlayManager::getSingleton().getByName("Profiler");
			if(overlayProfiler)
				overlayProfiler->hide();
		}

		static const String FACTORY_TYPE_NAME;

	private:
		Real mScrollX, mScrollY;
	};

	const String ProfilerSceneManager::FACTORY_TYPE_NAME = "ProfilerSceneManager";



	//--------------------------------------------------------------------------
	// ProfilerSceneManager - factory
	//--------------------------------------------------------------------------
	class ProfilerSceneManagerFactory : public SceneManagerFactory
	{
	public:
		ProfilerSceneManagerFactory()
		{
			SceneManagerEnumerator::getSingleton().addFactory(this);
		}

		~ProfilerSceneManagerFactory()
		{
			SceneManagerEnumerator::getSingleton().removeFactory(this);
		}

		SceneManager* createInstance(const String& _instanceName)
		{
			return OGRE_NEW ProfilerSceneManager(_instanceName);
		}

		void destroyInstance(SceneManager* _sm)
		{
			OGRE_DELETE (ProfilerSceneManager*) _sm;
		}

		void initMetaData() const
		{
			mMetaData.typeName = FACTORY_TYPE_NAME;
			mMetaData.description = FACTORY_TYPE_NAME;
			mMetaData.sceneTypeMask = ST_GENERIC;
			mMetaData.worldGeometrySupported = false;
		}

		static const String FACTORY_TYPE_NAME;
	};

	const String ProfilerSceneManagerFactory::FACTORY_TYPE_NAME = ProfilerSceneManager::FACTORY_TYPE_NAME;




	//--------------------------------------------------------------------------
	// ProfilerViewport - main class implementation
	//--------------------------------------------------------------------------
	ProfilerViewport::ProfilerViewport()
		: mViewport(nullptr), mDummyCamera(nullptr), 
		  mProfilerSceneManager(nullptr), 
		  mProfilerSceneManagerFactory(nullptr)
	{
		// Load settings from the configuration files.
		registerGraphicSettingsListener();
	}
	//--------------------------------------------------------------------------
	ProfilerViewport::~ProfilerViewport()
	{
		stopProfiler();
	}
	//--------------------------------------------------------------------------
	void ProfilerViewport::graphicSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		mProfilerEnabled = GraphicSettings::getSingleton().getSetting("Enabled", "Profiler", true);
		if(mProfilerEnabled)
		{
			mScrollX = GraphicSettings::getSingleton().getSetting("ScrollX", "Profiler", 0.00f);
			mScrollY = GraphicSettings::getSingleton().getSetting("ScrollY", "Profiler", -0.08f);
			startProfiler();
		}
		else
			stopProfiler();
	}
	//-------------------------------------------------------------------------
	void ProfilerViewport::startProfiler()
	{
		if(!mProfilerSceneManagerFactory)
			mProfilerSceneManagerFactory = new ProfilerSceneManagerFactory;

		if(!mProfilerSceneManager)
			mProfilerSceneManager = (ProfilerSceneManager*) Root::getSingleton().createSceneManager("ProfilerSceneManager", "ProfilerSceneManager");
			
		mProfilerSceneManager->setScroll(mScrollX, mScrollY);

		if(!mDummyCamera)
			mDummyCamera = mProfilerSceneManager->createCamera("DummyCamera");

		if(!mViewport)
		{
			mViewport = Root::getSingleton().getAutoCreatedWindow()
				->addViewport(mDummyCamera, 9999);
			mViewport->setClearEveryFrame(false);
		}
	}
	//-------------------------------------------------------------------------
	void ProfilerViewport::stopProfiler()
	{
		if(mViewport)
		{
			Root::getSingleton().getAutoCreatedWindow()->removeViewport(mViewport->getZOrder());
			mViewport = nullptr;
		}

		if(mDummyCamera)
		{
			mProfilerSceneManager->destroyCamera(mDummyCamera);
			mDummyCamera = nullptr;
		}

		if(mProfilerSceneManager)
		{
			Root::getSingleton().destroySceneManager(mProfilerSceneManager);
			mProfilerSceneManager = nullptr;
		}

		if(mProfilerSceneManagerFactory)
		{
			delete mProfilerSceneManagerFactory;
			mProfilerSceneManagerFactory = nullptr;
		}
	}


} // namespace GUISystem