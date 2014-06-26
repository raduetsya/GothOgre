#include "Precompiled.h"
#include "SkyCalculator.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------------
	// Internal template ModifierSequence
	//--------------------------------------------------------------------------------
	template<typename T>
	SkyCalculator::ModifierSequence<T>::ModifierSequence()
	{
	}
	//--------------------------------------------------------------------------------
	template<typename T>
	SkyCalculator::ModifierSequence<T>::~ModifierSequence()
	{
	}
	//--------------------------------------------------------------------------------
	template<typename T>
	void SkyCalculator::ModifierSequence<T>::clear()
	{
		mModifierWeights.clear();
	}
	//--------------------------------------------------------------------------------
	template<typename T>
	void SkyCalculator::ModifierSequence<T>::add(const SkyDef::Modifier<T>& _modifier, Real _weight)
	{
		ModifierWeight mw;
		mw.modifier = _modifier;
		mw.weight = _weight;
		mModifierWeights.push_back( mw );
	}
	//--------------------------------------------------------------------------------
	template<typename T>
	T SkyCalculator::ModifierSequence<T>::getTotalStartingFrom( T _startValue ) const
	{
		T r = _startValue;

		// Go though all the added modifiers
		for( ModifierWeights::const_iterator it = mModifierWeights.begin();
			it != mModifierWeights.end(); ++it )
		{
			const SkyDef::Modifier<T>& modifier = it->modifier;
			Real weight = it->weight;
			modifier.transform(r);
		}

		return r;
	}
	//--------------------------------------------------------------------------------
	template<typename T>
	T SkyCalculator::ModifierSequence<T>::getTotal() const
	{
		T zero = SkyDef::Modifier<T>::IDENTITY.getAddend();
		return getTotalStartingFrom( zero );
	}
	//--------------------------------------------------------------------------------
	template<typename T>
	SkyDef::Modifier<T> SkyCalculator::ModifierSequence<T>::getTotalModifier() const
	{
		T zero = SkyDef::Modifier<T>::IDENTITY.getAddend();
		T one  = SkyDef::Modifier<T>::IDENTITY.getMultiplier();
		T f_zero = getTotalStartingFrom( zero );
		T f_one  = getTotalStartingFrom( one );
		T addend     = f_zero;
		T multiplier = f_one + f_zero * T(-1);
		return SkyDef::Modifier<T>( multiplier, addend );
	}



	//-----------------------------------------------------------------------
	// Main functions
	//-----------------------------------------------------------------------
	SkyCalculator::SkyCalculator( SceneManagerExImpl* _sceneManager )
	{
		mSceneManager           = _sceneManager;
		mFogIntern              = new FogIntern(this);
		mFarClippingPlaneIntern = new FarClippingPlaneIntern(this);
		mBackgroundIntern       = new BackgroundIntern(this);

		registerFrameListener();
	}
	//-----------------------------------------------------------------------
	SkyCalculator::~SkyCalculator()
	{
		delete mFogIntern;
		delete mFarClippingPlaneIntern;
		delete mBackgroundIntern;
	}
	//--------------------------------------------------------------------------------------
	const SkyCalculator::Fog& SkyCalculator::getFog() const
	{
		return *mFogIntern;
	}
	//--------------------------------------------------------------------------------------
	const SkyCalculator::FarClippingPlane& SkyCalculator::getFarClippingPlane() const
	{
		return *mFarClippingPlaneIntern;
	}
	//--------------------------------------------------------------------------------------
	const SkyCalculator::Background& SkyCalculator::getBackground() const
	{
		return *mBackgroundIntern;
	}
	//--------------------------------------------------------------------------------------
	SkyCalculator::AtmosphereIterator SkyCalculator::getAtmosphereIterator() const
	{
		return (AtmosphereIterator) mAtmosphereInterns;
	}
	//--------------------------------------------------------------------------------------
	SkyCalculator::BodyIterator SkyCalculator::getBodyIterator() const
	{
		return (BodyIterator) mBodyInterns;
	}
	//--------------------------------------------------------------------------------------
	SkyCalculator::CloudLayerIterator SkyCalculator::getCloudLayerIterator() const
	{
		return (CloudLayerIterator) mCloudLayerInterns;
	}
	//--------------------------------------------------------------------------------------
	SkyCalculator::MaterialIterator SkyCalculator::getMaterialIterator() const
	{
		return (MaterialIterator) mMaterialInterns;
	}
	//--------------------------------------------------------------------------------------
	SkyCalculator::SoundIterator SkyCalculator::getSoundIterator() const
	{
		return (SoundIterator) mSoundInterns;
	}
	//--------------------------------------------------------------------------------------
	void SkyCalculator::frameEnded( const FrameEvent& _evt )
	{
		deleteUnusedInCurrentFrame();
		resetUsedInCurrentFrame();
	}
	//-----------------------------------------------------------------------
	void SkyCalculator::calculate( Camera* _camera )
	{
		mCurrentCamera = _camera;

		calculateUnderwaterDepth();
		calculateIndoorDepth();

		findVisibleSkies();
		sortVisibleSkies();

		resetUsedByCurrentCamera();
		clearModSeqs();
		buildModSeqs();
		calculateModSeqs();
		calculateDerivedTransforms();
		calculateLightFilters();
	}




	//--------------------------------------------------------------------------------------
	// Searching visible sky objects (by bounding boxes)
	//--------------------------------------------------------------------------------------
	void SkyCalculator::findVisibleSkies()
	{
		mSkyWeights.clear();

		// The position of the camera
		const Vector3& worldPos = mCurrentCamera->getPositionForViewUpdate();

		// Find sky objects which bounding boxes contains the camera
		SceneManager::MovableObjectIterator moIt = mSceneManager->getMovableObjectIterator(
			SkySystem::FACTORY_TYPE_NAME);
		while(moIt.hasMoreElements())
		{
			Sky* sky = (Sky*) (moIt.getNext());
			if(sky->getVisible() && 
				(sky->getVisibilityFlags() & mSceneManager->_getCombinedVisibilityMask()))
			{
				Real innerPart = sky->getInnerPart();
				Real pointWeight = 0;
				if(sky->isSkyBoundingBoxInLocalSpace())
				{
					if(sky->isInScene())
					{
						Vector3 localPos = mCurrentCamera->getParentNode()->convertWorldToLocalPosition(worldPos);
						const AxisAlignedBox& localBBox = sky->getSkyBoundingBox();
						pointWeight = calculatePointWeight(localPos, localBBox, innerPart);
					}
				}
				else
				{
					const AxisAlignedBox& worldBBox = sky->getSkyBoundingBox();
					pointWeight = calculatePointWeight(worldPos, worldBBox, innerPart);
				}

				Real weight = sky->getWeight() * pointWeight;

				if(weight > 0)
				{
					mSkyWeights.push_back(SkyWeight());
					SkyWeight& sw = mSkyWeights.back();
					sw.sky = sky;
					sw.weight = weight;
					sw.pointWeight = pointWeight;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	// Calculates weight (0..1) of a point relative to a bounding box.
	// The function returns 0 if a point is outside a specified bounding box;
	// the function returns 1 if a point is inside the inner part of the bounding box;
	// the function returns a value from 0 to 1 elsewhere.
	Real SkyCalculator::calculatePointWeight(const Vector3& _pos, const AxisAlignedBox& _aab, Real _innerPart)
	{
		if(_aab.isInfinite())
			return 1;
		else if(_aab.isNull())
			return 0;

		Vector3 center = _aab.getCenter();
		Vector3 halfSize = _aab.getHalfSize();
		Real f[3];
		for(size_t i = 0; i != 3; ++i)
		{
			if(halfSize[i] == 0)
				return 0;
			f[i] = Math::Abs((_pos[i] - center[i]) / halfSize[i]);
		}

		Real fmax = std::max(f[0], std::max(f[1], f[2]));
		if(fmax >= 1)
			return 0;
		else if(fmax <= _innerPart)
			return 1;
		else // _innerPart < fmax < 1
			return (1 - fmax) / (1 - _innerPart);
	}
	//--------------------------------------------------------------------------------------
	void SkyCalculator::sortVisibleSkies()
	{
		// Sort the founded sky objects by priority (in ascending order)
		std::sort(mSkyWeights.begin(), mSkyWeights.end());
	}
	//--------------------------------------------------------------------------------------
	bool SkyCalculator::SkyWeight::operator <(const SkyWeight& _rhs) const 
	{
		return sky->getPriority() < _rhs.sky->getPriority();
	}



	//--------------------------------------------------------------------------------
	// Functions which enumerate over all aspects of SkyDef
	//--------------------------------------------------------------------------------
	void SkyCalculator::resetUsedByCurrentCamera()
	{
		mBackgroundIntern->setUsedByCurrentCamera( false );
		mFogIntern->setUsedByCurrentCamera( false );
		mFarClippingPlaneIntern->setUsedByCurrentCamera( false );

		for( AtmosphereInterns::iterator itA = mAtmosphereInterns.begin();
			itA != mAtmosphereInterns.end(); ++itA )
		{
			AtmosphereIntern& ai = *itA;
			ai.setUsedByCurrentCamera( false );
		}

		for( BodyInterns::iterator itB = mBodyInterns.begin();
			itB != mBodyInterns.end(); ++itB )
		{
			BodyIntern& bi = *itB;
			bi.setUsedByCurrentCamera( false );
		}

		for( CloudLayerInterns::iterator itC = mCloudLayerInterns.begin();
			itC != mCloudLayerInterns.end(); ++itC )
		{
			CloudLayerIntern& cli = *itC;
			cli.setUsedByCurrentCamera( false );
		}

		for( SoundInterns::iterator itS = mSoundInterns.begin();
			itS != mSoundInterns.end(); ++itS )
		{
			SoundIntern& si = *itS;
			si.setUsedByCurrentCamera( false );
		}

		for( MaterialInterns::iterator itM = mMaterialInterns.begin();
			itM != mMaterialInterns.end(); ++itM )
		{
			MaterialIntern& mi = *itM;
			mi.setUsedByCurrentCamera( false );
		}

		for( OverlayInterns::iterator itO = mOverlayInterns.begin();
			itO != mOverlayInterns.end(); ++itO )
		{
			OverlayIntern& oi = *itO;
			oi.setUsedByCurrentCamera( false );
		}

		for( SkyInterns::iterator itSK = mSkyInterns.begin();
			itSK != mSkyInterns.end(); ++itSK )
		{
			SkyIntern& ski = *itSK;
			ski.setUsedByCurrentCamera( false );
		}

		for( FunctionInterns::iterator itF = mFunctionInterns.begin();
			itF != mFunctionInterns.end(); ++itF )
		{
			FunctionIntern& fi = *itF;
			fi.setUsedByCurrentCamera( false );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::resetUsedInCurrentFrame()
	{
		if( mBackgroundIntern )
			mBackgroundIntern->setUsedInCurrentFrame( false );

		if( mFogIntern )
			mFogIntern->setUsedInCurrentFrame( false );

		if( mFarClippingPlaneIntern )
			mFarClippingPlaneIntern->setUsedInCurrentFrame( false );

		for( AtmosphereInterns::iterator itA = mAtmosphereInterns.begin();
			itA != mAtmosphereInterns.end(); ++itA )
		{
			AtmosphereIntern& ai = *itA;
			ai.setUsedInCurrentFrame( false );
		}

		for( BodyInterns::iterator itB = mBodyInterns.begin();
			itB != mBodyInterns.end(); ++itB )
		{
			BodyIntern& bi = *itB;
			bi.setUsedInCurrentFrame( false );
		}

		for( CloudLayerInterns::iterator itC = mCloudLayerInterns.begin();
			itC != mCloudLayerInterns.end(); ++itC )
		{
			CloudLayerIntern& cli = *itC;
			cli.setUsedInCurrentFrame( false );
		}

		for( SoundInterns::iterator itS = mSoundInterns.begin();
			itS != mSoundInterns.end(); ++itS )
		{
			SoundIntern& si = *itS;
			si.setUsedInCurrentFrame( false );
		}

		for( MaterialInterns::iterator itM = mMaterialInterns.begin();
			itM != mMaterialInterns.end(); ++itM )
		{
			MaterialIntern& mi = *itM;
			mi.setUsedInCurrentFrame( false );
		}

		for( OverlayInterns::iterator itO = mOverlayInterns.begin();
			itO != mOverlayInterns.end(); ++itO )
		{
			OverlayIntern& oi = *itO;
			oi.setUsedInCurrentFrame( false );
		}

		for( SkyInterns::iterator itSK = mSkyInterns.begin();
			itSK != mSkyInterns.end(); ++itSK )
		{
			SkyIntern& ski = *itSK;
			ski.setUsedInCurrentFrame( false );
		}

		for( FunctionInterns::iterator itF = mFunctionInterns.begin();
			itF != mFunctionInterns.end(); ++itF )
		{
			FunctionIntern& fi = *itF;
			fi.setUsedInCurrentFrame( false );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::deleteUnusedInCurrentFrame()
	{
		for( AtmosphereInterns::iterator itA = mAtmosphereInterns.begin();
			itA != mAtmosphereInterns.end(); )
		{
			AtmosphereInterns::iterator itA_cur = itA++;
			AtmosphereIntern& ai = *itA_cur;
			if( !ai.isUsedInCurrentFrame() )
			{
				mAtmosphereInterns.erase( itA_cur );
			}
		}

		for( BodyInterns::iterator itB = mBodyInterns.begin();
			itB != mBodyInterns.end(); )
		{
			BodyInterns::iterator itB_cur = itB++;
			BodyIntern& bi = *itB_cur;
			if( !bi.isUsedInCurrentFrame() )
			{
				mBodyInterns.erase( itB_cur );
			}
		}

		for( CloudLayerInterns::iterator itC = mCloudLayerInterns.begin();
			itC != mCloudLayerInterns.end(); )
		{
			CloudLayerInterns::iterator itC_cur = itC++;
			CloudLayerIntern& cli = *itC_cur;
			if( !cli.isUsedInCurrentFrame() )
			{
				mCloudLayerInterns.erase( itC_cur );
			}
		}

		for( SoundInterns::iterator itS = mSoundInterns.begin();
			itS != mSoundInterns.end(); )
		{
			SoundInterns::iterator itS_cur = itS++;
			SoundIntern& si = *itS_cur;
			if( !si.isUsedInCurrentFrame() )
			{
				mSoundInterns.erase( itS_cur );
			}
		}

		for( MaterialInterns::iterator itM = mMaterialInterns.begin();
			itM != mMaterialInterns.end(); )
		{
			MaterialInterns::iterator itM_cur = itM++;
			MaterialIntern& mi = *itM_cur;
			if( !mi.isUsedInCurrentFrame() )
			{
				mMaterialInterns.erase( itM_cur );
			}
		}

		for( OverlayInterns::iterator itO = mOverlayInterns.begin();
			itO != mOverlayInterns.end(); )
		{
			OverlayInterns::iterator itO_cur = itO++;
			OverlayIntern& oi = *itO_cur;
			if( !oi.isUsedInCurrentFrame() )
			{
				mOverlayInterns.erase( itO_cur );
			}
		}

		for( SkyInterns::iterator itSK = mSkyInterns.begin();
			itSK != mSkyInterns.end(); )
		{
			SkyInterns::iterator itSK_cur = itSK++;
			SkyIntern& ski = *itSK_cur;
			if( !ski.isUsedInCurrentFrame() )
			{
				mSkyInterns.erase( itSK_cur );
			}
		}

		for( FunctionInterns::iterator itF = mFunctionInterns.begin();
			itF != mFunctionInterns.end(); )
		{
			FunctionInterns::iterator itF_cur = itF++;
			FunctionIntern& fi = *itF_cur;
			if( !fi.isUsedInCurrentFrame() )
			{
				mFunctionInterns.erase( itF_cur );
			}
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::clearModSeqs()
	{
		if( mBackgroundIntern )
			mBackgroundIntern->clearModSeqs();

		if( mFogIntern )
			mFogIntern->clearModSeqs();

		if( mFarClippingPlaneIntern )
			mFarClippingPlaneIntern->clearModSeqs();

		for( AtmosphereInterns::iterator itA = mAtmosphereInterns.begin();
			itA != mAtmosphereInterns.end(); ++itA )
		{
			AtmosphereIntern& ai = *itA;
			ai.clearModSeqs();
		}

		for( BodyInterns::iterator itB = mBodyInterns.begin();
			itB != mBodyInterns.end(); ++itB )
		{
			BodyIntern& bi = *itB;
			bi.clearModSeqs();
		}

		for( CloudLayerInterns::iterator itC = mCloudLayerInterns.begin();
			itC != mCloudLayerInterns.end(); ++itC )
		{
			CloudLayerIntern& cli = *itC;
			cli.clearModSeqs();
		}

		for( SoundInterns::iterator itS = mSoundInterns.begin();
			itS != mSoundInterns.end(); ++itS )
		{
			SoundIntern& si = *itS;
			si.clearModSeqs();
		}

		for( MaterialInterns::iterator itM = mMaterialInterns.begin();
			itM != mMaterialInterns.end(); ++itM )
		{
			MaterialIntern& mi = *itM;
			mi.clearModSeqs();
		}

		for( OverlayInterns::iterator itO = mOverlayInterns.begin();
			itO != mOverlayInterns.end(); ++itO )
		{
			OverlayIntern& oi = *itO;
			oi.clearModSeqs();
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::calculateModSeqs()
	{
		if( mBackgroundIntern )
			mBackgroundIntern->calculateModSeqs();

		if( mFogIntern )
			mFogIntern->calculateModSeqs();

		if( mFarClippingPlaneIntern )
			mFarClippingPlaneIntern->calculateModSeqs();

		for( AtmosphereInterns::iterator itA = mAtmosphereInterns.begin();
			itA != mAtmosphereInterns.end(); ++itA )
		{
			AtmosphereIntern& ai = *itA;
			ai.calculateModSeqs();
		}

		for( BodyInterns::iterator itB = mBodyInterns.begin();
			itB != mBodyInterns.end(); ++itB )
		{
			BodyIntern& bi = *itB;
			bi.calculateModSeqs();
		}

		for( CloudLayerInterns::iterator itC = mCloudLayerInterns.begin();
			itC != mCloudLayerInterns.end(); ++itC )
		{
			CloudLayerIntern& cli = *itC;
			cli.calculateModSeqs();
		}

		for( SoundInterns::iterator itS = mSoundInterns.begin();
			itS != mSoundInterns.end(); ++itS )
		{
			SoundIntern& si = *itS;
			si.calculateModSeqs();
		}

		for( MaterialInterns::iterator itM = mMaterialInterns.begin();
			itM != mMaterialInterns.end(); ++itM )
		{
			MaterialIntern& mi = *itM;
			mi.calculateModSeqs();
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::calculateDerivedTransforms()
	{
		for( AtmosphereInterns::iterator itA = mAtmosphereInterns.begin();
			itA != mAtmosphereInterns.end(); ++itA )
		{
			AtmosphereIntern& ai = *itA;
			ai.calculateDerivedTransform();
		}

		for( BodyInterns::iterator itB = mBodyInterns.begin();
			itB != mBodyInterns.end(); ++itB )
		{
			BodyIntern& bi = *itB;
			bi.calculateDerivedTransform();
		}

		for( CloudLayerInterns::iterator itC = mCloudLayerInterns.begin();
			itC != mCloudLayerInterns.end(); ++itC )
		{
			CloudLayerIntern& cli = *itC;
			cli.calculateDerivedTransform();
		}

		for( SoundInterns::iterator itS = mSoundInterns.begin();
			itS != mSoundInterns.end(); ++itS )
		{
			SoundIntern& si = *itS;
			si.calculateDerivedTransform();
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqs()
	{
		for( SkyWeights::iterator it = mSkyWeights.begin();
			it != mSkyWeights.end(); ++it )
		{
			SkyWeight& sw = *it;
			buildModSeqsForSky( sw.sky->getSkyDef()->getName(), sw.weight );
		}
	}
	//--------------------------------------------------------------------------------
	const Real SkyCalculator::WEIGHT_EPSILON = 0.001f;
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForSky( const String& _skyName, Real _weight )
	{
		if( _weight < WEIGHT_EPSILON )
			return;

		bool alreadyWasUsedByCurrentCamera;
		SkyIntern& ski = getSkyIntern( _skyName, alreadyWasUsedByCurrentCamera );
		if( alreadyWasUsedByCurrentCamera )
			return;

		ski.setWeight( _weight );
		SkySystem::getSingleton()._addToVisibleList( mCurrentCamera, _skyName );
		SkyDefPtr skyDef = ski.getSkyDef();

		buildModSeqsForBodies     ( skyDef, _weight );
		buildModSeqsForOverlays   ( skyDef, _weight );
		buildModSeqsForCloudLayers( skyDef, _weight );
		buildModSeqsForAtmospheres( skyDef, _weight );
		buildModSeqsForBackground ( skyDef, _weight );
		buildModSeqsForFog        ( skyDef, _weight );
		buildModSeqsForFarClippingPlane( skyDef, _weight );
		buildModSeqsForSounds     ( skyDef, _weight );
		buildModSeqsForMaterials  ( skyDef, _weight );

		buildModSeqsForOverlaySkies();
	}





	//--------------------------------------------------------------------------------
	// Intern
	//--------------------------------------------------------------------------------
	SkyCalculator::Intern::Intern( SkyCalculator* _skyCalculator )
	{
		mSkyCalculator = _skyCalculator;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::Intern::setUsedInCurrentFrame(bool _used)
	{
		mUsedInCurrentFrame = _used;
	}
	//--------------------------------------------------------------------------------
	bool SkyCalculator::Intern::isUsedInCurrentFrame() const
	{
		return mUsedInCurrentFrame;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::Intern::setUsedByCurrentCamera(bool _used)
	{
		mUsedByCurrentCamera = _used;
	}
	//--------------------------------------------------------------------------------
	bool SkyCalculator::Intern::isUsedByCurrentCamera() const
	{
		return mUsedByCurrentCamera;
	}




	
	//--------------------------------------------------------------------------------
	// Processing of SkyDef::Background and SkyDef::BackgroundController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForBackground( const SkyDefPtr& _skyDef, Real _weight )
	{
		if( _skyDef->hasBackground() )
		{
			const SkyDef::Background& background = _skyDef->getBackground();
			BackgroundIntern& bi = getBackgroundIntern();
			bi.addModSeq( background, _weight );
		}

		SkyDef::BackgroundControllerIterator it = _skyDef->getBackgroundControllerIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::BackgroundController& ctrl = it.getNext();
			BackgroundIntern& bi = getBackgroundIntern();
			bi.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::BackgroundIntern& SkyCalculator::getBackgroundIntern()
	{
		BackgroundIntern& bi = *mBackgroundIntern;
		bi.setUsedByCurrentCamera( true );
		bi.setUsedInCurrentFrame ( true );
		return bi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::BackgroundIntern::BackgroundIntern( SkyCalculator* _skyCalculator )
		: Intern( _skyCalculator )
	{
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BackgroundIntern::clearModSeqs()
	{
		mColourModSeq.clear();
		mColourModSeq.add( ColourValueModifier( ColourValue::ZERO, ColourValue::Black ), 1);
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BackgroundIntern::addModSeq( const SkyDef::Background& _background, Real _weight )
	{
		mColourModSeq.add( ColourValueModifier( ColourValue::ZERO, _background.colour ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BackgroundIntern::addModSeq( const SkyDef::BackgroundController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::BackgroundController::Key::value, 
			&SkyDef::BackgroundController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BackgroundIntern::addModSeq( const SkyDef::BackgroundController::Key& _key1, const SkyDef::BackgroundController::Key& _key2, Real _ratio, Real _weight )
	{
		mColourModSeq.add( Interpolation::lerp( _key1.colourModifier, _key2.colourModifier, _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BackgroundIntern::calculateModSeqs()
	{
		colour = mColourModSeq.getTotal();
	}




	//--------------------------------------------------------------------------------
	// Processing of SkyDef::Fog and SkyDef::FogController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForFog( const SkyDefPtr& _skyDef, Real _weight )
	{
		if( _skyDef->hasFog() )
		{
			const SkyDef::Fog& fog = _skyDef->getFog();
			FogIntern& fgi = getFogIntern();
			fgi.addModSeq( fog, _weight );
		}

		SkyDef::FogControllerIterator it = _skyDef->getFogControllerIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::FogController& ctrl = it.getNext();
			FogIntern& fgi = getFogIntern();
			fgi.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::FogIntern& SkyCalculator::getFogIntern()
	{
		FogIntern& fgi = *mFogIntern;
		fgi.setUsedByCurrentCamera( true );
		fgi.setUsedInCurrentFrame ( true );
		return fgi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::FogIntern::FogIntern( SkyCalculator* _skyCalculator )
		: Intern( _skyCalculator )
	{
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FogIntern::clearModSeqs()
	{
		mColourModSeq.clear();
		mNearDistanceModSeq.clear();
		mFarDistanceModSeq.clear();

		mColourModSeq.add( ColourValueModifier( ColourValue::ZERO, ColourValue::Black), 1 );
		mNearDistanceModSeq.add( RealModifier( 0, Math::POS_INFINITY), 1 );
		mFarDistanceModSeq.add( RealModifier(0, Math::POS_INFINITY), 1 );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FogIntern::addModSeq( const SkyDef::Fog& _fog, Real _weight )
	{
		mColourModSeq.add      ( ColourValueModifier( ColourValue::ZERO, _fog.colour       ), _weight );
		mNearDistanceModSeq.add( RealModifier       (                 0, _fog.nearDistance ), _weight );
		mFarDistanceModSeq.add ( RealModifier       (                 0, _fog.farDistance  ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FogIntern::addModSeq( const SkyDef::FogController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::FogController::Key::value, 
			&SkyDef::FogController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FogIntern::addModSeq( const SkyDef::FogController::Key& _key1, const SkyDef::FogController::Key& _key2, Real _ratio, Real _weight )
	{
		mColourModSeq.add      ( Interpolation::lerp( _key1.colourModifier,       _key2.colourModifier,       _ratio), _weight );
		mNearDistanceModSeq.add( Interpolation::lerp( _key1.nearDistanceModifier, _key2.nearDistanceModifier, _ratio), _weight );
		mFarDistanceModSeq.add ( Interpolation::lerp( _key1.farDistanceModifier,  _key2.farDistanceModifier,  _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FogIntern::calculateModSeqs()
	{
		colour       = mColourModSeq.getTotal();
		nearDistance = mNearDistanceModSeq.getTotal();
		farDistance  = mFarDistanceModSeq.getTotal();
	}




	//--------------------------------------------------------------------------------
	// Processing of SkyDef::FarClippingPlane and SkyDef::FarClippingPlaneController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForFarClippingPlane( const SkyDefPtr& _skyDef, Real _weight )
	{
		if( _skyDef->hasFarClippingPlane() )
		{
			const SkyDef::FarClippingPlane& farClippingPlane = _skyDef->getFarClippingPlane();
			FarClippingPlaneIntern& fgi = getFarClippingPlaneIntern();
			fgi.addModSeq( farClippingPlane, _weight );
		}

		SkyDef::FarClippingPlaneControllerIterator it = _skyDef->getFarClippingPlaneControllerIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::FarClippingPlaneController& ctrl = it.getNext();
			FarClippingPlaneIntern& fgi = getFarClippingPlaneIntern();
			fgi.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::FarClippingPlaneIntern& SkyCalculator::getFarClippingPlaneIntern()
	{
		FarClippingPlaneIntern& fgi = *mFarClippingPlaneIntern;
		fgi.setUsedByCurrentCamera( true );
		fgi.setUsedInCurrentFrame ( true );
		return fgi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::FarClippingPlaneIntern::FarClippingPlaneIntern( SkyCalculator* _skyCalculator )
		: Intern( _skyCalculator )
	{
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FarClippingPlaneIntern::clearModSeqs()
	{
		mDistanceModSeq.clear();
		mDistanceModSeq.add( RealModifier(0, Math::POS_INFINITY), 1 );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FarClippingPlaneIntern::addModSeq( const SkyDef::FarClippingPlane& _farClippingPlane, Real _weight )
	{
		mDistanceModSeq.add ( RealModifier( 0, _farClippingPlane.distance  ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FarClippingPlaneIntern::addModSeq( const SkyDef::FarClippingPlaneController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::FarClippingPlaneController::Key::value, 
			&SkyDef::FarClippingPlaneController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FarClippingPlaneIntern::addModSeq( const SkyDef::FarClippingPlaneController::Key& _key1, const SkyDef::FarClippingPlaneController::Key& _key2, Real _ratio, Real _weight )
	{
		mDistanceModSeq.add ( Interpolation::lerp( _key1.distanceModifier,  _key2.distanceModifier,  _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::FarClippingPlaneIntern::calculateModSeqs()
	{
		distance  = mDistanceModSeq.getTotal();
	}




	//--------------------------------------------------------------------------------
	// Processing of SkyDef::Atmosphere and SkyDef::AtmosphereController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForAtmospheres( const SkyDefPtr& _skyDef, Real _weight )
	{
		SkyDef::AtmosphereIterator it = _skyDef->getAtmosphereIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::Atmosphere& atmosphere = it.getNext();
			AtmosphereIntern& ai = getAtmosphereIntern( atmosphere );
			ai.addModSeq( atmosphere, _weight );
		}

		SkyDef::AtmosphereControllerIterator it2 = _skyDef->getAtmosphereControllerIterator();
		while( it2.hasMoreElements() )
		{
			SkyDef::AtmosphereController& ctrl = it2.getNext();
			AtmosphereIntern& ai = getAtmosphereIntern( ctrl.atmosphereName );
			ai.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::AtmosphereIntern& SkyCalculator::getAtmosphereIntern( const SkyDef::Atmosphere& _atmosphere )
	{
		AtmosphereInterns::iterator it;
		for(it = mAtmosphereInterns.begin(); it != mAtmosphereInterns.end(); ++it)
		{
			AtmosphereIntern& ai = *it;
			if( UnicodeUtil::equalsNoCase(ai.name, _atmosphere.name) )
				break;
		}

		if( it == mAtmosphereInterns.end() )
		{
			mAtmosphereInterns.push_back( AtmosphereIntern( this, _atmosphere) );
			it = mAtmosphereInterns.end();
			--it;
		}

		AtmosphereIntern& ai = *it;
		ai.setUsedByCurrentCamera( true );
		ai.setUsedInCurrentFrame ( true );
		return ai;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::AtmosphereIntern& SkyCalculator::getAtmosphereIntern( const String& _atmosphereName )
	{
		AtmosphereInterns::iterator it;
		for(it = mAtmosphereInterns.begin(); it != mAtmosphereInterns.end(); ++it)
		{
			AtmosphereIntern& ai = *it;
			if( UnicodeUtil::equalsNoCase(ai.name, _atmosphereName) )
				break;
		}

		if( it == mAtmosphereInterns.end() )
		{
			GOTHOGRE_EXCEPT( "Atmosphere '" << _atmosphereName << "' not found.");
		}

		AtmosphereIntern& ai = *it;
		ai.setUsedByCurrentCamera( true );
		ai.setUsedInCurrentFrame ( true );
		return ai;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::AtmosphereIntern::AtmosphereIntern( SkyCalculator* _skyCalculator, const SkyDef::Atmosphere& _atmosphere )
		: Intern( _skyCalculator )
	{
		name  = _atmosphere.name;
		layer = _atmosphere.layer;
		generateGeometry( _atmosphere.radius, _atmosphere.minAltitude );
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::generateGeometry( Real _radius, Degree _minAltitude )
	{
		// Minimum and maximum altitudes (in radians)
		Real maxAltitude = Math::HALF_PI;
		Real minAltitude = _minAltitude.valueRadians();
		if( Math::Abs(minAltitude + Math::HALF_PI) < 0.001 )
			minAltitude = -Math::HALF_PI;

		// Level of details - number of segments per Pi/2
		static const size_t NUM_SEGMENTS_PER_HALF_PI = 10;

		size_t numVertSegments = NUM_SEGMENTS_PER_HALF_PI * 4;
		size_t numHorSegments = (size_t) Math::Ceil( (maxAltitude - minAltitude) / Math::HALF_PI * NUM_SEGMENTS_PER_HALF_PI );

		// Number of vertices and number of faces 
		// depending on theses numbers of segments
		size_t numPoints = numVertSegments * (numHorSegments + 1);

		if(maxAltitude == 90)
			numPoints -= numVertSegments - 1;
		if(minAltitude == -90)
			numPoints -= numVertSegments - 1;

		size_t numFaces = numVertSegments * numHorSegments;
		size_t numIndices = numFaces * 3;

		// Reserve buffers for vertices and indices
		points.resize( 0 );
		points.reserve( numPoints );
		indices.resize( 0 );
		indices.reserve( numIndices );

		// Calculate vertices and indices (faces)
		size_t rowStart = -1;
		size_t rowEnd = -1;
		for(size_t ia = 0; ia != numHorSegments + 1; ++ia)
		{
			// A row contains vertices with the same Z coordinate
			size_t prevRowStart = rowStart;
			size_t prevRowEnd = rowEnd;
			rowStart = points.size();

			// Vertices
			Real altitude = ia * (maxAltitude - minAltitude) / (numHorSegments + 1) + minAltitude;
			Real cos_altitude = Math::Cos( altitude );
			Real sin_altitude = Math::Sin( altitude );
			Real r = _radius * cos_altitude;
			Real z = _radius * sin_altitude;

			if( cos_altitude < 0.001 )
			{
				Real x = 0;
				Real y = 0;
				points.push_back( Vector3(x, y, z) );
			}
			else
			{
				for(size_t ib = 0; ib != numVertSegments; ++ib)
				{
					Real beta = ib * Math::TWO_PI / numVertSegments;
					Real x = r * Math::Cos(beta);
					Real y = r * Math::Sin(beta);
					points.push_back(Vector3(x, y, z));
					// normal = Vector3(-x, -y, -z).normalisedCopy();
				}
			}

			rowEnd = points.size();

			// Indices
			if(prevRowStart != -1)
			{
				if(prevRowEnd - prevRowStart == 1)
				{
					size_t numVertsInRow = rowEnd - rowStart;
					for(size_t ib = 0; ib != numVertsInRow; ++ib)
					{
						size_t ib1 = (ib + 1) % numVertsInRow;
						indices.push_back( prevRowStart       );
						indices.push_back(     rowStart + ib  );
						indices.push_back(     rowStart + ib1 );
					}					
				}
				else if(rowEnd - rowStart == 1)
				{
					size_t numVertsInRow = prevRowEnd - prevRowStart;
					for(size_t ib = 0; ib != numVertsInRow; ++ib)
					{
						size_t ib1 = (ib + 1) % numVertsInRow;
						indices.push_back( prevRowStart + ib  );
						indices.push_back( prevRowStart + ib1 );
						indices.push_back(     rowStart       );
					}					
				}
				else
				{
					size_t numVertsInRow = rowEnd - rowStart;
					GOTHOGRE_ASSERT( 
						prevRowEnd - prevRowStart == numVertsInRow 
						&& prevRowEnd == rowStart, 
						"Internal error in the atmosphere's mesh generation's algorithm." );

					for(size_t ib = 0; ib != numVertsInRow; ++ib)
					{
						size_t ib1 = (ib + 1) % numVertsInRow;
						indices.push_back( prevRowStart + ib  );
						indices.push_back( prevRowStart + ib1 );
						indices.push_back(     rowStart + ib1 );
						indices.push_back( prevRowStart + ib  );
						indices.push_back(     rowStart + ib1 );
						indices.push_back(     rowStart + ib  );
					}
				}
			}
		}

		GOTHOGRE_ASSERT( points.size() == numPoints
			&& indices.size() != numIndices,
			"Internal error in the atmosphere's mesh generation's algorithm." );

		// Prepare buffer for colours
		colours.resize( 0 );
		colours.resize( numPoints );
		mColourModSeqs.resize( 0 );
		mColourModSeqs.resize( numPoints );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::clearModSeqs()
	{
		mAmbientLightModSeq.clear();
		mAmbientLightFilterModSeq.clear();
		mDiffuseLightFilterModSeq.clear();
		mSpecularLightFilterModSeq.clear();

		for(size_t i = 0; i != mColourModSeqs.size(); ++i)
			mColourModSeqs[i].clear();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::addModSeq( const SkyDef::Atmosphere& _atmosphere, Real _weight )
	{
		mAmbientLightModSeq.add       ( ColourValueModifier( ColourValue::ZERO, _atmosphere.ambientLight        ), _weight );
		mAmbientLightFilterModSeq.add ( ColourValueModifier( ColourValue::ZERO, _atmosphere.ambientLightFilter  ), _weight );
		mDiffuseLightFilterModSeq.add ( ColourValueModifier( ColourValue::ZERO, _atmosphere.diffuseLightFilter  ), _weight );
		mSpecularLightFilterModSeq.add( ColourValueModifier( ColourValue::ZERO, _atmosphere.specularLightFilter ), _weight );

		for(size_t i = 0; i != mColourModSeqs.size(); ++i)
			mColourModSeqs[i].add( ColourValueModifier(ColourValue::ZERO, _atmosphere.colour ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::addModSeq( const SkyDef::AtmosphereController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );

		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::AtmosphereController::Key::value, 
			&SkyDef::AtmosphereController::Key::period );

		if( fcpr.success )
		{
			size_t lb = fcpr.lowerBound;
			size_t ub = fcpr.upperBound;
			Real ratio = fcpr.ratio;

			const FunctionIntern& fi = mSkyCalculator->getFunctionIntern( _ctrl.functionName );
			if( !fi.getBodyName().empty() )
			{
				const BodyIntern& bi = mSkyCalculator->getBodyIntern( fi.getBodyName() );
				Matrix4 biMat = bi.getDerivedTransform();
				Matrix4 aiMat = getDerivedTransform();
				Vector3 dirToBody = aiMat.inverse() * biMat.getTrans();
				addModSeq( _ctrl.keys[lb], _ctrl.keys[ub], ratio, _weight, dirToBody );
			}
			else
			{
				addModSeq( _ctrl.keys[lb], _ctrl.keys[ub], ratio, _weight );
			}
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::addModSeq_Base( const SkyDef::AtmosphereController::Key& _key1, const SkyDef::AtmosphereController::Key& _key2, Real _ratio, Real _weight )
	{
		mAmbientLightModSeq.add       ( Interpolation::lerp( _key1.ambientLightModifier,        _key2.ambientLightModifier,        _ratio), _weight );
		mAmbientLightFilterModSeq.add ( Interpolation::lerp( _key1.ambientLightFilterModifier,  _key2.ambientLightFilterModifier,  _ratio), _weight );
		mDiffuseLightFilterModSeq.add ( Interpolation::lerp( _key1.diffuseLightFilterModifier,  _key2.diffuseLightFilterModifier,  _ratio), _weight );
		mSpecularLightFilterModSeq.add( Interpolation::lerp( _key1.specularLightFilterModifier, _key2.specularLightFilterModifier, _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::addModSeq( const SkyDef::AtmosphereController::Key& _key1, const SkyDef::AtmosphereController::Key& _key2, Real _ratio, Real _weight )
	{
		addModSeq_Base( _key1, _key2, _ratio, _weight );

		for( size_t i = 0; i != mColourModSeqs.size(); ++i )
		{
			SkyDef::ColourValueModifier clrMod1 = _key1.colourModifier;
			SkyDef::ColourValueModifier clrMod2 = _key2.colourModifier;
			mColourModSeqs[i].add( Interpolation::lerp( clrMod1, clrMod2, _ratio), _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::addModSeq( const SkyDef::AtmosphereController::Key& _key1, const SkyDef::AtmosphereController::Key& _key2, Real _ratio, Real _weight, const Vector3& _directionToBody )
	{
		addModSeq_Base( _key1, _key2, _ratio, _weight );

		for( size_t i = 0; i != mColourModSeqs.size(); ++i )
		{
			Vector3& pt = points[i];
			Degree angle = pt.angleBetween(_directionToBody);

			SkyDef::ColourValueModifier clrMod1 = 
				getColourValueModifierFromKey( _key1, angle );

			SkyDef::ColourValueModifier clrMod2 = 
				getColourValueModifierFromKey( _key2, angle );

			mColourModSeqs[i].add( Interpolation::lerp( clrMod1, clrMod2, _ratio), _weight );
		}
	}

	//--------------------------------------------------------------------------------
	SkyDef::ColourValueModifier SkyCalculator::AtmosphereIntern::getColourValueModifierFromKey( 
		const SkyDef::AtmosphereController::Key& _key, Degree angle )
	{
		SkyDef::ColourValueModifier clrMod;
		if( _key.angleColourModifierPairs.size() == 0 )
		{
			clrMod = _key.colourModifier;
		}
		else
		{
			Interpolation::FindClosestPointsResult<Degree>
				fcpr = Interpolation::findClosestPoints( angle,
				_key.angleColourModifierPairs,
				&SkyDef::AtmosphereController::AngleColourModifierPair::first );

			SkyDef::ColourValueModifier clrModA = _key.angleColourModifierPairs[fcpr.lowerBound].second;
			SkyDef::ColourValueModifier clrModB = _key.angleColourModifierPairs[fcpr.upperBound].second;
			clrMod = Interpolation::lerp( clrModA, clrModB, fcpr.ratio );
		}
		return clrMod;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::calculateModSeqs()
	{
		ambientLight = mAmbientLightModSeq.getTotal();
		mAmbientLightFilter = mAmbientLightFilterModSeq.getTotal();
		mDiffuseLightFilter = mDiffuseLightFilterModSeq.getTotal();
		mSpecularLightFilter = mSpecularLightFilterModSeq.getTotal();

		for( size_t i = 0; i != mColourModSeqs.size(); ++i )
		{
			colours[i] = mColourModSeqs[i].getTotal();
		}
	}
	//--------------------------------------------------------------------------------
	ColourValue SkyCalculator::AtmosphereIntern::getAmbientLightFilter() const
	{
		return mAmbientLightFilter;
	}
	//--------------------------------------------------------------------------------
	ColourValue SkyCalculator::AtmosphereIntern::getDiffuseLightFilter() const
	{
		return mDiffuseLightFilter;
	}
	//--------------------------------------------------------------------------------
	ColourValue SkyCalculator::AtmosphereIntern::getSpecularLightFilter() const
	{
		return mSpecularLightFilter;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::AtmosphereIntern::calculateDerivedTransform()
	{
		Matrix4 mat = getDerivedTransform();
		mat.decomposition( derivedPosition, derivedScale, derivedOrientation );
	}
	//--------------------------------------------------------------------------------
	Matrix4 SkyCalculator::AtmosphereIntern::getDerivedTransform() const
	{
		// Scene directions.
		const Vector3& eastDir  = mSkyCalculator->mSceneManager->getEastDirection();
		const Vector3& northDir = mSkyCalculator->mSceneManager->getNorthDirection();
		const Vector3& upDir    = mSkyCalculator->mSceneManager->getUpDirection();

		// Generate a matrix from the scene directions.
		Matrix3 mat3;
		mat3.FromAxes( eastDir, northDir, upDir );
		Matrix4 mat4( mat3 );
		Camera* camera = mSkyCalculator->mCurrentCamera;
		mat4.setTrans( camera->getPositionForViewUpdate() );
		return mat4;
	}

	



	//--------------------------------------------------------------------------------
	// Processing of SkyDef::Body and SkyDef::BodyController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForBodies( const SkyDefPtr& _skyDef, Real _weight )
	{
		SkyDef::BodyIterator it = _skyDef->getBodyIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::Body& body = it.getNext();
			BodyIntern& bi = getBodyIntern( body );
			bi.addModSeq( body, _weight );
		}

		SkyDef::BodyControllerIterator it2 = _skyDef->getBodyControllerIterator();
		while( it2.hasMoreElements() )
		{
			SkyDef::BodyController& ctrl = it2.getNext();
			BodyIntern& bi = getBodyIntern( ctrl.bodyName );
			bi.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::BodyIntern& SkyCalculator::getBodyIntern( const SkyDef::Body& _body )
	{
		BodyInterns::iterator it;
		for(it = mBodyInterns.begin();	it != mBodyInterns.end(); ++it)
		{
			BodyIntern& bi = *it;
			if( UnicodeUtil::equalsNoCase(bi.name, _body.name))
				break;
		}

		if( it == mBodyInterns.end() )
		{
			mBodyInterns.push_back( BodyIntern(this, _body) );
			it = mBodyInterns.end();
			--it;
		}

		BodyIntern& bi = *it;
		bi.setUsedByCurrentCamera( true );
		bi.setUsedInCurrentFrame ( true );
		return bi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::BodyIntern& SkyCalculator::getBodyIntern( const String& _bodyName )
	{
		BodyInterns::iterator it;
		for(it = mBodyInterns.begin();	it != mBodyInterns.end(); ++it)
		{
			BodyIntern& bi = *it;
			if( UnicodeUtil::equalsNoCase(bi.name, _bodyName))
				break;
		}

		if( it == mBodyInterns.end() )
		{
			GOTHOGRE_EXCEPT( "Body '" << _bodyName << "' not found.");
		}

		BodyIntern& bi = *it;
		bi.setUsedByCurrentCamera( true );
		bi.setUsedInCurrentFrame ( true );
		return bi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::BodyIntern::BodyIntern( SkyCalculator* _skyCalculator, const SkyDef::Body& _body )
		: Intern( _skyCalculator )
	{
		name = _body.name;
		layer = _body.layer;
		mesh = _body.mesh;
		mtlName = _body.mtlName;
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BodyIntern::clearModSeqs()
	{
		mAltitudeModSeq.clear();
		mAzimuthModSeq.clear();
		mDistanceModSeq.clear();
		mPositionModSeq.clear();
		mEulerAnglesDegModSeq.clear();
		mScaleModSeq.clear();
		mAmbientLightModSeq.clear();
		mDiffuseLightModSeq.clear();
		mSpecularLightModSeq.clear();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BodyIntern::addModSeq( const SkyDef::Body& _body, Real _weight )
	{
		mAltitudeModSeq.add      ( DegreeModifier     ( Degree(0), _body.altitude       ), _weight );
		mAzimuthModSeq.add       ( DegreeModifier     ( Degree(0), _body.azimuth        ), _weight );
		mDistanceModSeq.add      ( RealModifier       ( 0, _body.distance       ), _weight );
		mPositionModSeq.add      ( Vector3Modifier    ( Vector3::ZERO, _body.position       ), _weight );
		mEulerAnglesDegModSeq.add( Vector3Modifier    ( Vector3::ZERO, _body.eulerAnglesDeg ), _weight );
		mScaleModSeq.add         ( Vector3Modifier    ( Vector3::ZERO, _body.scale          ), _weight );
		mAmbientLightModSeq.add  ( ColourValueModifier( ColourValue::ZERO, _body.ambientLight   ), _weight );
		mDiffuseLightModSeq.add  ( ColourValueModifier( ColourValue::ZERO, _body.diffuseLight   ), _weight );
		mSpecularLightModSeq.add ( ColourValueModifier( ColourValue::ZERO, _body.specularLight  ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BodyIntern::addModSeq( const SkyDef::BodyController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::BodyController::Key::value, 
			&SkyDef::BodyController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BodyIntern::addModSeq( const SkyDef::BodyController::Key& _key1, const SkyDef::BodyController::Key& _key2, Real _ratio, Real _weight )
	{
		mAltitudeModSeq.add      ( Interpolation::lerp( _key1.altitudeModifier,       _key2.altitudeModifier,       _ratio), _weight );
		mAzimuthModSeq.add       ( Interpolation::lerp( _key1.azimuthModifier,        _key2.azimuthModifier,        _ratio), _weight );
		mDistanceModSeq.add      ( Interpolation::lerp( _key1.distanceModifier,       _key2.distanceModifier,       _ratio), _weight );
		mPositionModSeq.add      ( Interpolation::lerp( _key1.positionModifier,       _key2.positionModifier,       _ratio), _weight );
		mEulerAnglesDegModSeq.add( Interpolation::lerp( _key1.eulerAnglesDegModifier, _key2.eulerAnglesDegModifier, _ratio), _weight );
		mScaleModSeq.add         ( Interpolation::lerp( _key1.scaleModifier,          _key2.scaleModifier,          _ratio), _weight );
		mAmbientLightModSeq.add  ( Interpolation::lerp( _key1.ambientLightModifier,   _key2.ambientLightModifier,   _ratio), _weight );
		mDiffuseLightModSeq.add  ( Interpolation::lerp( _key1.diffuseLightModifier,   _key2.diffuseLightModifier,   _ratio), _weight );
		mSpecularLightModSeq.add ( Interpolation::lerp( _key1.specularLightModifier,  _key2.specularLightModifier,  _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BodyIntern::calculateModSeqs()
	{
		altitude       = mAltitudeModSeq.getTotal();
		azimuth        = mAzimuthModSeq.getTotal();
		distance       = mDistanceModSeq.getTotal();
		position       = mPositionModSeq.getTotal();
		eulerAnglesDeg = mEulerAnglesDegModSeq.getTotal();
		scale          = mScaleModSeq.getTotal();
		ambientLight   = mAmbientLightModSeq.getTotal();
		diffuseLight   = mDiffuseLightModSeq.getTotal();
		specularLight  = mSpecularLightModSeq.getTotal();
	}
	//--------------------------------------------------------------------------------
	Degree SkyCalculator::BodyIntern::getAltitude() const
	{
		return mAltitudeModSeq.getTotal();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::BodyIntern::calculateDerivedTransform()
	{
		Matrix4 mat = getDerivedTransform();
		mat.decomposition( derivedPosition, derivedScale, derivedOrientation );
	}
	//--------------------------------------------------------------------------------
	Matrix4 SkyCalculator::BodyIntern::getDerivedTransform() const
	{
		Matrix4 matrix[6];

		// #0: scaling
		matrix[0] = Matrix4::IDENTITY;
		matrix[0].setScale( mScaleModSeq.getTotal() );

		// #1: first rotation using euler angles.
		Vector3 eaDeg = mEulerAnglesDegModSeq.getTotal();
		Radian eaRad[3];
		eaRad[0] = (Radian) (Degree) eaDeg[0];
		eaRad[1] = (Radian) (Degree) eaDeg[1];
		eaRad[2] = (Radian) (Degree) eaDeg[2];
		Matrix3 eaMat;
		eaMat.FromEulerAnglesXYZ( eaRad[0], eaRad[1], eaRad[2] );
		matrix[1] = Matrix4( eaMat );

		// #2: first translation using explicit position value.
		matrix[2].makeTrans( mPositionModSeq.getTotal() );

		// Scene directions
		const Vector3& eastDir  = mSkyCalculator->mSceneManager->getEastDirection();
		const Vector3& northDir = mSkyCalculator->mSceneManager->getNorthDirection();
		const Vector3& upDir    = mSkyCalculator->mSceneManager->getUpDirection();

		// #3: second translation using distance of the body from the observer.
		Real dist = mDistanceModSeq.getTotal();
		matrix[3].makeTrans( dist * (-upDir) );

		// #4: rotation using altitude and azimuth
		Matrix3 altMat, azmMat;
		Degree altDeg = mAltitudeModSeq.getTotal();
		Radian altRad = (Radian) altDeg;
		altMat.FromAxisAngle( eastDir, altRad );
		Degree azmDeg = mAzimuthModSeq.getTotal();
		Radian azmRad = (Radian) azmDeg;
		azmMat.FromAxisAngle( upDir, -azmRad );
		matrix[4] = azmMat * altMat;

		// #5: third translation to the current camera's position
		Camera* camera = mSkyCalculator->mCurrentCamera;
		matrix[5].makeTrans( camera->getPositionForViewUpdate() );

		// Final multiplication of all these transformations
		Matrix4 finalMat = matrix[5] * matrix[4] * matrix[3] * matrix[2] * matrix[1] * matrix[0];
		return finalMat;
	}





	//--------------------------------------------------------------------------------
	// Processing of SkyDef::CloudLayer and SkyDef::CloudLayerController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForCloudLayers( const SkyDefPtr& _skyDef, Real _weight )
	{
		SkyDef::CloudLayerIterator it = _skyDef->getCloudLayerIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::CloudLayer& cloudLayer = it.getNext();
			CloudLayerIntern& cli = getCloudLayerIntern( cloudLayer );
			cli.addModSeq( cloudLayer, _weight );
		}

		SkyDef::CloudLayerControllerIterator it2 = _skyDef->getCloudLayerControllerIterator();
		while( it2.hasMoreElements() )
		{
			SkyDef::CloudLayerController& ctrl = it2.getNext();
			CloudLayerIntern& cli = getCloudLayerIntern( ctrl.cloudLayerName );
			cli.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::CloudLayerIntern& SkyCalculator::getCloudLayerIntern( const SkyDef::CloudLayer& _cloudLayer )
	{
		CloudLayerInterns::iterator it;
		for(it = mCloudLayerInterns.begin();	it != mCloudLayerInterns.end(); ++it)
		{
			CloudLayerIntern& cli = *it;
			if( UnicodeUtil::equalsNoCase(cli.name, _cloudLayer.name) )
				break;
		}

		if( it == mCloudLayerInterns.end() )
		{
			mCloudLayerInterns.push_back( CloudLayerIntern(this, _cloudLayer) );
			it = mCloudLayerInterns.end();
			--it;
		}

		CloudLayerIntern& cli = *it;
		cli.setUsedByCurrentCamera( true );
		cli.setUsedInCurrentFrame ( true );
		return cli;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::CloudLayerIntern& SkyCalculator::getCloudLayerIntern( const String& _cloudLayerName )
	{
		CloudLayerInterns::iterator it;
		for(it = mCloudLayerInterns.begin();	it != mCloudLayerInterns.end(); ++it)
		{
			CloudLayerIntern& cli = *it;
			if( UnicodeUtil::equalsNoCase(cli.name, _cloudLayerName) )
				break;
		}

		if( it == mCloudLayerInterns.end() )
		{
			GOTHOGRE_EXCEPT( "CloudLayer '" << _cloudLayerName << "' not found.");
		}

		CloudLayerIntern& cli = *it;
		cli.setUsedByCurrentCamera( true );
		cli.setUsedInCurrentFrame ( true );
		return cli;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::CloudLayerIntern::CloudLayerIntern( SkyCalculator* _skyCalculator, const SkyDef::CloudLayer& _cloudLayer )
		: Intern( _skyCalculator )
	{
		name = _cloudLayer.name;
		layer = _cloudLayer.layer;
		mesh = _cloudLayer.mesh;
		mtlName = _cloudLayer.mtlName;
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::CloudLayerIntern::clearModSeqs()
	{
		mPositionModSeq.clear();
		mEulerAnglesDegModSeq.clear();
		mScaleModSeq.clear();
		mAmbientLightModSeq.clear();
		mDiffuseLightModSeq.clear();
		mSpecularLightModSeq.clear();
		mAmbientLightFilterModSeq.clear();
		mDiffuseLightFilterModSeq.clear();
		mSpecularLightFilterModSeq.clear();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::CloudLayerIntern::addModSeq( const SkyDef::CloudLayer& _cloudLayer, Real _weight )
	{
		mPositionModSeq.add            ( Vector3Modifier    ( Vector3::ZERO,     _cloudLayer.position            ), _weight );
		mEulerAnglesDegModSeq.add      ( Vector3Modifier    ( Vector3::ZERO,     _cloudLayer.eulerAnglesDeg      ), _weight );
		mScaleModSeq.add               ( Vector3Modifier    ( Vector3::ZERO,     _cloudLayer.scale               ), _weight );
		mAmbientLightModSeq.add        ( ColourValueModifier( ColourValue::ZERO, _cloudLayer.ambientLight        ), _weight );
		mDiffuseLightModSeq.add        ( ColourValueModifier( ColourValue::ZERO, _cloudLayer.diffuseLight        ), _weight );
		mSpecularLightModSeq.add       ( ColourValueModifier( ColourValue::ZERO, _cloudLayer.specularLight       ), _weight );
		mAmbientLightFilterModSeq.add  ( ColourValueModifier( ColourValue::ZERO, _cloudLayer.ambientLightFilter  ), _weight );
		mDiffuseLightFilterModSeq.add  ( ColourValueModifier( ColourValue::ZERO, _cloudLayer.diffuseLightFilter  ), _weight );
		mSpecularLightFilterModSeq.add ( ColourValueModifier( ColourValue::ZERO, _cloudLayer.specularLightFilter ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::CloudLayerIntern::addModSeq( const SkyDef::CloudLayerController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::CloudLayerController::Key::value, 
			&SkyDef::CloudLayerController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::CloudLayerIntern::addModSeq( const SkyDef::CloudLayerController::Key& _key1, const SkyDef::CloudLayerController::Key& _key2, Real _ratio, Real _weight )
	{
		mPositionModSeq.add           ( Interpolation::lerp( _key1.positionModifier,            _key2.positionModifier,            _ratio), _weight );
		mEulerAnglesDegModSeq.add     ( Interpolation::lerp( _key1.eulerAnglesDegModifier,      _key2.eulerAnglesDegModifier,      _ratio), _weight );
		mScaleModSeq.add              ( Interpolation::lerp( _key1.scaleModifier,               _key2.scaleModifier,               _ratio), _weight );
		mAmbientLightModSeq.add       ( Interpolation::lerp( _key1.ambientLightModifier,        _key2.ambientLightModifier,        _ratio), _weight );
		mDiffuseLightModSeq.add       ( Interpolation::lerp( _key1.diffuseLightModifier,        _key2.diffuseLightModifier,        _ratio), _weight );
		mSpecularLightModSeq.add      ( Interpolation::lerp( _key1.specularLightModifier,       _key2.specularLightModifier,       _ratio), _weight );
		mAmbientLightFilterModSeq.add ( Interpolation::lerp( _key1.ambientLightFilterModifier,  _key2.ambientLightFilterModifier,  _ratio), _weight );
		mDiffuseLightFilterModSeq.add ( Interpolation::lerp( _key1.diffuseLightFilterModifier,  _key2.diffuseLightFilterModifier,  _ratio), _weight );
		mSpecularLightFilterModSeq.add( Interpolation::lerp( _key1.specularLightFilterModifier, _key2.specularLightFilterModifier, _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::CloudLayerIntern::calculateModSeqs()
	{
		position             = mPositionModSeq.getTotal();
		eulerAnglesDeg       = mEulerAnglesDegModSeq.getTotal();
		scale                = mScaleModSeq.getTotal();
		ambientLight         = mAmbientLightModSeq.getTotal();
		diffuseLight         = mDiffuseLightModSeq.getTotal();
		specularLight        = mSpecularLightModSeq.getTotal();
		mAmbientLightFilter  = mAmbientLightFilterModSeq.getTotal();
		mDiffuseLightFilter  = mDiffuseLightFilterModSeq.getTotal();
		mSpecularLightFilter = mSpecularLightFilterModSeq.getTotal();
	}
	//--------------------------------------------------------------------------------
	ColourValue SkyCalculator::CloudLayerIntern::getAmbientLightFilter() const
	{
		return mAmbientLightFilter;
	}
	//--------------------------------------------------------------------------------
	ColourValue SkyCalculator::CloudLayerIntern::getDiffuseLightFilter() const
	{
		return mDiffuseLightFilter;
	}
	//--------------------------------------------------------------------------------
	ColourValue SkyCalculator::CloudLayerIntern::getSpecularLightFilter() const
	{
		return mSpecularLightFilter;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::CloudLayerIntern::calculateDerivedTransform()
	{
		Matrix4 mat = getDerivedTransform();
		mat.decomposition( derivedPosition, derivedScale, derivedOrientation );
	}
	//--------------------------------------------------------------------------------
	Matrix4 SkyCalculator::CloudLayerIntern::getDerivedTransform() const
	{
		Matrix4 matrix[4];

		// #0: scaling
		matrix[0] = Matrix4::IDENTITY;
		matrix[0].setScale( mScaleModSeq.getTotal() );

		// #1: rotation using euler angles.
		Vector3 eaDeg = mEulerAnglesDegModSeq.getTotal();
		Radian eaRad[3];
		eaRad[0] = (Radian) (Degree) eaDeg[0];
		eaRad[1] = (Radian) (Degree) eaDeg[1];
		eaRad[2] = (Radian) (Degree) eaDeg[2];
		Matrix3 eaMat;
		eaMat.FromEulerAnglesXYZ( eaRad[0], eaRad[1], eaRad[2] );
		matrix[1] = Matrix4( eaMat );

		// #2: translation using explicit position value.
		matrix[2].makeTrans( mPositionModSeq.getTotal() );

		// #3: third translation to the current camera's position
		Camera* camera = mSkyCalculator->mCurrentCamera;
		matrix[3].makeTrans( camera->getPositionForViewUpdate() );

		// Final multiplication of all these transformations
		Matrix4 finalMat = matrix[3] * matrix[2] * matrix[1] * matrix[0];
		return finalMat;
	}






	//--------------------------------------------------------------------------------
	// Processing of SkyDef::Sound and SkyDef::SoundController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForSounds( const SkyDefPtr& _skyDef, Real _weight )
	{
		SkyDef::SoundIterator it = _skyDef->getSoundIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::Sound& sound = it.getNext();
			SoundIntern& si = getSoundIntern( sound );
			si.addModSeq( sound, _weight );
		}

		SkyDef::SoundControllerIterator it2 = _skyDef->getSoundControllerIterator();
		while( it2.hasMoreElements() )
		{
			SkyDef::SoundController& ctrl = it2.getNext();
			SoundIntern& si = getSoundIntern( ctrl.soundName );
			si.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::SoundIntern& SkyCalculator::getSoundIntern( const SkyDef::Sound& _sound )
	{
		SoundInterns::iterator it;
		for(it = mSoundInterns.begin();	it != mSoundInterns.end(); ++it)
		{
			SoundIntern& si = *it;
			if( UnicodeUtil::equalsNoCase(si.name, _sound.name ))
				break;
		}

		if( it == mSoundInterns.end() )
		{
			mSoundInterns.push_back( SoundIntern(this, _sound) );
			it = mSoundInterns.end();
			--it;
		}

		SoundIntern& si = *it;
		si.setUsedByCurrentCamera( true );
		si.setUsedInCurrentFrame ( true );
		return si;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::SoundIntern& SkyCalculator::getSoundIntern( const String& _soundName )
	{
		SoundInterns::iterator it;
		for(it = mSoundInterns.begin();	it != mSoundInterns.end(); ++it)
		{
			SoundIntern& si = *it;
			if( UnicodeUtil::equalsNoCase(si.name, _soundName ))
				break;
		}

		if( it == mSoundInterns.end() )
		{
			GOTHOGRE_EXCEPT( "Sound '" << _soundName << "' not found.");
		}

		SoundIntern& si = *it;
		si.setUsedByCurrentCamera( true );
		si.setUsedInCurrentFrame ( true );
		return si;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::SoundIntern::SoundIntern( SkyCalculator* _skyCalculator, const SkyDef::Sound& _sound )
		: Intern( _skyCalculator )
	{
		name = _sound.name;
		soundDef = _sound.soundDef;
		looping = _sound.looping;
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::SoundIntern::clearModSeqs()
	{
		mPositionModSeq.clear();
		mMaxDistanceModSeq.clear();
		mVolumePercModSeq.clear();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::SoundIntern::addModSeq( const SkyDef::Sound& _sound, Real _weight )
	{
		mPositionModSeq.add   ( Vector3Modifier( Vector3::ZERO, _sound.position    ), _weight );
		mMaxDistanceModSeq.add( RealModifier   (             0, _sound.maxDistance ), _weight );
		mVolumePercModSeq.add ( RealModifier   (             0, _sound.volumePerc  ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::SoundIntern::addModSeq( const SkyDef::SoundController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::SoundController::Key::value, 
			&SkyDef::SoundController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::SoundIntern::addModSeq( const SkyDef::SoundController::Key& _key1, const SkyDef::SoundController::Key& _key2, Real _ratio, Real _weight )
	{
		mPositionModSeq.add   ( Interpolation::lerp( _key1.positionModifier,    _key2.positionModifier,    _ratio), _weight );
		mMaxDistanceModSeq.add( Interpolation::lerp( _key1.maxDistanceModifier, _key2.maxDistanceModifier, _ratio), _weight );
		mVolumePercModSeq.add ( Interpolation::lerp( _key1.volumePercModifier,  _key2.volumePercModifier,  _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::SoundIntern::calculateModSeqs()
	{
		position    = mPositionModSeq.getTotal();
		maxDistance = mMaxDistanceModSeq.getTotal();
		volumePerc  = mVolumePercModSeq.getTotal();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::SoundIntern::calculateDerivedTransform()
	{
		Matrix4 mat4 = getDerivedTransform();
		derivedPosition = mat4.getTrans();
	}
	//--------------------------------------------------------------------------------
	Matrix4 SkyCalculator::SoundIntern::getDerivedTransform() const
	{
		Camera* camera = mSkyCalculator->mCurrentCamera;
		Matrix4 mat4 = Matrix4::IDENTITY;
		mat4.setTrans( position + camera->getPositionForViewUpdate() );
		return mat4;
	}




	//--------------------------------------------------------------------------------
	// Processing of SkyDef::MaterialController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForMaterials( const SkyDefPtr& _skyDef, Real _weight )
	{
		SkyDef::MaterialControllerIterator it = _skyDef->getMaterialControllerIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::MaterialController& ctrl = it.getNext();
			MaterialIntern& mi = getMaterialIntern( ctrl.mtlName );
			mi.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::MaterialIntern& SkyCalculator::getMaterialIntern( const String& _mtlName )
	{
		MaterialInterns::iterator it;
		for(it = mMaterialInterns.begin();	it != mMaterialInterns.end(); ++it)
		{
			MaterialIntern& mi = *it;
			if( UnicodeUtil::equalsNoCase(mi.mtlName, _mtlName) )
				break;
		}

		if( it == mMaterialInterns.end() )
		{
			mMaterialInterns.push_back( MaterialIntern(this, _mtlName) );
			it = mMaterialInterns.end();
			--it;
		}

		MaterialIntern& mi = *it;
		mi.setUsedByCurrentCamera( true );
		mi.setUsedInCurrentFrame ( true );
		return mi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::MaterialIntern::MaterialIntern( SkyCalculator* _skyCalculator, const String& _mtlName )
		: Intern( _skyCalculator )
	{
		mtlName = _mtlName;
		clearModSeqs();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::MaterialIntern::clearModSeqs()
	{
		mFrameIndexModSeq.clear();
		mScrollSpeedModSeq.clear();
		mColourFactorModSeq.clear();
		mSpecularIntensityFactorModSeq.clear();
		mSpecularColourFactorModSeq.clear();
		mReflectivityFactorModSeq.clear();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::MaterialIntern::addModSeq( const SkyDef::MaterialController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::MaterialController::Key::value, 
			&SkyDef::MaterialController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::MaterialIntern::addModSeq( const SkyDef::MaterialController::Key& _key1, const SkyDef::MaterialController::Key& _key2, Real _ratio, Real _weight )
	{
		mFrameIndexModSeq.add             ( Interpolation::lerp( _key1.frameIndexModifier,              _key2.frameIndexModifier,              _ratio), _weight );
		mScrollSpeedModSeq.add            ( Interpolation::lerp( _key1.scrollSpeedModifier,             _key2.scrollSpeedModifier,             _ratio), _weight );
		mColourFactorModSeq.add           ( Interpolation::lerp( _key1.colourFactorModifier,            _key2.colourFactorModifier,            _ratio), _weight );
		mSpecularIntensityFactorModSeq.add( Interpolation::lerp( _key1.specularIntensityFactorModifier, _key2.specularIntensityFactorModifier, _ratio), _weight );
		mSpecularColourFactorModSeq.add   ( Interpolation::lerp( _key1.specularColourFactorModifier,    _key2.specularColourFactorModifier,    _ratio), _weight );
		mReflectivityFactorModSeq.add     ( Interpolation::lerp( _key1.reflectivityFactorModifier,      _key2.reflectivityFactorModifier,      _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::MaterialIntern::calculateModSeqs()
	{
		frameIndexModifier              = mFrameIndexModSeq.getTotalModifier();
		scrollSpeedModifier             = mScrollSpeedModSeq.getTotalModifier();
		colourFactorModifier            = mColourFactorModSeq.getTotalModifier();
		specularIntensityFactorModifier = mSpecularIntensityFactorModSeq.getTotalModifier();
		specularColourFactorModifier    = mSpecularColourFactorModSeq.getTotalModifier();
		reflectivityFactorModifier      = mReflectivityFactorModSeq.getTotalModifier();
	}





	//--------------------------------------------------------------------------------
	// Processing of SkyDef::Overlay and SkyDef::OverlayController.
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForOverlays( const SkyDefPtr& _skyDef, Real _weight )
	{
		SkyDef::OverlayIterator it = _skyDef->getOverlayIterator();
		while( it.hasMoreElements() )
		{
			SkyDef::Overlay& overlay = it.getNext();
			OverlayIntern& oi = getOverlayIntern( overlay );
			oi.addModSeq( overlay, _weight );
		}

		SkyDef::OverlayControllerIterator it2 = _skyDef->getOverlayControllerIterator();
		while( it2.hasMoreElements() )
		{
			SkyDef::OverlayController& ctrl = it2.getNext();
			OverlayIntern& oi = getOverlayIntern( ctrl.overlayName );
			oi.addModSeq( ctrl, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::OverlayIntern& SkyCalculator::getOverlayIntern( const SkyDef::Overlay& _overlay )
	{
		OverlayInterns::iterator it;
		for(it = mOverlayInterns.begin();	it != mOverlayInterns.end(); ++it)
		{
			OverlayIntern& oi = *it;
			if( UnicodeUtil::equalsNoCase(oi.getOverlayName(), _overlay.name) )
				break;
		}

		if( it == mOverlayInterns.end() )
		{
			mOverlayInterns.push_back( OverlayIntern(this, _overlay) );
			it = mOverlayInterns.end();
			--it;
		}

		OverlayIntern& oi = *it;
		oi.setUsedByCurrentCamera( true );
		oi.setUsedInCurrentFrame ( true );
		return oi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::OverlayIntern& SkyCalculator::getOverlayIntern( const String& _overlayName )
	{
		OverlayInterns::iterator it;
		for(it = mOverlayInterns.begin();	it != mOverlayInterns.end(); ++it)
		{
			OverlayIntern& oi = *it;
			if( UnicodeUtil::equalsNoCase(oi.getOverlayName(), _overlayName))
				break;
		}

		if( it == mOverlayInterns.end() )
		{
			GOTHOGRE_EXCEPT( "Overlay '" << _overlayName << "' not found.");
		}

		OverlayIntern& oi = *it;
		oi.setUsedByCurrentCamera( true );
		oi.setUsedInCurrentFrame ( true );
		return oi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::OverlayIntern::OverlayIntern( SkyCalculator* _skyCalculator, const SkyDef::Overlay& _overlay )
		: Intern( _skyCalculator )
	{
		mOverlayName = _overlay.name;
		mSkyName = _overlay.skyName;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::OverlayIntern::clearModSeqs()
	{
		mWeightModSeq.clear();
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::OverlayIntern::addModSeq( const SkyDef::Overlay& _overlay, Real _weight )
	{
		mWeightModSeq.add( RealModifier( 0, _overlay.weight ), _weight );
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::OverlayIntern::addModSeq( const SkyDef::OverlayController& _ctrl, Real _weight )
	{
		Real funcValue = mSkyCalculator->getFunctionValue( _ctrl.functionName );
		
		Interpolation::FindClosestPointsResult<Real>
			fcpr = Interpolation::findClosestPoints( funcValue, _ctrl.keys,
			&SkyDef::OverlayController::Key::value, 
			&SkyDef::OverlayController::Key::period );

		if(fcpr.success)
		{
			addModSeq( _ctrl.keys[fcpr.lowerBound], _ctrl.keys[fcpr.upperBound], 
				fcpr.ratio, _weight );
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::OverlayIntern::addModSeq( const SkyDef::OverlayController::Key& _key1, const SkyDef::OverlayController::Key& _key2, Real _ratio, Real _weight )
	{
		mWeightModSeq.add( Interpolation::lerp( _key1.weightModifier, _key2.weightModifier,    _ratio), _weight );
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::OverlayIntern::getWeight() const
	{
		return mWeightModSeq.getTotal();
	}
	//--------------------------------------------------------------------------------
	const String& SkyCalculator::OverlayIntern::getSkyName() const
	{
		return mSkyName;
	}
	//--------------------------------------------------------------------------------
	const String& SkyCalculator::OverlayIntern::getOverlayName() const
	{
		return mOverlayName;
	}




	//--------------------------------------------------------------------------------
	// Overlaying skies
	//--------------------------------------------------------------------------------
	void SkyCalculator::buildModSeqsForOverlaySkies()
	{
		for( OverlayInterns::iterator it = mOverlayInterns.begin();
			it != mOverlayInterns.end(); ++it )
		{
			OverlayIntern& oi = *it;
			buildModSeqsForSky( oi.getSkyName(), oi.getWeight() );
		}
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::SkyIntern& SkyCalculator::getSkyIntern( const String& _skyName, bool& _alreadyWasUsedByCurrentCamera )
	{
		SkyInterns::iterator it;
		for(it = mSkyInterns.begin(); it != mSkyInterns.end(); ++it)
		{
			SkyIntern& ski = *it;
			if( UnicodeUtil::equalsNoCase(ski.getSkyDef()->getName(), _skyName) )
				break;
		}

		if( it == mSkyInterns.end() )
		{
			SkyDefPtr skyDef = SkyDefManagerEx::getSingleton().create( _skyName );
			mSkyInterns.push_back( SkyIntern(this, skyDef) );
			it = mSkyInterns.end();
			--it;
		}

		SkyIntern& ski = *it;
		_alreadyWasUsedByCurrentCamera = ski.isUsedByCurrentCamera();
		ski.setUsedByCurrentCamera( true );
		ski.setUsedInCurrentFrame ( true );
		return ski;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::SkyIntern& SkyCalculator::getSkyIntern( const String& _skyName )
	{
		bool b;
		return getSkyIntern( _skyName, b );
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::SkyIntern::SkyIntern( SkyCalculator* _skyCalculator, const SkyDefPtr& _skyDef )
		: Intern( _skyCalculator )
	{
		mSkyDef = _skyDef;
		mOldWeight = 0;
		mWeight = 0;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::SkyIntern::setWeight( Real _weight )
	{
		mOldWeight = mWeight;
		mWeight = _weight;
		
		if( mWeight >= WEIGHT_EPSILON && mOldWeight < WEIGHT_EPSILON )
		{
			mSkyStartWorldTime = WorldTime::getSingleton().getCurrentTime();
			mSkyStartSystemTime = SystemTime::getSingleton().getCurrentTime();
		}
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::SkyIntern::getWeight() const
	{
		return mWeight;
	}
	//--------------------------------------------------------------------------------
	SkyDefPtr SkyCalculator::SkyIntern::getSkyDef() const
	{
		return mSkyDef;
	}
	//--------------------------------------------------------------------------------
	Time SkyCalculator::SkyIntern::getSkyStartWorldTime() const
	{
		return mSkyStartWorldTime;
	}
	//--------------------------------------------------------------------------------
	Time SkyCalculator::SkyIntern::getSkyStartSystemTime() const
	{
		return mSkyStartSystemTime;
	}




	//--------------------------------------------------------------------------------
	// Light filtering
	//--------------------------------------------------------------------------------
	void SkyCalculator::calculateLightFilters()
	{
		for(AtmosphereInterns::iterator itA = mAtmosphereInterns.begin(); 
			itA != mAtmosphereInterns.end(); ++itA)
		{
			AtmosphereIntern& ai = *itA;
			applyLightFilters(ai.layer, ai.getAmbientLightFilter(), 
				ai.getDiffuseLightFilter(), ai.getSpecularLightFilter());
		}

		for(CloudLayerInterns::iterator itC = mCloudLayerInterns.begin(); 
			itC != mCloudLayerInterns.end(); ++itC)
		{
			CloudLayerIntern& cli = *itC;
			applyLightFilters(cli.layer, cli.getAmbientLightFilter(), 
				cli.getDiffuseLightFilter(), cli.getSpecularLightFilter());
		}
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::applyLightFilters( int _layer, 
		const ColourValue& _ambientLightFilter,
		const ColourValue& _diffuseLightFilter,
		const ColourValue& _specularLightFilter )
	{
		for(AtmosphereInterns::iterator itA = mAtmosphereInterns.begin(); 
			itA != mAtmosphereInterns.end(); ++itA)
		{
			AtmosphereIntern& ai = *itA;
			if(ai.layer < _layer)
				ai.ambientLight = ai.ambientLight * _ambientLightFilter;
		}

		for(CloudLayerInterns::iterator itC = mCloudLayerInterns.begin(); 
			itC != mCloudLayerInterns.end(); ++itC)
		{
			CloudLayerIntern& cli = *itC;
			if(cli.layer < _layer)
			{
				cli.ambientLight = cli.ambientLight * _ambientLightFilter;
				cli.diffuseLight = cli.diffuseLight * _diffuseLightFilter;
				cli.specularLight = cli.specularLight * _specularLightFilter;
			}
		}

		for(BodyInterns::iterator itB = mBodyInterns.begin(); 
			itB != mBodyInterns.end(); ++itB)
		{
			BodyIntern& bi = *itB;
			if(bi.layer < _layer)
			{
				bi.ambientLight = bi.ambientLight * _ambientLightFilter;
				bi.diffuseLight = bi.diffuseLight * _diffuseLightFilter;
				bi.specularLight = bi.specularLight * _specularLightFilter;
			}
		}
	}




	//--------------------------------------------------------------------------------
	// Calculations of the depths
	//--------------------------------------------------------------------------------
	void SkyCalculator::calculateUnderwaterDepth()
	{
		SceneUnderwaterInfo underwaterInfo = mSceneManager->getUnderwaterInfo( mCurrentCamera );
		mUnderwaterDepth = underwaterInfo.immersionDepth;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::getUnderwaterDepth() const
	{
		return mUnderwaterDepth;
	}
	//--------------------------------------------------------------------------------
	void SkyCalculator::calculateIndoorDepth()
	{
		SceneSectorInfo sectorInfo = mSceneManager->getSectorInfo( mCurrentCamera );
		mIndoorDepth = 0; //calcIndoorDepthRecursively( sectorInfo.position, sectorInfo.sectorName );
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::calcIndoorDepthRecursively( const Vector3& _position, const String& _sectorName )
	{
		PortalSystem::PortalIterator it = 
			PortalSystem::getSingleton().getAdjacentPortalIterator( 
				_sectorName, mSceneManager);

		Real totaldepth = Math::POS_INFINITY;
		while( it.hasMoreElements())
		{
			Portal* portal = it.getNext();
			Real r = portal->_getDerivedCenter().distance( _position );
			Real area = portal->getArea();
			if( area == 0 )
				continue;
			Real depth = r / Math::Sqrt( area );
			if( !portal->isToOutdoor())
			{
				Real depth2 = calcIndoorDepthRecursively( portal->_getDerivedCenter(), portal->getSectorTo() );
				depth += depth2;
			}
			totaldepth = 1 / Math::Sqrt( 1 / Math::Sqr( totaldepth ) + 1 / Math::Sqr( depth ));
		}
		return totaldepth;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::getIndoorDepth() const
	{
		return mIndoorDepth;
	}






	//--------------------------------------------------------------------------------
	// Calculation of skydef's functions
	//--------------------------------------------------------------------------------
	Real SkyCalculator::getFunctionValue( const String& _functionName )
	{
		FunctionIntern& fi = getFunctionIntern( _functionName );
		return fi.getFunctionValue();
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::FunctionIntern& SkyCalculator::getFunctionIntern( const String& _functionName )
	{
		FunctionInterns::iterator it;
		for( it = mFunctionInterns.begin();	it != mFunctionInterns.end(); ++it)
		{
			FunctionIntern& fi = *it;
			if( fi.getFunctionName() == _functionName )
				break;
		}

		if( it == mFunctionInterns.end() )
		{
			for( SkyInterns::iterator it2 = mSkyInterns.begin(); 
				it2 != mSkyInterns.end(); ++it2 )
			{
				SkyIntern& ski = *it2;
				size_t index = ski.getSkyDef()->findFunctionIndex( _functionName );
				if( index != -1 )
				{
					const SkyDef::Function& mFunction = ski.getSkyDef()->getFunction( index );
					mFunctionInterns.push_back( FunctionIntern( this, mFunction ));
					FunctionIntern& fi = mFunctionInterns.back();
					it = mFunctionInterns.end();
					--it;
					break;
				}
			}
		}

		if( it == mFunctionInterns.end() )
			GOTHOGRE_EXCEPT("Function " << _functionName << " not found.");

		FunctionIntern& fi = *it;
		fi.setUsedByCurrentCamera( true );
		fi.setUsedInCurrentFrame(  true );
		return fi;
	}
	//--------------------------------------------------------------------------------
	SkyCalculator::FunctionIntern::FunctionIntern( SkyCalculator* _skyCalculator, const SkyDef::Function& _function ) 
		: Intern( _skyCalculator )
	{
		mFunction = _function;
		mRestStep = 0;
		mCurArgValue = Math::NEG_INFINITY; // -Infinity here means a requirement of delayed initialisation
	}
	//--------------------------------------------------------------------------------
	SkyDef::Function::Argument SkyCalculator::FunctionIntern::getArgument() const
	{
		return mFunction.argument;
	}
	//--------------------------------------------------------------------------------
	const String& SkyCalculator::FunctionIntern::getFunctionName() const
	{
		return mFunction.name;
	}
	//--------------------------------------------------------------------------------
	const String& SkyCalculator::FunctionIntern::getBodyName() const
	{
		return mFunction.bodyName;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getFunctionValue()
	{
		return getFunctionValue( getArgumentValue() );
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getArgumentValue() const
	{
		Real argValue;
		switch( getArgument() )
		{
			case SkyDef::Function::WORLD_TIME:
			{
				argValue = (Real) WorldTime::getSingleton().getCurrentTime();
				break;
			}
			case SkyDef::Function::SYSTEM_TIME:
			{
				argValue = (Real) SystemTime::getSingleton().getCurrentTime();
				break;
			}
			case SkyDef::Function::WORLD_TIME_REL:
			{
				argValue = (Real) WorldTime::getSingleton().getCurrentTime();
				argValue -= (Real) mSkyCalculator->getSkyIntern( mFunction.skyName ).getSkyStartWorldTime();
				break;
			}
			case SkyDef::Function::SYSTEM_TIME_REL:
			{
				argValue = (Real) WorldTime::getSingleton().getCurrentTime();
				argValue -= (Real) mSkyCalculator->getSkyIntern( mFunction.skyName ).getSkyStartSystemTime();
				break;
			}
			case SkyDef::Function::UNDERWATER_DEPTH:
			{
				argValue = mSkyCalculator->getUnderwaterDepth();
				break;
			}
			case SkyDef::Function::INDOOR_DEPTH:
			{
				argValue = mSkyCalculator->getIndoorDepth();
				break;
			}
			case SkyDef::Function::ALTITUDE_OF_BODY:
			{
				argValue = mSkyCalculator->getBodyIntern( mFunction.bodyName ).getAltitude().valueDegrees();
				break;
			}
			default:
				GOTHOGRE_EXCEPT("Unknown function's argument: " << mFunction.argument );
		}
		return argValue;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getFunctionValue( Real _argValue )
	{
		Real result;
		switch( mFunction.type )
		{
			case SkyDef::Function::CONSTANT:
				result = getConstantFuncValue( _argValue );
				break;
			case SkyDef::Function::LINEAR:
				result = getLinearFuncValue( _argValue );
				break;
			case SkyDef::Function::QUADRATIC:
				result = getQuadraticFuncValue( _argValue );
				break;
			case SkyDef::Function::SIN:
				result = getSinFuncValue( _argValue );
				break;
			case SkyDef::Function::RANDOM_VALUE:
				result = getRandomFuncValue( _argValue );
				break;
			case SkyDef::Function::RANDOM_RATE:
				result = getRandomRateFuncValue( _argValue );
				break;
			case SkyDef::Function::RANDOM_RATE2:
				result = getRandomRate2FuncValue( _argValue );
				break;
			case SkyDef::Function::RANDOM_MANUAL:
				result = getRandomManualFuncValue( _argValue );
				break;
			default:
				GOTHOGRE_EXCEPT( "Unknown function's type: " << mFunction.type );
		}
		return result;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getConstantFuncValue( Real _argValue )
	{
		if(mCurArgValue != _argValue)
		{
			mCurArgValue = _argValue;
			mCurFuncValue = mFunction.value;
		}
		return mCurFuncValue;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getLinearFuncValue( Real _argValue )
	{
		if(mCurArgValue != _argValue)
		{
			mCurArgValue = _argValue;
			
			Real curArgOffset = mCurArgValue;
			if( mFunction.period > 0 && mFunction.period != Math::POS_INFINITY )
			{
				curArgOffset -= floor( _argValue / mFunction.period ) 
					* mFunction.period;
			}

			mCurFuncValue = mFunction.initValue + mFunction.rate * curArgOffset;
		}
		return mCurFuncValue;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getQuadraticFuncValue( Real _argValue )
	{
		if( mCurArgValue != _argValue )
		{
			mCurArgValue = _argValue;
			
			Real curArgOffset = mCurArgValue;
			if( mFunction.period > 0 && mFunction.period != Math::POS_INFINITY )
			{
				curArgOffset -= floor( curArgOffset / mFunction.period ) 
					* mFunction.period;
			}
			
			mCurFuncValue = mFunction.initValue + mFunction.initRate * curArgOffset
				+ mFunction.rate2 * curArgOffset * curArgOffset / 2;
		}
		return mCurFuncValue;
	}
	//--------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getSinFuncValue( Real _argValue )
	{
		if( mCurArgValue != _argValue )
		{
			mCurArgValue = _argValue;
			
			mCurFuncValue = 
				Math::Sin( Math::TWO_PI * mCurArgValue / mFunction.period 
					+ mFunction.initPhase ) 
				* (mFunction.maxValue - mFunction.minValue) / 2 
				+ (mFunction.minValue + mFunction.maxValue) / 2;
		}
		return mCurFuncValue;
	}
	//---------------------------------------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getRandomFuncValue( Real _argValue )
	{
		while( mCurArgValue != _argValue )
		{
			// Usually mCurArgValue <= _argValue, i.e.
			// the mFunction calculates its values for the increasing argument,
			// however sometimes the argument can be restarted 
			// to the beginning of its range.
			if( mCurArgValue > _argValue )
			{
				mCurArgValue = Math::NEG_INFINITY;
				mRestStep = 0;
			}

			// "mRestStep is zero" means a special point 
			// i.e. a time to change the mFunction's behaviour.
			if( mRestStep == 0 ) 
			{
				if( mCurArgValue == Math::NEG_INFINITY )
				{
					// Initialisation on the first calculation:
					mCurArgValue = _argValue;
				}
				// Calculate a random value.
				mCurFuncValue = Math::RangeRandom( mFunction.minValue, mFunction.maxValue );

				// The value will be unchanged over this step.
				mRestStep = mFunction.step;
			}

			// Calculate an increase of the mFunction's argument.
			Real delta = _argValue - mCurArgValue;
			if( delta > mRestStep )
				delta = mRestStep;

			// Calculate a new value of the mFunction's argument.
			mCurArgValue += delta;
			mRestStep -= delta;
		}
		return mCurFuncValue;
	}
	//---------------------------------------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getRandomRateFuncValue( Real _argValue )
	{
		while( mCurArgValue != _argValue )
		{
			// Usually mCurArgValue <= _argValue, i.e.
			// the mFunction calculates its values for the increasing argument,
			// however sometimes the argument can be restarted 
			// to the beginning of its range.
			if( mCurArgValue > _argValue )
			{
				mCurArgValue = Math::NEG_INFINITY;
				mRestStep = 0;
			}

			// "mRestStep is zero" means a special point 
			// i.e. a time to change the mFunction's behaviour.
			if( mRestStep == 0 )
			{
				if( mCurArgValue == Math::NEG_INFINITY )
				{
					// Initialisation on the first calculation:
					mCurArgValue = _argValue;
					mCurFuncValue = mFunction.initValue;
				}
				// Calculate a random rate of the mFunction.
				mCurFuncRate = Math::RangeRandom( mFunction.minRate, mFunction.maxRate );

				// The rate will be unchanged over this step.
				mRestStep = mFunction.step;
			}

			// Calculate an increase of the mFunction's argument.
			Real delta = _argValue - mCurArgValue;
			if( delta > mRestStep )
				delta = mRestStep;

			// Calculate a new value of the mFunction.
			mCurArgValue  += delta;
			mCurFuncValue += delta * mCurFuncRate;
			mRestStep -= delta;

			// Bound the value of the mFunction.
			mCurFuncValue = Math::Clamp( mCurFuncValue, mFunction.minValue, mFunction.maxValue );
		}
		return mCurFuncValue;
	}
	//---------------------------------------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getRandomRate2FuncValue( Real _argValue )
	{
		while( mCurArgValue != _argValue )
		{
			// Usually mCurArgValue <= _argValue, i.e.
			// the mFunction calculates its values for the increasing argument,
			// however sometimes the argument can be restarted 
			// to the beginning of its range.
			if( mCurArgValue > _argValue )
			{
				mCurArgValue = Math::NEG_INFINITY;
				mRestStep = 0;
			}

			// "mRestStep is zero" means a special point 
			// i.e. a time to change the mFunction's behaviour.
			if( mRestStep == 0 )
			{
				if( mCurArgValue == Math::NEG_INFINITY )
				{
					// Initialisation on the first calculation:
					mCurArgValue = _argValue;
					mCurFuncRate = mFunction.initRate;
					mCurFuncValue = mFunction.initValue;
				}
				// Calculate a random rate of the rate of the mFunction.
				mCurFuncRate2 = Math::RangeRandom( mFunction.minRate2, mFunction.maxRate2 );

				// The rate of the rate will be unchanged over this step.
				mRestStep = mFunction.step;
			}

			// Calculate an increase of the mFunction's argument.
			Real delta = _argValue - mCurArgValue;
			if( delta > mRestStep )
				delta = mRestStep;

			// Calculate a new value of the mFunction.
			mCurArgValue  += delta;
			mCurFuncRate  += delta * mCurFuncRate2;
			mCurFuncValue += delta * mCurFuncRate;
			mRestStep -= delta;

			// Bound the value of the mFunction.
			if( mCurFuncValue > mFunction.maxValue )
			{
				mCurFuncValue = mFunction.maxValue;
				mCurFuncRate *= mFunction.bounce;
			}
			else if( mCurFuncValue < mFunction.minValue )
			{
				mCurFuncValue = mFunction.minValue;
				mCurFuncRate *= mFunction.bounce;
			}
		}
		return mCurFuncValue;
	}
	//---------------------------------------------------------------------------------------------------------------
	Real SkyCalculator::FunctionIntern::getRandomManualFuncValue( Real _argValue )
	{
		while( mCurArgValue != _argValue )
		{
			// Usually mCurArgValue <= _argValue, i.e.
			// the mFunction calculates its values for the increasing argument,
			// however sometimes the argument can be restarted 
			// to the beginning of its range.
			if( mCurArgValue > _argValue )
			{
				mCurArgValue = Math::NEG_INFINITY;
				mRestStep = 0;
			}

			// "mRestStep is zero" means a special point 
			// i.e. a time to change the mFunction's behaviour.
			if( mRestStep == 0 )
			{
				if( mCurArgValue == Math::NEG_INFINITY )
				{
					// Initialisation on the first calculation:
					mCurArgValue = _argValue;
					mCurFuncValue = mFunction.initValue;
					
					// Search the nearest mFunction's state to the specified initial value.
					size_t nearestValueStateIndex = -1;
					Real distToNearestValue = Math::POS_INFINITY;
					for(size_t i = 0; i != mFunction.states.size(); ++i)
					{
						Real d = Math::Abs( mFunction.states[i].value - mFunction.initValue );
						if( distToNearestValue > d )
						{
							distToNearestValue = d;
							nearestValueStateIndex = i;
						}
					}
					if(nearestValueStateIndex == -1)
						GOTHOGRE_EXCEPT("List of states of mFunction " << mFunction.name << " is empty.");

					// Set the next state the mFunction will go to it over some time.
					mNextStateIndex = nearestValueStateIndex;
				}
				else
				{
					// The previous "next state" has been reached, so we need to calculate a new "next state".
					size_t curStateIndex = mNextStateIndex;
					mCurFuncValue = mFunction.states[curStateIndex].value;

					// Search two nearest states to the current state:
					// a state with a lesser value and a state with a greater value.
					size_t greaterValueStateIndex = -1; 
					Real distToGreaterValue = Math::POS_INFINITY;
					size_t lesserValueStateIndex = -1;
					Real distToLesserValue = Math::POS_INFINITY;
					for( size_t i = 0; i != mFunction.states.size(); ++i )
					{
						Real distG = mFunction.states[i].value - mCurFuncValue;
						Real distL = -distG;
						if( distToGreaterValue > distG && distG > 0 )
						{
							distToGreaterValue = distG;
							greaterValueStateIndex = i;
						}
						if( distToLesserValue > distL && distL > 0 )
						{
							distToLesserValue = distL;
							lesserValueStateIndex = i;
						}
					}

					// Use chances and the random numbers' generator
					// to calculate the behaviour of the mFunction for the some next interval.
					Real stayChance = mFunction.states[curStateIndex].stayChance;
					Real upChance = mFunction.states[curStateIndex].upChance;
					if(greaterValueStateIndex == -1)
						upChance = 0;
					Real downChance = mFunction.states[curStateIndex].downChance;
					if(lesserValueStateIndex == -1)
						downChance = 0;

					Real r = Math::RangeRandom(-downChance, upChance + stayChance);
					
					// Set the next state the mFunction will go to it over some time.
					if(r < 0)
						mNextStateIndex = lesserValueStateIndex;
					else if(r < upChance)
						mNextStateIndex = greaterValueStateIndex;
				}

				// Calculate "mCurFuncRate" and "mRestStep" based on the calculated "mNextStateIndex".
				Real d = mFunction.states[mNextStateIndex].value - mCurFuncValue;
				if(d == 0)
				{
					mCurFuncRate = 0;
					mRestStep = mFunction.states[mNextStateIndex].step;
				}
				else
				{
					if( d > 0 )
						mCurFuncRate = mFunction.maxRate;
					else
						mCurFuncRate = mFunction.minRate;
					mRestStep = d / mCurFuncRate;
				}
			}

			// Calculate an increase of the mFunction's argument.
			Real delta = _argValue - mCurArgValue;
			if( delta > mRestStep )
				delta = mRestStep;

			// Calculate a new value of the mFunction.
			mCurArgValue  += delta;
			mCurFuncValue += delta * mCurFuncRate;
			mRestStep -= delta;
		}
		return mCurFuncValue;
	}

}
