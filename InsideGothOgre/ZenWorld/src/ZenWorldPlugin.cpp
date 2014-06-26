#include "Precompiled.h"
#include "ZenWorldPlugin.h"
#include "CamTrjKeyFrame.h"
#include "CodeMaster.h"
#include "CSCamera.h"
#include "CSTrigger.h"
#include "Earthquake.h"
#include "Item.h"
#include "MessageFilter.h"
#include "MobBed.h"
#include "MobContainer.h"
#include "MobDoor.h"
#include "Mob.h"
#include "MobFire.h"
#include "MobInter.h"
#include "MobLadder.h"
#include "MobSwitch.h"
#include "MobWheel.h"
#include "Mover.h"
#include "MoverControler.h"
#include "PFXControler.h"
#include "TouchDamage.h"
#include "TriggerChangeLevel.h"
#include "Trigger.h"
#include "TriggerList.h"
#include "TriggerScript.h"
#include "TriggerUntouch.h"
#include "TriggerWorldStart.h"
#include "VobAnimate.h"
#include "Vob.h"
#include "VobLensFlare.h"
#include "VobLevelCompo.h"
#include "VobLight.h"
#include "VobScreenFX.h"
#include "VobSound.h"
#include "VobSoundDaytime.h"
#include "VobSpot.h"
#include "VobStair.h"
#include "VobStartpoint.h"
#include "ZenBsp.h"
#include "ZenMtl.h"
#include "ZenWorld.h"
#include "ZoneMusic.h"
#include "ZoneMusicDefault.h"
#include "ZoneVobFarPlane.h"
#include "ZoneVobFarPlaneDefault.h"
#include "ZoneZFog.h"
#include "ZoneZFogDefault.h"

namespace GothOgre
{
	GOTHOGRE_PLUGIN_ENTRY_POINT(ZenWorldPlugin)
	GOTHOGRE_SET_MODULE_NAMES("ZenWorld", "ZenWorld")

	ZenWorldPlugin::ZenWorldPlugin()
	{
		mCamTrjKeyFrameFactory		= nullptr;
		mCodeMasterFactory			= nullptr;
		mCSCameraFactory			= nullptr;
		mCSTriggerFactory			= nullptr;
		mEarthquakeFactory			= nullptr;
		mItemFactory				= nullptr;
		mMessageFilterFactory		= nullptr;
		mMobBedFactory				= nullptr;
		mMobContainerFactory		= nullptr;
		mMobDoorFactory				= nullptr;
		mMobFactory					= nullptr;
		mMobFireFactory				= nullptr;
		mMobInterFactory			= nullptr;
		mMobLadderFactory			= nullptr;
		mMobSwitchFactory			= nullptr;
		mMobWheelFactory			= nullptr;
		mMoverFactory				= nullptr;
		mMoverControlerFactory		= nullptr;
		mPFXControlerFactory		= nullptr;
		mTouchDamageFactory			= nullptr;
		mTriggerChangeLevelFactory	= nullptr;
		mTriggerFactory				= nullptr;
		mTriggerListFactory			= nullptr;
		mTriggerScriptFactory		= nullptr;
		mTriggerUntouchFactory		= nullptr;
		mTriggerWorldStartFactory	= nullptr;
		mVobAnimateFactory			= nullptr;
		mVobFactory					= nullptr;
		mVobLensFlareFactory		= nullptr;
		mVobLevelCompoFactory		= nullptr;
		mVobLightFactory			= nullptr;
		mVobScreenFXFactory			= nullptr;
		mVobSoundFactory			= nullptr;
		mVobSoundDaytimeFactory		= nullptr;
		mVobSpotFactory				= nullptr;
		mVobStairFactory			= nullptr;
		mVobStartpointFactory		= nullptr;
		mZenBspFactory				= nullptr;
		mZenMtlFactory				= nullptr;
		mZenWorldFactory			= nullptr;
		mZoneMusicFactory			= nullptr;
		mZoneMusicDefaultFactory	= nullptr;
		mZoneVobFarPlaneFactory		= nullptr;
		mZoneVobFarPlaneDefaultFactory	= nullptr;
		mZoneZFogFactory			= nullptr;
		mZoneZFogDefaultFactory		= nullptr;
	}
	//---------------------------------------------------------------------------
	const String& ZenWorldPlugin::getName() const
	{
		static String sPluginName = GOTHOGRE_LONG_MODULE_NAME;
		return sPluginName;
	}
	//---------------------------------------------------------------------------
	void ZenWorldPlugin::install()
	{
		GOTHOGRE_INFO("Installing the \"" << getName() << "\" plugin.");

		mCamTrjKeyFrameFactory		= new CamTrjKeyFrameFactory;
		mCodeMasterFactory			= new CodeMasterFactory;
		mCSCameraFactory			= new CSCameraFactory;
		mCSTriggerFactory			= new CSTriggerFactory;
		mEarthquakeFactory			= new EarthquakeFactory;
		mItemFactory				= new ItemFactory;
		mMessageFilterFactory		= new MessageFilterFactory;
		mMobBedFactory				= new MobBedFactory;
		mMobContainerFactory		= new MobContainerFactory;
		mMobDoorFactory				= new MobDoorFactory;
		mMobFactory					= new MobFactory;
		mMobFireFactory				= new MobFireFactory;
		mMobInterFactory			= new MobInterFactory;
		mMobLadderFactory			= new MobLadderFactory;
		mMobSwitchFactory			= new MobSwitchFactory;
		mMobWheelFactory			= new MobWheelFactory;
		mMoverFactory				= new MoverFactory;
		mMoverControlerFactory		= new MoverControlerFactory;
		mPFXControlerFactory		= new PFXControlerFactory;
		mTouchDamageFactory			= new TouchDamageFactory;
		mTriggerChangeLevelFactory	= new TriggerChangeLevelFactory;
		mTriggerFactory				= new TriggerFactory;
		mTriggerListFactory			= new TriggerListFactory;
		mTriggerScriptFactory		= new TriggerScriptFactory;
		mTriggerUntouchFactory		= new TriggerUntouchFactory;
		mTriggerWorldStartFactory	= new TriggerWorldStartFactory;
		mVobAnimateFactory			= new VobAnimateFactory;
		mVobFactory					= new VobFactory;
		mVobLensFlareFactory		= new VobLensFlareFactory;
		mVobLevelCompoFactory		= new VobLevelCompoFactory;
		mVobLightFactory			= new VobLightFactory;
		mVobScreenFXFactory			= new VobScreenFXFactory;
		mVobSoundFactory			= new VobSoundFactory;
		mVobSoundDaytimeFactory		= new VobSoundDaytimeFactory;
		mVobSpotFactory				= new VobSpotFactory;
		mVobStairFactory			= new VobStairFactory;
		mVobStartpointFactory		= new VobStartpointFactory;
		mZenBspFactory				= new ZenBspFactory;
		mZenMtlFactory				= new ZenMtlFactory;
		mZenWorldFactory			= new ZenWorldFactory;
		mZoneMusicFactory			= new ZoneMusicFactory;
		mZoneMusicDefaultFactory	= new ZoneMusicDefaultFactory;
		mZoneVobFarPlaneFactory		= new ZoneVobFarPlaneFactory;
		mZoneVobFarPlaneDefaultFactory	= new ZoneVobFarPlaneDefaultFactory;
		mZoneZFogFactory			= new ZoneZFogFactory;
		mZoneZFogDefaultFactory		= new ZoneZFogDefaultFactory;

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin installed successfully.");
	}
	//---------------------------------------------------------------------------
	void ZenWorldPlugin::initialise()
	{
	}
	//---------------------------------------------------------------------------
	void ZenWorldPlugin::shutdown()
	{
	}
	//---------------------------------------------------------------------------
	#define DELETE_FACTORY(factory) if(factory) {delete factory; factory = nullptr;}
	
	void ZenWorldPlugin::uninstall()
	{
		GOTHOGRE_INFO("Uninstalling the \"" << getName() << "\" plugin.");

		DELETE_FACTORY(mCamTrjKeyFrameFactory);
		DELETE_FACTORY(mCodeMasterFactory);
		DELETE_FACTORY(mCSCameraFactory);
		DELETE_FACTORY(mCSTriggerFactory);
		DELETE_FACTORY(mEarthquakeFactory);
		DELETE_FACTORY(mItemFactory);
		DELETE_FACTORY(mMessageFilterFactory);
		DELETE_FACTORY(mMobBedFactory);
		DELETE_FACTORY(mMobContainerFactory);
		DELETE_FACTORY(mMobDoorFactory);
		DELETE_FACTORY(mMobFactory);
		DELETE_FACTORY(mMobFireFactory);
		DELETE_FACTORY(mMobInterFactory);
		DELETE_FACTORY(mMobLadderFactory);
		DELETE_FACTORY(mMobSwitchFactory);
		DELETE_FACTORY(mMobWheelFactory);
		DELETE_FACTORY(mMoverFactory);
		DELETE_FACTORY(mMoverControlerFactory);
		DELETE_FACTORY(mPFXControlerFactory);
		DELETE_FACTORY(mTouchDamageFactory);
		DELETE_FACTORY(mTriggerChangeLevelFactory);
		DELETE_FACTORY(mTriggerFactory);
		DELETE_FACTORY(mTriggerListFactory);
		DELETE_FACTORY(mTriggerScriptFactory);
		DELETE_FACTORY(mTriggerUntouchFactory);
		DELETE_FACTORY(mTriggerWorldStartFactory);
		DELETE_FACTORY(mVobAnimateFactory);
		DELETE_FACTORY(mVobFactory);
		DELETE_FACTORY(mVobLensFlareFactory);
		DELETE_FACTORY(mVobLevelCompoFactory);
		DELETE_FACTORY(mVobLightFactory);
		DELETE_FACTORY(mVobScreenFXFactory);
		DELETE_FACTORY(mVobSoundFactory);
		DELETE_FACTORY(mVobSoundDaytimeFactory);
		DELETE_FACTORY(mVobSpotFactory);
		DELETE_FACTORY(mVobStairFactory);
		DELETE_FACTORY(mVobStartpointFactory);
		DELETE_FACTORY(mZenBspFactory);
		DELETE_FACTORY(mZenMtlFactory);
		DELETE_FACTORY(mZenWorldFactory);
		DELETE_FACTORY(mZoneMusicFactory);
		DELETE_FACTORY(mZoneMusicDefaultFactory);
		DELETE_FACTORY(mZoneVobFarPlaneFactory);
		DELETE_FACTORY(mZoneVobFarPlaneDefaultFactory);
		DELETE_FACTORY(mZoneZFogFactory);
		DELETE_FACTORY(mZoneZFogDefaultFactory);

		GOTHOGRE_INFO("The \"" << getName() << "\" plugin uninstalled successfully.");
	}

} // namespace GothOgre