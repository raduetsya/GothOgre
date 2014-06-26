#include "GothOgre_Precompiled.h"
#include "GothOgre_SkyDef.h"
#include "GothOgre_StrUtil.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// Static constants
	//---------------------------------------------------------------------------
	const SkyDef::RealModifier        SkyDef::RealModifier::ZERO(0, 0);
	const SkyDef::RealModifier        SkyDef::RealModifier::IDENTITY(1, 0);
	const SkyDef::DegreeModifier      SkyDef::DegreeModifier::ZERO( Degree(0), Degree(0) );
	const SkyDef::DegreeModifier      SkyDef::DegreeModifier::IDENTITY( Degree(1), Degree(0) );
	const SkyDef::SizeTModifier       SkyDef::SizeTModifier::ZERO(0, 0);
	const SkyDef::SizeTModifier       SkyDef::SizeTModifier::IDENTITY(1, 0);
	const SkyDef::ColourValueModifier SkyDef::ColourValueModifier::ZERO( ColourValue(0, 0, 0, 0), ColourValue(0, 0, 0, 0) );
	const SkyDef::ColourValueModifier SkyDef::ColourValueModifier::IDENTITY( ColourValue(1, 1, 1, 1), ColourValue(0, 0, 0, 0) );
	const SkyDef::Vector2Modifier     SkyDef::Vector2Modifier::ZERO( Vector2::ZERO, Vector2::ZERO );
	const SkyDef::Vector2Modifier     SkyDef::Vector2Modifier::IDENTITY( Vector2::UNIT_SCALE, Vector2::ZERO );
	const SkyDef::Vector3Modifier     SkyDef::Vector3Modifier::ZERO( Vector3::ZERO, Vector3::ZERO );
	const SkyDef::Vector3Modifier     SkyDef::Vector3Modifier::IDENTITY( Vector3::UNIT_SCALE, Vector3::ZERO );



	//---------------------------------------------------------------------------
	// Background
	//---------------------------------------------------------------------------
	bool SkyDef::hasBackground() const
	{
		return mBackground != nullptr;
	}
	//---------------------------------------------------------------------------
	const SkyDef::Background& SkyDef::getBackground() const
	{
		GOTHOGRE_ASSERT( mBackground != nullptr, "Background not found");
		return *mBackground;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertBackground(const SkyDef::Background& _src)
	{
		if(!mBackground)
			mBackground = new Background;
		*mBackground = _src;
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeBackground()
	{
		if(mBackground)
		{
			delete mBackground;
			mBackground = nullptr;
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// BackgroundController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumBackgroundControllers() const
	{
		return mBackgroundControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::BackgroundController& SkyDef::getBackgroundController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mBackgroundControllers.size(), "Index of BackgroundController is out of range.");
		return mBackgroundControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::BackgroundControllerIterator SkyDef::getBackgroundControllerIterator() const
	{
		return (BackgroundControllerIterator) mBackgroundControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertBackgroundController(const BackgroundController& _src)
	{
		mBackgroundControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeBackgroundController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mBackgroundControllers.size(), "Index of BackgroundController is out of range.");
		mBackgroundControllers.erase(mBackgroundControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllBackgroundControllers()
	{
		if(!mBackgroundControllers.empty())
		{
			mBackgroundControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// Fog
	//---------------------------------------------------------------------------
	bool SkyDef::hasFog() const
	{
		return mFog != nullptr;
	}
	//---------------------------------------------------------------------------
	const SkyDef::Fog& SkyDef::getFog() const
	{
		GOTHOGRE_ASSERT( mFog != nullptr, "Fog not found");
		return *mFog;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertFog(const SkyDef::Fog& _src)
	{
		if(!mFog)
			mFog = new Fog;
		*mFog = _src;
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeFog()
	{
		if(mFog)
		{
			delete mFog;
			mFog = nullptr;
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// FogController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumFogControllers() const
	{
		return mFogControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::FogController& SkyDef::getFogController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mFogControllers.size(), "Index of FogController is out of range.");
		return mFogControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::FogControllerIterator SkyDef::getFogControllerIterator() const
	{
		return (FogControllerIterator) mFogControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertFogController(const FogController& _src)
	{
		mFogControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeFogController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mFogControllers.size(), "Index of FogController is out of range.");
		mFogControllers.erase(mFogControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllFogControllers()
	{
		if(!mFogControllers.empty())
		{
			mFogControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// FarClippingPlane
	//---------------------------------------------------------------------------
	bool SkyDef::hasFarClippingPlane() const
	{
		return mFarClippingPlane != nullptr;
	}
	//---------------------------------------------------------------------------
	const SkyDef::FarClippingPlane& SkyDef::getFarClippingPlane() const
	{
		GOTHOGRE_ASSERT( mFarClippingPlane != nullptr, "FarClippingPlane not found");
		return *mFarClippingPlane;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertFarClippingPlane(const SkyDef::FarClippingPlane& _src)
	{
		if(!mFarClippingPlane)
			mFarClippingPlane = new FarClippingPlane;
		*mFarClippingPlane = _src;
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeFarClippingPlane()
	{
		if(mFarClippingPlane)
		{
			delete mFarClippingPlane;
			mFarClippingPlane = nullptr;
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// FarClippingPlaneController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumFarClippingPlaneControllers() const
	{
		return mFarClippingPlaneControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::FarClippingPlaneController& SkyDef::getFarClippingPlaneController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mFarClippingPlaneControllers.size(), "Index of FarClippingPlaneController is out of range.");
		return mFarClippingPlaneControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::FarClippingPlaneControllerIterator SkyDef::getFarClippingPlaneControllerIterator() const
	{
		return (FarClippingPlaneControllerIterator) mFarClippingPlaneControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertFarClippingPlaneController(const FarClippingPlaneController& _src)
	{
		mFarClippingPlaneControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeFarClippingPlaneController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mFarClippingPlaneControllers.size(), "Index of FarClippingPlaneController is out of range.");
		mFarClippingPlaneControllers.erase(mFarClippingPlaneControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllFarClippingPlaneControllers()
	{
		if(!mFarClippingPlaneControllers.empty())
		{
			mFarClippingPlaneControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// Atmosphere
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumAtmospheres() const
	{
		return mAtmospheres.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::Atmosphere& SkyDef::getAtmosphere(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mAtmospheres.size(), "Index of Atmosphere is out of range.");
		return mAtmospheres[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::AtmosphereIterator SkyDef::getAtmosphereIterator() const
	{
		return (AtmosphereIterator) mAtmospheres;
	}
	//---------------------------------------------------------------------------
	const SkyDef::Atmosphere& SkyDef::getAtmosphere(const String& _name) const
	{
		size_t index = getAtmosphereIndex(_name);
		return getAtmosphere(index);
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::getAtmosphereIndex(const String& _name) const
	{
		for(size_t i = 0; i != mAtmospheres.size(); ++i)
		{
			if(StrUtil::equalsNoCase( mAtmospheres[i].name, _name ))
				return i;
		}
		return -1;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertAtmosphere(const Atmosphere& _src)
	{
		size_t index = getAtmosphereIndex(_src.name);
		if(index != -1)
			mAtmospheres[index] = _src;
		else
			mAtmospheres.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAtmosphere(size_t _index)
	{
		mAtmospheres.erase( mAtmospheres.begin() + _index );
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllAtmospheres()
	{
		if(!mAtmospheres.empty())
		{
			mAtmospheres.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// AtmosphereController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumAtmosphereControllers() const
	{
		return mAtmosphereControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::AtmosphereController& SkyDef::getAtmosphereController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mAtmosphereControllers.size(), "Index of AtmosphereController is out of range.");
		return mAtmosphereControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::AtmosphereControllerIterator SkyDef::getAtmosphereControllerIterator() const
	{
		return (AtmosphereControllerIterator) mAtmosphereControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertAtmosphereController(const AtmosphereController& _src)
	{
		mAtmosphereControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAtmosphereController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mAtmosphereControllers.size(), "Index of AtmosphereController is out of range.");
		mAtmosphereControllers.erase(mAtmosphereControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllAtmosphereControllers()
	{
		if(!mAtmosphereControllers.empty())
		{
			mAtmosphereControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// Body
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumBodies() const
	{
		return mBodies.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::Body& SkyDef::getBody(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mBodies.size(), "Index of Body is out of range.");
		return mBodies[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::BodyIterator SkyDef::getBodyIterator() const
	{
		return (BodyIterator) mBodies;
	}
	//---------------------------------------------------------------------------
	const SkyDef::Body& SkyDef::getBody(const String& _name) const
	{
		size_t index = getBodyIndex(_name);
		return mBodies[index];
	}
	//---------------------------------------------------------------------------
	bool SkyDef::hasBody(const String& _name) const
	{
		return findBodyIndex(_name) != -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::findBodyIndex(const String& _name) const
	{
		for(size_t i = 0; i != mBodies.size(); ++i)
		{
			if(StrUtil::equalsNoCase( mBodies[i].name, _name ))
				return i;
		}
		return -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::getBodyIndex(const String& _name) const
	{
		size_t index = findBodyIndex( _name );
		if(index == -1)
			GOTHOGRE_EXCEPT("Body '" << _name << "' not found");

		return index;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertBody(const Body& _src)
	{
		size_t index = findBodyIndex(_src.name);
		if(index != -1)
			mBodies[index] = _src;
		else
			mBodies.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeBody(size_t _index)
	{
		mBodies.erase( mBodies.begin() + _index );
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllBodies()
	{
		if(!mBodies.empty())
		{
			mBodies.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// BodyController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumBodyControllers() const
	{
		return mBodyControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::BodyController& SkyDef::getBodyController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mBodyControllers.size(), "Index of BodyController is out of range.");
		return mBodyControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::BodyControllerIterator SkyDef::getBodyControllerIterator() const
	{
		return (BodyControllerIterator) mBodyControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertBodyController(const BodyController& _src)
	{
		mBodyControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeBodyController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mBodyControllers.size(), "Index of BodyController is out of range.");
		mBodyControllers.erase(mBodyControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllBodyControllers()
	{
		if(!mBodyControllers.empty())
		{
			mBodyControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// CloudLayer
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumCloudLayers() const
	{
		return mCloudLayers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::CloudLayer& SkyDef::getCloudLayer(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mCloudLayers.size(), "Index of CloudLayer is out of range.");
		return mCloudLayers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::CloudLayerIterator SkyDef::getCloudLayerIterator() const
	{
		return (CloudLayerIterator) mCloudLayers;
	}
	//---------------------------------------------------------------------------
	const SkyDef::CloudLayer& SkyDef::getCloudLayer(const String& _name) const
	{
		size_t index = getCloudLayerIndex(_name);
		return mCloudLayers[index];
	}
	//---------------------------------------------------------------------------
	bool SkyDef::hasCloudLayer(const String& _name) const
	{
		return findCloudLayerIndex(_name) != -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::findCloudLayerIndex(const String& _name) const
	{
		for(size_t i = 0; i != mCloudLayers.size(); ++i)
		{
			if(StrUtil::equalsNoCase( mCloudLayers[i].name, _name ))
				return i;
		}
		return -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::getCloudLayerIndex(const String& _name) const
	{
		size_t index = findCloudLayerIndex( _name );
		if(index == -1)
			GOTHOGRE_EXCEPT("CloudLayer '" << _name << "' not found");

		return index;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertCloudLayer(const CloudLayer& _src)
	{
		size_t index = findCloudLayerIndex(_src.name);
		if(index != -1)
			mCloudLayers[index] = _src;
		else
			mCloudLayers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeCloudLayer(size_t _index)
	{
		mCloudLayers.erase( mCloudLayers.begin() + _index );
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllCloudLayers()
	{
		if(!mCloudLayers.empty())
		{
			mCloudLayers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// CloudLayerController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumCloudLayerControllers() const
	{
		return mCloudLayerControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::CloudLayerController& SkyDef::getCloudLayerController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mCloudLayerControllers.size(), "Index of CloudLayerController is out of range.");
		return mCloudLayerControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::CloudLayerControllerIterator SkyDef::getCloudLayerControllerIterator() const
	{
		return (CloudLayerControllerIterator) mCloudLayerControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertCloudLayerController(const CloudLayerController& _src)
	{
		mCloudLayerControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeCloudLayerController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mCloudLayerControllers.size(), "Index of CloudLayerController is out of range.");
		mCloudLayerControllers.erase(mCloudLayerControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllCloudLayerControllers()
	{
		if(!mCloudLayerControllers.empty())
		{
			mCloudLayerControllers.clear();
			++mStateCount;
		}
	}




	//---------------------------------------------------------------------------
	// MaterialController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumMaterialControllers() const
	{
		return mMaterialControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::MaterialController& SkyDef::getMaterialController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mMaterialControllers.size(), "Index of MaterialController is out of range.");
		return mMaterialControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::MaterialControllerIterator SkyDef::getMaterialControllerIterator() const
	{
		return (MaterialControllerIterator) mMaterialControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertMaterialController(const MaterialController& _src)
	{
		mMaterialControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeMaterialController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mMaterialControllers.size(), "Index of MaterialController is out of range.");
		mMaterialControllers.erase(mMaterialControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllMaterialControllers()
	{
		if(!mMaterialControllers.empty())
		{
			mMaterialControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// Sound
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumSounds() const
	{
		return mSounds.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::Sound& SkyDef::getSound(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mSounds.size(), "Index of Sound is out of range.");
		return mSounds[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::SoundIterator SkyDef::getSoundIterator() const
	{
		return (SoundIterator) mSounds;
	}
	//---------------------------------------------------------------------------
	const SkyDef::Sound& SkyDef::getSound(const String& _name) const
	{
		size_t index = getSoundIndex(_name);
		return mSounds[index];
	}
	//---------------------------------------------------------------------------
	bool SkyDef::hasSound(const String& _name) const
	{
		return findSoundIndex(_name) != -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::findSoundIndex(const String& _name) const
	{
		for(size_t i = 0; i != mSounds.size(); ++i)
		{
			if(StrUtil::equalsNoCase( mSounds[i].name, _name ))
				return i;
		}
		return -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::getSoundIndex(const String& _name) const
	{
		size_t index = findSoundIndex( _name );
		if(index == -1)
			GOTHOGRE_EXCEPT("Sound '" << _name << "' not found");

		return index;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertSound(const Sound& _src)
	{
		size_t index = findSoundIndex(_src.name);
		if(index != -1)
			mSounds[index] = _src;
		else
			mSounds.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeSound(size_t _index)
	{
		mSounds.erase( mSounds.begin() + _index );
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllSounds()
	{
		if(!mSounds.empty())
		{
			mSounds.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// SoundController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumSoundControllers() const
	{
		return mSoundControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::SoundController& SkyDef::getSoundController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mSoundControllers.size(), "Index of SoundController is out of range.");
		return mSoundControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::SoundControllerIterator SkyDef::getSoundControllerIterator() const
	{
		return (SoundControllerIterator) mSoundControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertSoundController(const SoundController& _src)
	{
		mSoundControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeSoundController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mSoundControllers.size(), "Index of SoundController is out of range.");
		mSoundControllers.erase(mSoundControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllSoundControllers()
	{
		if(!mSoundControllers.empty())
		{
			mSoundControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// Overlay
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumOverlays() const
	{
		return mOverlays.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::Overlay& SkyDef::getOverlay(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mOverlays.size(), "Index of Overlay is out of range.");
		return mOverlays[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::OverlayIterator SkyDef::getOverlayIterator() const
	{
		return (OverlayIterator) mOverlays;
	}
	//---------------------------------------------------------------------------
	const SkyDef::Overlay& SkyDef::getOverlay(const String& _name) const
	{
		size_t index = getOverlayIndex(_name);
		return mOverlays[index];
	}
	//---------------------------------------------------------------------------
	bool SkyDef::hasOverlay(const String& _name) const
	{
		return findOverlayIndex(_name) != -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::findOverlayIndex(const String& _name) const
	{
		for(size_t i = 0; i != mOverlays.size(); ++i)
		{
			if(StrUtil::equalsNoCase( mOverlays[i].name, _name ))
				return i;
		}
		return -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::getOverlayIndex(const String& _name) const
	{
		size_t index = findOverlayIndex( _name );
		if(index == -1)
			GOTHOGRE_EXCEPT("Overlay '" << _name << "' not found");

		return index;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertOverlay(const Overlay& _src)
	{
		size_t index = findOverlayIndex(_src.name);
		if(index != -1)
			mOverlays[index] = _src;
		else
			mOverlays.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeOverlay(size_t _index)
	{
		mOverlays.erase( mOverlays.begin() + _index );
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllOverlays()
	{
		if(!mOverlays.empty())
		{
			mOverlays.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// OverlayController
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumOverlayControllers() const
	{
		return mOverlayControllers.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::OverlayController& SkyDef::getOverlayController(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mOverlayControllers.size(), "Index of OverlayController is out of range.");
		return mOverlayControllers[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::OverlayControllerIterator SkyDef::getOverlayControllerIterator() const
	{
		return (OverlayControllerIterator) mOverlayControllers;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertOverlayController(const OverlayController& _src)
	{
		mOverlayControllers.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeOverlayController(size_t _index)
	{
		GOTHOGRE_ASSERT(_index < mOverlayControllers.size(), "Index of OverlayController is out of range.");
		mOverlayControllers.erase(mOverlayControllers.begin() + _index);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllOverlayControllers()
	{
		if(!mOverlayControllers.empty())
		{
			mOverlayControllers.clear();
			++mStateCount;
		}
	}



	//---------------------------------------------------------------------------
	// Function
	//---------------------------------------------------------------------------
	size_t SkyDef::getNumFunctions() const
	{
		return mFunctions.size();
	}
	//---------------------------------------------------------------------------
	const SkyDef::Function& SkyDef::getFunction(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mFunctions.size(), "Index of Function is out of range.");
		return mFunctions[_index];
	}
	//---------------------------------------------------------------------------
	SkyDef::FunctionIterator SkyDef::getFunctionIterator() const
	{
		return (FunctionIterator) mFunctions;
	}
	//---------------------------------------------------------------------------
	const SkyDef::Function& SkyDef::getFunction(const String& _name) const
	{
		size_t index = getFunctionIndex(_name);
		return mFunctions[index];
	}
	//---------------------------------------------------------------------------
	bool SkyDef::hasFunction(const String& _name) const
	{
		return findFunctionIndex(_name) != -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::findFunctionIndex(const String& _name) const
	{
		for(size_t i = 0; i != mFunctions.size(); ++i)
		{
			if(StrUtil::equalsNoCase( mFunctions[i].name, _name ))
				return i;
		}
		return -1;
	}
	//---------------------------------------------------------------------------
	size_t SkyDef::getFunctionIndex(const String& _name) const
	{
		size_t index = findFunctionIndex( _name );
		if(index == -1)
			GOTHOGRE_EXCEPT("Function '" << _name << "' not found");

		return index;
	}
	//---------------------------------------------------------------------------
	void SkyDef::insertFunction(const Function& _src)
	{
		size_t index = findFunctionIndex(_src.name);
		if(index != -1)
			mFunctions[index] = _src;
		else
			mFunctions.push_back(_src);
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeFunction(size_t _index)
	{
		mFunctions.erase( mFunctions.begin() + _index );
		++mStateCount;
	}
	//---------------------------------------------------------------------------
	void SkyDef::removeAllFunctions()
	{
		if(!mFunctions.empty())
		{
			mFunctions.clear();
			++mStateCount;
		}
	}




	//------------------------------------------------------------------------
	// SkyDef
	//------------------------------------------------------------------------
	SkyDef::SkyDef(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
		const String& _group, bool _isManual, ManualResourceLoader* _loader)
	: ResourceEx(_creator, _name, _handle, _group, _isManual, _loader)
	{
		mFog = nullptr;
		mFarClippingPlane = nullptr;
		mBackground = nullptr;
	}
	//------------------------------------------------------------------------
	SkyDef::~SkyDef()
	{
		removeFog();
		removeAllFogControllers();
		removeFarClippingPlane();
		removeAllFarClippingPlaneControllers();
		removeBackground();
		removeAllBackgroundControllers();
		removeAllAtmospheres();
		removeAllAtmosphereControllers();
		removeAllBodies();
		removeAllBodyControllers();
		removeAllCloudLayers();
		removeAllCloudLayerControllers();
		removeAllMaterialControllers();
		removeAllSounds();
		removeAllSoundControllers();
		removeAllOverlays();
		removeAllOverlayControllers();
	}


} // namespace GothOgre