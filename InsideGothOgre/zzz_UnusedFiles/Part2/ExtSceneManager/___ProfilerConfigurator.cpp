#include "Precompiled.h"
#include "ProfilerConfigurator.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	ProfilerConfigurator::ProfilerConfigurator(SceneManagerExImpl* _sm)
		: mSceneManager(_sm)
	{
		// Load settings from the configuration files.
		registerGraphicSettingsListener();

		// Find necessary resources
		ResourceLocationScanner::getSingleton().scan( ResourceGroup::GUIMEDIA );

		// Initially enable the profiler (for correct order of profiles).
		if(Profiler::getSingletonPtr())
			Profiler::getSingleton().setEnabled(true);
	}
	//-------------------------------------------------------------------------
	ProfilerConfigurator::~ProfilerConfigurator()
	{
	}
	//-------------------------------------------------------------------------
	void ProfilerConfigurator::graphicSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		mProfilerEnabled = GraphicSettings::getSingleton().getSetting("Profiler", "Debug", true);
	}
	//-------------------------------------------------------------------------
	void ProfilerConfigurator::preRenderScene(Viewport* _viewport)
	{
		Overlay* overlayProfiler = nullptr;
		if(Profiler::getSingletonPtr())
		{
			// Enable/disable the Ogre profiler
			if(Profiler::getSingleton().getEnabled() != mProfilerEnabled)
				Profiler::getSingleton().setEnabled(mProfilerEnabled);

			// Show the profiler's overlay, if enabled.
			if(mProfilerEnabled)
			{
				overlayProfiler = OverlayManager::getSingleton().getByName("Profiler");
				if(overlayProfiler)
					overlayProfiler->show();
			}
		}
	}
	//-------------------------------------------------------------------------
	void ProfilerConfigurator::postRenderScene(Viewport* _viewport)
	{

	}


} // namespace GothOgre
