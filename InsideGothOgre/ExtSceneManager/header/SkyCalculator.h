#ifndef SKY_CALCULATOR_H
#define SKY_CALCULATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;


	/** The calculator of the parameters of the skies 
	which must be currently displayed. */
	class SkyCalculator
	{
	//----------------------------------------------------------------------
	// Public interface
	public:
		// Calculated version of SkyDef::Background
		struct Background
		{
			ColourValue colour;
		};

		// Calculated version of SkyDef::Fog
		struct Fog
		{
			ColourValue colour;
			Real        nearDistance;
			Real        farDistance;
		};
		
		// Calculated version of SkyDef::FarClippingPlane
		struct FarClippingPlane
		{
			Real        distance;
		};

		// Calculated version of SkyDef::Atmosphere
		struct Atmosphere
		{
			String                     name;
			int                        layer;
			ColourValue                ambientLight;
			Vector3                    derivedPosition;
			Quaternion                 derivedOrientation;
			Vector3                    derivedScale;
			vector<Vector3>::type      points;
			vector<ColourValue>::type  colours;
			vector<uint32>::type       indices;
		};

		// Calculated version of SkyDef::Body
		struct Body
		{
			String       name;
			MeshPtr      mesh;
			String       mtlName;
			int          layer;
			Degree       altitude;
			Degree       azimuth;
			Real         distance;
			Vector3      position;
			Vector3      eulerAnglesDeg;
			Vector3      scale;
			Vector3      derivedPosition;
			Quaternion   derivedOrientation;
			Vector3      derivedScale;
			ColourValue  ambientLight;
			ColourValue  diffuseLight;
			ColourValue  specularLight;
		};

		// Calculated version of SkyDef::CloudLayer
		struct CloudLayer
		{
			String       name;
			MeshPtr      mesh;
			String       mtlName;
			int          layer;
			Vector3      position;
			Vector3      eulerAnglesDeg;
			Vector3      scale;
			Vector3      derivedPosition;
			Quaternion   derivedOrientation;
			Vector3      derivedScale;
			ColourValue  ambientLight;
			ColourValue  diffuseLight;
			ColourValue  specularLight;
		};

		typedef SkyDef::SizeTModifier        SizeTModifier;
		typedef SkyDef::RealModifier         RealModifier;
		typedef SkyDef::DegreeModifier       DegreeModifier;
		typedef SkyDef::ColourValueModifier  ColourValueModifier;
		typedef SkyDef::Vector2Modifier      Vector2Modifier;
		typedef SkyDef::Vector3Modifier      Vector3Modifier;

		// Calculated version of material
		struct Material
		{
			String               mtlName;
			SizeTModifier        frameIndexModifier;
			Vector2Modifier      scrollSpeedModifier;
			ColourValueModifier  colourFactorModifier;
			RealModifier         specularIntensityFactorModifier;
			ColourValueModifier  specularColourFactorModifier;
			RealModifier         reflectivityFactorModifier;
		};

		// Calculated version of SkyDef::Sound
		struct Sound
		{
			String       name;
			SoundDefPtr  soundDef;
			bool         looping;
			Vector3      position;
			Vector3      derivedPosition;
			Real         maxDistance;
			Real         volumePerc;
		};

		class AtmosphereIterator;
		class BodyIterator;
		class CloudLayerIterator;
		class MaterialIterator;
		class SoundIterator;

	public:
		/** Constructor. */
		SkyCalculator( SceneManagerExImpl* _sceneManager );

		/** Destructor. */
		~SkyCalculator();

		/** Calculates the state of the skies for the current time. */
		void calculate( Camera* _camera );

		/** Returns a reference to the calculated fog. */
		const Fog& getFog() const;

		/** Returns a reference to the calculated fog. */
		const FarClippingPlane& getFarClippingPlane() const;

		/** Returns a reference to the calculated background. */
		const Background& getBackground() const;

		/** Returns an iterator to the list of all the calculated atmospheres. */
		AtmosphereIterator getAtmosphereIterator() const;

		/** Returns an iterator to the list of all the calculated bodies. */
		BodyIterator getBodyIterator() const;

		/** Returns an iterator to the list of all the calculated cloud layers. */
		CloudLayerIterator getCloudLayerIterator() const;

		/** Returns an iterator to the list of all the calculated materials. */
		MaterialIterator getMaterialIterator() const;

		/** Returns an iterator to the list of all the calculated sounds. */
		SoundIterator getSoundIterator() const;


	//----------------------------------------------------------------------
	// Private implementation
	private:
		// Sequence of modifiers (see the SkyDef::Modifier template class).
		template<typename T>
		class ModifierSequence
		{
		public:
			ModifierSequence();
			~ModifierSequence();

			void clear();
			void add(const SkyDef::Modifier<T>& _modifier, Real _weight);
			
			T getTotal() const;
			SkyDef::Modifier<T> getTotalModifier() const;

		private:
			T getTotalStartingFrom( T _startValue ) const;

		private:
			struct ModifierWeight
			{
				SkyDef::Modifier<T>  modifier;
				Real                 weight;
			};

			typedef typename vector<ModifierWeight>::type ModifierWeights;
			ModifierWeights mModifierWeights;
		};

		// Typedefs for convenience.
		typedef ModifierSequence<Real>        RealModifierSequence;
		typedef ModifierSequence<Degree>      DegreeModifierSequence;
		typedef ModifierSequence<size_t>      SizeTModifierSequence;
		typedef ModifierSequence<ColourValue> ColourValueModifierSequence;
		typedef ModifierSequence<Vector2>     Vector2ModifierSequence;
		typedef ModifierSequence<Vector3>     Vector3ModifierSequence;

		// Base class for internal calculation data.
		class Intern
		{
		protected:
			bool            mUsedByCurrentCamera;
			bool            mUsedInCurrentFrame;
			SkyCalculator*  mSkyCalculator;

		public:
			Intern( SkyCalculator* _skyCalculator );

			void setUsedInCurrentFrame(bool _used);
			bool isUsedInCurrentFrame() const;

			void setUsedByCurrentCamera(bool _used);
			bool isUsedByCurrentCamera() const;
		};

		// Internal calculation data for SkyDef::Background
		class BackgroundIntern : public Background, public Intern
		{
		public:
			BackgroundIntern( SkyCalculator* _skyCalculator );

			void clearModSeqs();
			void addModSeq( const SkyDef::Background& _background, Real _weight );
			void addModSeq( const SkyDef::BackgroundController& _ctrl, Real _weight );
			void calculateModSeqs();

		private:
			void addModSeq( const SkyDef::BackgroundController::Key& _key1, const SkyDef::BackgroundController::Key& _key2, Real _ratio, Real _weight );

		private:
			ColourValueModifierSequence  mColourModSeq;
		};

		BackgroundIntern& getBackgroundIntern();

		// Internal calculation data for SkyDef::Fog
		class FogIntern : public Fog, public Intern
		{
		public:
			FogIntern( SkyCalculator* _skyCalculator );

			void clearModSeqs();
			void addModSeq( const SkyDef::Fog& _fog, Real _weight );
			void addModSeq( const SkyDef::FogController& _ctrl, Real _weight );
			void calculateModSeqs();

		private:
			void addModSeq( const SkyDef::FogController::Key& _key1, const SkyDef::FogController::Key& _key2, Real _ratio, Real _weight );

		private:
			ColourValueModifierSequence  mColourModSeq;
			RealModifierSequence         mNearDistanceModSeq;
			RealModifierSequence         mFarDistanceModSeq;
		};

		FogIntern& getFogIntern();

		// Internal calculation data for SkyDef::FarClippingPlane
		class FarClippingPlaneIntern : public FarClippingPlane, public Intern
		{
		public:
			FarClippingPlaneIntern( SkyCalculator* _skyCalculator );

			void clearModSeqs();
			void addModSeq( const SkyDef::FarClippingPlane& _farClippingPlane, Real _weight );
			void addModSeq( const SkyDef::FarClippingPlaneController& _ctrl, Real _weight );
			void calculateModSeqs();

		private:
			void addModSeq( const SkyDef::FarClippingPlaneController::Key& _key1, const SkyDef::FarClippingPlaneController::Key& _key2, Real _ratio, Real _weight );

		private:
			RealModifierSequence         mDistanceModSeq;
		};

		FarClippingPlaneIntern& getFarClippingPlaneIntern();

		// Internal calculation data for SkyDef::Atmosphere
		class AtmosphereIntern : public Atmosphere, public Intern
		{
		public:
			AtmosphereIntern( SkyCalculator* _skyCalculator, const SkyDef::Atmosphere& _atmosphere );

			void clearModSeqs();
			void addModSeq( const SkyDef::Atmosphere& _atmosphere, Real _weight );
			void addModSeq( const SkyDef::AtmosphereController& _ctrl, Real _weight );
			void calculateModSeqs();

			void calculateDerivedTransform();
			Matrix4 getDerivedTransform() const;

			ColourValue getAmbientLightFilter() const;
			ColourValue getDiffuseLightFilter() const;
			ColourValue getSpecularLightFilter() const;

		private:
			void generateGeometry( Real _radius, Degree _minAltitude );
			void addModSeq( const SkyDef::AtmosphereController::Key& _key1, const SkyDef::AtmosphereController::Key& _key2, Real _ratio, Real _weight );
			void addModSeq( const SkyDef::AtmosphereController::Key& _key1, const SkyDef::AtmosphereController::Key& _key2, Real _ratio, Real _weight, const Vector3& _directionToBody );
			void addModSeq_Base( const SkyDef::AtmosphereController::Key& _key1, const SkyDef::AtmosphereController::Key& _key2, Real _ratio, Real _weight );
			SkyDef::ColourValueModifier getColourValueModifierFromKey( const SkyDef::AtmosphereController::Key& _key, Degree angle );

		private:
			ColourValue  mAmbientLightFilter;
			ColourValue  mDiffuseLightFilter;
			ColourValue  mSpecularLightFilter;
			
			ColourValueModifierSequence  mAmbientLightModSeq;
			ColourValueModifierSequence  mAmbientLightFilterModSeq;
			ColourValueModifierSequence  mDiffuseLightFilterModSeq;
			ColourValueModifierSequence  mSpecularLightFilterModSeq;
			vector< ColourValueModifierSequence >::type mColourModSeqs;
		};

		AtmosphereIntern& getAtmosphereIntern( const SkyDef::Atmosphere& _atmosphere );
		AtmosphereIntern& getAtmosphereIntern( const String& _atmosphereName );

		// Internal calculation data for SkyDef::Body
		struct BodyIntern : public Body, public Intern
		{
		public:
			BodyIntern( SkyCalculator* _skyCalculator, const SkyDef::Body& _body );

			void clearModSeqs();
			void addModSeq( const SkyDef::Body& _body, Real _weight );
			void addModSeq( const SkyDef::BodyController& _ctrl, Real _weight );
			void calculateModSeqs();

			void calculateDerivedTransform();
			Matrix4 getDerivedTransform() const;

			Degree getAltitude() const;

		private:
			void addModSeq( const SkyDef::BodyController::Key& _key1, const SkyDef::BodyController::Key& _key2, Real _ratio, Real _weight );

		private:
			DegreeModifierSequence       mAltitudeModSeq;
			DegreeModifierSequence       mAzimuthModSeq;
			RealModifierSequence         mDistanceModSeq;
			Vector3ModifierSequence      mPositionModSeq;
			Vector3ModifierSequence      mEulerAnglesDegModSeq;
			Vector3ModifierSequence      mScaleModSeq;
			ColourValueModifierSequence  mAmbientLightModSeq;
			ColourValueModifierSequence  mDiffuseLightModSeq;
			ColourValueModifierSequence  mSpecularLightModSeq;
		};

		BodyIntern& getBodyIntern( const SkyDef::Body& _body );
		BodyIntern& getBodyIntern( const String& _bodyName );

		// Internal calculation data for SkyDef::CloudLayer
		struct CloudLayerIntern : public CloudLayer, public Intern
		{
		public:
			CloudLayerIntern( SkyCalculator* _skyCalculator, const SkyDef::CloudLayer& _cloudLayer );

			void clearModSeqs();
			void addModSeq( const SkyDef::CloudLayer& _cloudLayer, Real _weight );
			void addModSeq( const SkyDef::CloudLayerController& _ctrl, Real _weight );
			void calculateModSeqs();

			void calculateDerivedTransform();
			Matrix4 getDerivedTransform() const;

			ColourValue getAmbientLightFilter() const;
			ColourValue getDiffuseLightFilter() const;
			ColourValue getSpecularLightFilter() const;

		private:
			void addModSeq( const SkyDef::CloudLayerController::Key& _key1, const SkyDef::CloudLayerController::Key& _key2, Real _ratio, Real _weight );

		private:
			ColourValue  mAmbientLightFilter;
			ColourValue  mDiffuseLightFilter;
			ColourValue  mSpecularLightFilter;

			Vector3ModifierSequence      mPositionModSeq;
			Vector3ModifierSequence      mEulerAnglesDegModSeq;
			Vector3ModifierSequence      mScaleModSeq;
			ColourValueModifierSequence  mAmbientLightModSeq;
			ColourValueModifierSequence  mDiffuseLightModSeq;
			ColourValueModifierSequence  mSpecularLightModSeq;
			ColourValueModifierSequence  mAmbientLightFilterModSeq;
			ColourValueModifierSequence  mDiffuseLightFilterModSeq;
			ColourValueModifierSequence  mSpecularLightFilterModSeq;
		};

		CloudLayerIntern& getCloudLayerIntern( const SkyDef::CloudLayer& _cloudLayer );
		CloudLayerIntern& getCloudLayerIntern( const String& _cloudLayerName );

		// Internal calculation data for SkyDef::Sound
		class SoundIntern : public Sound, public Intern
		{
		public:
			SoundIntern( SkyCalculator* _skyCalculator, const SkyDef::Sound& _sound );

			void clearModSeqs();
			void addModSeq( const SkyDef::Sound& _sound, Real _weight );
			void addModSeq( const SkyDef::SoundController& _ctrl, Real _weight );
			void calculateModSeqs();

			void calculateDerivedTransform();
			Matrix4 getDerivedTransform() const;

		private:
			void addModSeq( const SkyDef::SoundController::Key& _key1, const SkyDef::SoundController::Key& _key2, Real _ratio, Real _weight );

		private:
			Vector3ModifierSequence      mPositionModSeq;
			RealModifierSequence         mMaxDistanceModSeq;
			RealModifierSequence         mVolumePercModSeq;
		};

		SoundIntern& getSoundIntern( const SkyDef::Sound& _sound );
		SoundIntern& getSoundIntern( const String& _soundName );

		// Internal calculation data for SkyDef::Material
		class MaterialIntern : public Material, public Intern
		{
		public:
			MaterialIntern( SkyCalculator* _skyCalculator, const String& _materialName );

			void clearModSeqs();
			void addModSeq( const SkyDef::MaterialController& _ctrl, Real _weight );
			void calculateModSeqs();

		private:
			void addModSeq( const SkyDef::MaterialController::Key& _key1, const SkyDef::MaterialController::Key& _key2, Real _ratio, Real _weight );

		private:
			SizeTModifierSequence         mFrameIndexModSeq;
			Vector2ModifierSequence       mScrollSpeedModSeq;
			ColourValueModifierSequence   mColourFactorModSeq;
			RealModifierSequence          mSpecularIntensityFactorModSeq;
			ColourValueModifierSequence   mSpecularColourFactorModSeq;
			RealModifierSequence          mReflectivityFactorModSeq;
		};

		MaterialIntern& getMaterialIntern( const String& _mtlName );

		// Internal calculation data for SkyDef::Overlay
		class OverlayIntern : public Intern
		{
		public:
			OverlayIntern( SkyCalculator* _skyCalculator, const SkyDef::Overlay& _overlay );

			void clearModSeqs();
			void addModSeq( const SkyDef::Overlay& _overlay, Real _weight );
			void addModSeq( const SkyDef::OverlayController& _ctrl, Real _weight );

			const String& getOverlayName() const;
			const String& getSkyName() const;
			Real getWeight() const;

		private:
			void addModSeq( const SkyDef::OverlayController::Key& _key1, const SkyDef::OverlayController::Key& _key2, Real _ratio, Real _weight );

		private:
			String                mOverlayName;
			String                mSkyName;
			RealModifierSequence  mWeightModSeq;
		};

		OverlayIntern& getOverlayIntern( const SkyDef::Overlay& _overlay );
		OverlayIntern& getOverlayIntern( const String& _overlayName );

		// Internal calculation data for SkyDef::Sky
		class SkyIntern : public Intern
		{
		public:
			SkyIntern( SkyCalculator* _skyCalculator, const SkyDefPtr& _skyDef);

			void setWeight(Real _weight);
			Real getWeight() const;
			
			SkyDefPtr getSkyDef() const;

			Time getSkyStartSystemTime() const;
			Time getSkyStartWorldTime() const;

		private:
			SkyDefPtr  mSkyDef;
			Real       mWeight;
			Real       mOldWeight;
			Time       mSkyStartSystemTime;
			Time       mSkyStartWorldTime;
		};

		SkyIntern& getSkyIntern( const String& _skyName );
		SkyIntern& getSkyIntern( const String& _skyName, bool& _alreadyWasUsedByCurrentCamera );

		// Internal calculation data for SkyDef::Function
		class FunctionIntern : public Intern
		{
		public:
			FunctionIntern( SkyCalculator* _skyCalculator, const SkyDef::Function& _function );
			
			const String& getFunctionName() const;
			const String& getBodyName() const;
			SkyDef::Function::Argument getArgument() const;

			Real getArgumentValue() const;
			Real getFunctionValue( Real _argValue );
			Real getFunctionValue();

		private:
			Real getConstantFuncValue( Real _argValue );
			Real getLinearFuncValue( Real _argValue );
			Real getQuadraticFuncValue( Real _argValue );
			Real getSinFuncValue( Real _argValue );
			Real getRandomFuncValue( Real _argValue );
			Real getRandomRateFuncValue( Real _argValue );
			Real getRandomRate2FuncValue( Real _argValue );
			Real getRandomManualFuncValue( Real _argValue );

		private:
			SkyDef::Function  mFunction;
			Real              mCurArgValue;
			Real              mCurFuncValue;
			Real              mCurFuncRate;
			Real              mCurFuncRate2;
			Real              mRestStep;
			size_t            mNextStateIndex;
		};

		FunctionIntern& getFunctionIntern( const String& _functionName );
		Real getFunctionValue( const String& _functionName );

	private:
		// Private lists
		typedef vector<AtmosphereIntern>::type   AtmosphereInterns;
		typedef vector<BodyIntern>::type         BodyInterns;
		typedef vector<CloudLayerIntern>::type   CloudLayerInterns;
		typedef vector<MaterialIntern>::type     MaterialInterns;
		typedef vector<SoundIntern>::type        SoundInterns;
		typedef vector<SkyIntern>::type          SkyInterns;
		typedef vector<OverlayIntern>::type      OverlayInterns;
		typedef vector<FunctionIntern>::type FunctionInterns;

	public:
		// Iterator which skips entries unused by the current camera
		template<typename ContainerType, typename PublicValueType>
		class ConstVectorIterator2 : public IteratorWrapper<ContainerType, 
			typename ContainerType::const_iterator, PublicValueType>
		{
		public:
			ConstVectorIterator2 ( const ContainerType& _container )
				: IteratorWrapper<ContainerType, ContainerType::const_iterator, PublicValueType>( _container.begin(), _container.end() ) 
			{
				searchNextUsedByCurrentCamera();
			}

			const PublicValueType& peekNext() const
			{
				return *mCurrent;
			}

			const PublicValueType& getNext() 
			{
				const PublicValueType& val = *(mCurrent++);
				searchNextUsedByCurrentCamera();
				return val;
			}  

		private:
			void searchNextUsedByCurrentCamera()
			{
				while( mCurrent != mEnd && !mCurrent->isUsedByCurrentCamera() )
					++mCurrent;
			}
		};

		// Public iterators
		class AtmosphereIterator : public ConstVectorIterator2<AtmosphereInterns, Atmosphere> 
		{
		public:
			AtmosphereIterator( const AtmosphereInterns& _container )
				: ConstVectorIterator2<AtmosphereInterns, Atmosphere>( _container ) {}
		};

		class BodyIterator : public ConstVectorIterator2<BodyInterns, Body> 
		{
		public:
			BodyIterator( const BodyInterns& _container )
				: ConstVectorIterator2<BodyInterns, Body>( _container ) {}
		};

		class CloudLayerIterator : public ConstVectorIterator2<CloudLayerInterns, CloudLayer> 
		{
		public:
			CloudLayerIterator( const CloudLayerInterns& _container )
				: ConstVectorIterator2<CloudLayerInterns, CloudLayer>( _container ) {}
		};

		class SoundIterator : public ConstVectorIterator2<SoundInterns, Sound> 
		{
		public:
			SoundIterator( const SoundInterns& _container )
				: ConstVectorIterator2<SoundInterns, Sound>( _container ) {}
		};

		class MaterialIterator : public ConstVectorIterator2<MaterialInterns, Material> 
		{
		public:
			MaterialIterator( const MaterialInterns& _container )
				: ConstVectorIterator2<MaterialInterns, Material>( _container ) {}
		};

	private:
		// Visible sky
		struct SkyWeight
		{
			Sky*  sky;
			Real  weight;
			Real  pointWeight;
			bool operator <(const SkyWeight& _rhs) const;
		};

		typedef vector<SkyWeight>::type SkyWeights;

		void findVisibleSkies();
		void sortVisibleSkies();
		static Real calculatePointWeight(const Vector3& _pos, const AxisAlignedBox& aab, Real _innerPart);

	private:
		void resetUsedByCurrentCamera();
		void resetUsedInCurrentFrame();
		void deleteUnusedInCurrentFrame();
		
		void clearModSeqs();
		void calculateModSeqs();
		void calculateDerivedTransforms();

		void buildModSeqs();
		void buildModSeqsForSky( const String& _skyName, Real _weight );
		void buildModSeqsForBackground( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForFog( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForFarClippingPlane( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForAtmospheres( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForBodies( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForCloudLayers( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForSounds( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForMaterials( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForOverlays( const SkyDefPtr& _skyDef, Real _weight );
		void buildModSeqsForOverlaySkies();

		void calculateLightFilters();
		void applyLightFilters( int _layer, const ColourValue& _ambientLightFilter, 
			const ColourValue& _diffuseLightFilter, const ColourValue& _specularLightFilter );

		void calculateUnderwaterDepth();
		void calculateIndoorDepth();
		Real calcIndoorDepthRecursively( const Vector3& _position, const String& _sectorName );
		Real getUnderwaterDepth() const;
		Real getIndoorDepth() const;

		GOTHOGRE_DECLARE_FRAME_ENDED( SkyCalculator );

		static const Real WEIGHT_EPSILON;

	private:
		SceneManagerExImpl*     mSceneManager;
		Camera*                 mCurrentCamera;
		SkyWeights		        mSkyWeights;
		BackgroundIntern*       mBackgroundIntern;
		FogIntern*              mFogIntern;
		FarClippingPlaneIntern* mFarClippingPlaneIntern;
		AtmosphereInterns       mAtmosphereInterns;
		BodyInterns             mBodyInterns;
		CloudLayerInterns       mCloudLayerInterns;
		MaterialInterns         mMaterialInterns;
		SoundInterns            mSoundInterns;
		SkyInterns              mSkyInterns;
		OverlayInterns          mOverlayInterns;
		FunctionInterns         mFunctionInterns;
		Real                    mUnderwaterDepth;
		Real                    mIndoorDepth;
	};

} // namespace GothOgre

#endif // SKY_CALCULATOR_H