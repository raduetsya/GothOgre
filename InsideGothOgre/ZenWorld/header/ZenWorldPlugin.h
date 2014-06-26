#ifndef ZENWORLD_PLUGIN_H
#define ZENWORLD_PLUGIN_H


namespace GothOgre
{
	class CamTrjKeyFrameFactory;
	class CodeMasterFactory;
	class CSCameraFactory;
	class CSTriggerFactory;
	class EarthquakeFactory;
	class ItemFactory;
	class MessageFilterFactory;
	class MobBedFactory;
	class MobContainerFactory;
	class MobDoorFactory;
	class MobFactory;
	class MobFireFactory;
	class MobInterFactory;
	class MobLadderFactory;
	class MobSwitchFactory;
	class MobWheelFactory;
	class MoverFactory;
	class MoverControlerFactory;
	class PFXControlerFactory;
	class TouchDamageFactory;
	class TriggerChangeLevelFactory;
	class TriggerFactory;
	class TriggerListFactory;
	class TriggerScriptFactory;
	class TriggerUntouchFactory;
	class TriggerWorldStartFactory;
	class VobAnimateFactory;
	class VobFactory;
	class VobLensFlareFactory;
	class VobLevelCompoFactory;
	class VobLightFactory;
	class VobScreenFXFactory;
	class VobSoundFactory;
	class VobSoundDaytimeFactory;
	class VobSpotFactory;
	class VobStairFactory;
	class VobStartpointFactory;
	class ZenBspFactory;
	class ZenMtlFactory;
	class ZenWorldFactory;
	class ZoneMusicFactory;
	class ZoneMusicDefaultFactory;
	class ZoneVobFarPlaneFactory;
	class ZoneVobFarPlaneDefaultFactory;
	class ZoneZFogDefaultFactory;
	class ZoneZFogFactory;

	/** Plugin instance for EditorTools */
	class ZenWorldPlugin : public Plugin
	{
	public:
		ZenWorldPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		CamTrjKeyFrameFactory*		mCamTrjKeyFrameFactory;
		CodeMasterFactory*			mCodeMasterFactory;
		CSCameraFactory*			mCSCameraFactory;
		CSTriggerFactory*			mCSTriggerFactory;
		EarthquakeFactory*			mEarthquakeFactory;
		ItemFactory*				mItemFactory;
		MessageFilterFactory*		mMessageFilterFactory;
		MobBedFactory*				mMobBedFactory;
		MobContainerFactory*		mMobContainerFactory;
		MobDoorFactory*				mMobDoorFactory;
		MobFactory*					mMobFactory;
		MobFireFactory*				mMobFireFactory;
		MobInterFactory*			mMobInterFactory;
		MobLadderFactory*			mMobLadderFactory;
		MobSwitchFactory*			mMobSwitchFactory;
		MobWheelFactory*			mMobWheelFactory;
		MoverFactory*				mMoverFactory;
		MoverControlerFactory*		mMoverControlerFactory;
		PFXControlerFactory*		mPFXControlerFactory;
		TouchDamageFactory*			mTouchDamageFactory;
		TriggerChangeLevelFactory*	mTriggerChangeLevelFactory;
		TriggerFactory*				mTriggerFactory;
		TriggerListFactory*			mTriggerListFactory;
		TriggerScriptFactory*		mTriggerScriptFactory;
		TriggerUntouchFactory*		mTriggerUntouchFactory;
		TriggerWorldStartFactory*	mTriggerWorldStartFactory;
		VobAnimateFactory*			mVobAnimateFactory;
		VobFactory*					mVobFactory;
		VobLensFlareFactory*		mVobLensFlareFactory;
		VobLevelCompoFactory*		mVobLevelCompoFactory;
		VobLightFactory*			mVobLightFactory;
		VobScreenFXFactory*			mVobScreenFXFactory;
		VobSoundFactory*			mVobSoundFactory;
		VobSoundDaytimeFactory*		mVobSoundDaytimeFactory;
		VobSpotFactory*				mVobSpotFactory;
		VobStairFactory*			mVobStairFactory;
		VobStartpointFactory*		mVobStartpointFactory;
		ZenBspFactory*				mZenBspFactory;
		ZenMtlFactory*				mZenMtlFactory;
		ZenWorldFactory*			mZenWorldFactory;
		ZoneMusicFactory*			mZoneMusicFactory;
		ZoneMusicDefaultFactory*	mZoneMusicDefaultFactory;
		ZoneVobFarPlaneFactory*		mZoneVobFarPlaneFactory;
		ZoneVobFarPlaneDefaultFactory*	mZoneVobFarPlaneDefaultFactory;
		ZoneZFogDefaultFactory*		mZoneZFogDefaultFactory;
		ZoneZFogFactory*			mZoneZFogFactory;
	};

} // namespace GothOgre

#endif // ZENWORLD_PLUGIN_H
