#include "Precompiled.h"
#include "LightShadowConfigurator.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	LightShadowConfigurator::LightShadowConfigurator(SceneManagerExImpl* _sm)
		: mSceneManager(_sm)
	{
		registerGraphicSettingsListener();
	}
	//-------------------------------------------------------------------------
	LightShadowConfigurator::~LightShadowConfigurator()
	{
	}
	//-------------------------------------------------------------------------
	void LightShadowConfigurator::graphicSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		// MaxRadiusList for LiBST
		LiBST::RealList defaultLiBSTMaxRadii;
		defaultLiBSTMaxRadii.clear();
		defaultLiBSTMaxRadii.push_back(5000);   // 50m
		defaultLiBSTMaxRadii.push_back(50000);  // 500m
		LiBST::RealList liBSTMaxRadii = GraphicSettings::getSingleton().getSetting("LightBSTMaxRadiusList", "Advanced", defaultLiBSTMaxRadii);
		mSceneManager->_getLightBST()->setMaxRadiusList(liBSTMaxRadii);
	}
	//-------------------------------------------------------------------------
	void LightShadowConfigurator::preRenderScene(Viewport* _viewport)
	{

	}
	//-------------------------------------------------------------------------
	void LightShadowConfigurator::postRenderScene(Viewport* _viewport)
	{
	}


} // namespace GothOgre
