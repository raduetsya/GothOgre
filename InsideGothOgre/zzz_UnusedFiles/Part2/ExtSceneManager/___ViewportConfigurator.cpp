#include "Precompiled.h"
#include "ViewportConfigurator.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	ViewportConfigurator::ViewportConfigurator(SceneManagerExImpl* _sm)
		: mSceneManager(_sm)
	{
		registerGraphicSettingsListener();
	}
	//-------------------------------------------------------------------------
	ViewportConfigurator::~ViewportConfigurator()
	{
	}
	//-------------------------------------------------------------------------
	void ViewportConfigurator::graphicSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		// graphics settings' fog
		Real defaultSightRange = 20000; // 200m
		Real sightRange = GraphicSettings::getSingleton().getSetting("SightRange", "Basic", defaultSightRange);
		mFogDisabled = GraphicSettings::getSingleton().getSetting("FogDisabled", "Debug", false);
		mMaxFogStart = GraphicSettings::getSingleton().getSetting("FogStart", "Advanced", 0.5f);
		mMaxFogEnd = GraphicSettings::getSingleton().getSetting("FogEnd", "Advanced", 1.0f);
		mMaxFogStart *= sightRange;
		mMaxFogEnd *= sightRange;

		// Far clip distance
		Real defaultFarClipDistance = 1.0f; // coefficient for sight range
		mMaxFarClipDistance = GraphicSettings::getSingleton().getSetting("FarClipDistance", "Advanced", defaultFarClipDistance);
		mMaxFarClipDistance *= sightRange;

		// Far clipping on hardware
		mNoHardwareFarClipping = GraphicSettings::getSingleton().getSetting("NoHardwareFarClipping", "Debug", false);

		// MaxRadiusList for SnBST
		SnBST::RealList defaultMaxRadii;
		defaultMaxRadii.push_back(1000);   // 10m
		defaultMaxRadii.push_back(3000);   // 30m
		defaultMaxRadii.push_back(10000);  // 100m
		defaultMaxRadii.push_back(30000);  // 300m
		defaultMaxRadii.push_back(100000); // 1km
		SnBST::RealList maxRadii = GraphicSettings::getSingleton().getSetting("SceneNodeBSTMaxRadiusList", "Advanced", defaultMaxRadii);
		mSceneManager->_getSceneNodeBST()->setMaxRadiusList( maxRadii );

		// DistanceMinRadiusPairList
		SnBST::RealPairList defaultDistMinRPoints;
		defaultDistMinRPoints.clear();
		defaultDistMinRPoints.push_back(std::make_pair( (Real) 0.0, (Real) 0 ));
		defaultDistMinRPoints.push_back(std::make_pair( (Real) 0.2, (Real) 0 ));
		defaultDistMinRPoints.push_back(std::make_pair( (Real) 0.7, (Real) 500 ));
		defaultDistMinRPoints.push_back(std::make_pair( (Real) 1.0, (Real) 1000 ));
		SnBST::RealPairList distanceMinRPoints = GraphicSettings::getSingleton().getSetting("DistanceMinRadiusPairList", "Advanced", defaultDistMinRPoints);
		for(size_t i = 0; i != distanceMinRPoints.size(); ++i)
			distanceMinRPoints[i].first *= sightRange;
		mSceneManager->_setDistanceMinRadiusPairList( distanceMinRPoints );

		// Polygon mode
		mPolygonMode = GraphicSettings::getSingleton().getSetting("PolygonMode", "", PM_SOLID);
	}
	//-------------------------------------------------------------------------
	void ViewportConfigurator::preRenderScene(Viewport* _viewport)
	{
		// Get weather properties from scene manager
		const ColourValue& weatherFogColour = mSceneManager->getWeatherFogColour();
		Real weatherFogStart = mSceneManager->getWeatherFogStart();
		Real weatherFogEnd = mSceneManager->getWeatherFogEnd();
		Real weatherVisibilityRange = mSceneManager->getWeatherVisibilityRange();
		const ColourValue& backgroundColour = mSceneManager->getWeatherBackgroundColour();
		
		_viewport->setBackgroundColour(backgroundColour);

		Real farClipDistance = std::min(weatherVisibilityRange, mMaxFarClipDistance);
		mSceneManager->_setFarClipDistance(farClipDistance);
		if(mNoHardwareFarClipping)
			_viewport->getCamera()->setFarClipDistance(0);
		else
			_viewport->getCamera()->setFarClipDistance(farClipDistance);

		if(mFogDisabled)
		{
			mSceneManager->setFog(FOG_NONE);
		}
		else
		{
			Real fogStart = std::min(weatherFogStart, mMaxFogStart);
			Real fogEnd   = std::min(weatherFogEnd, mMaxFogEnd);
			mSceneManager->setFog(FOG_LINEAR, weatherFogColour,
				0.001f, fogStart, fogEnd);
		}

		_viewport->getCamera()->setPolygonMode(mPolygonMode);
	}
	//-------------------------------------------------------------------------
	void ViewportConfigurator::postRenderScene(Viewport* _viewport)
	{
	}


} // namespace GothOgre
