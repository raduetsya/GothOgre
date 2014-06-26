#ifndef SKY_PRODUCER_H
#define SKY_PRODUCER_H


namespace GothOgre
{
	class SceneManagerExImpl;

	//-------------------------------------------------------------------------
	class SkyProducer
	{
	public:
		SkyProducer(SceneManagerExImpl* _sm);
		~SkyProducer();

		/** Called prior to the scene being rendered. */
		void queueForRendering(Camera* _camera);

	private:
		static Real calculatePointWeight(const Vector3& _pos, const AxisAlignedBox& _aab, Real _innerPart);

		void findVisibleSkyObjs(Camera* _camera);
		
		void findVisibleSkies();

		GOTHOGRE_DECLARE_FRAME_ENDED(SkyProducer);

	private:
		struct VisibleSkyObj
		{
			SkyObj* skyObj;
			Real    weight;

			VisibleSkyObj(SkyObj* _skyObj, Real _weight);
			~VisibleSkyObj();
			bool operator <(const VisibleSkyObj& _rhs) const;
		};
		typedef vector<VisibleSkyObj>::type VisibleSkyObjs;
		VisibleSkyObjs  mVisibleSkyObjs;

	private:
		struct SkyWeight
		{
			SkyPtr   sky;
			Real     weight;

			SkyWeight(SkyPtr _sky, Real _weight);
			~SkyWeight();
		};
		typedef vector<SkyWeight>::type VisibleSkies;
		

		struct BackgroundVisualizer
		{
			Real nearDistance;
			Real farDistance;


		};

		struct FogVisualizer
		{

		};

		struct AtmosphereVisualizer
		{

		};

		struct BodyVisualizer
		{

		};

		struct CloudLayerVisualizer
		{

		};

	private:
		class ControllerWorker
		{
		public:
			virtual const String& getFunctionName() = 0;
			virtual void control() = 0;
		};

		template<typename CTRL>
		class ControllerWorkerT : public ControllerWorker
		{
		public:
			ControllerWorkerT(const CTRL& _ctrl) : mController( &_ctrl) {}
			const CTRL& getController() const {return *mController;}
			const String& getFunctionName() {return mController->functionName;}

		private:
			const CTRL* mController;
		};


		class BackgroundControllerWorker : 
			public ControllerWorkerT<SkyDef::BackgroundController>
		{
		public:
			BackgroundControllerWorker( const SkyDef::BackgroundController& _controller )
				: ControllerWorkerT<SkyDef::BackgroundController>( _ctrl ) {}

			void control();
		};


		typedef vector<ControllerWorker>::type ControllerWorkers;
		ControllerWorkers mControllerWorkers;


		void sortControllerWorkers()
		{
			ControllerWorkers controllerWorkers2;
			for(size_t i = 0; i != mControllerWorkers.size(); ++i)
			{
				ControllerWorker& cw = mControllerWorkers[i];


			}

			typedef list<ControllerWorker>::type List;
			typedef list<List>::type ListOfLists;
			ListOfList
		}

		bool precede( const ControllerWorker& cwL, const ControllerWorker& cwR )
		{
			const String& funcNameR = cwR.getFunctionName();
			FunctionCalcInfo& fciR = getFunctionCalcInfo( funcNameR );
			SkyDef::Function* funcR = fciR.function;
			SkyDef::Function::Argument argR = funcR->argument;

			switch(argR)
			{
				case SkyDef::Function::ALTITUDE_OF_BODY:
				{
					if (cwL->type != BODY_CONTROLLER)
						break;

					SkyDef::BodyController* ctrlL = (SkyDef::BodyController*) (cwL->controller);
					if (funcR->bodyName != ctrlL->nameOfControlledBody)
						break;

					return true;
				}

				case SkyDef::Function::WORLD_TIME_REL:
				case SkyDef::Function::SYSTEM_TIME_REL:
				{
					if (cwL->type != OVERLAY_CONTROLLER)
						break;

					SkyDef::OverlayController& ctrlL = (SkyDef::OverlayController&) (cwL->controller);
					SkyDef::Overlay* overlay = ctrlL->nameOfControlledOverlay;
					if (funcR->skyName != overlay->skyName )
						break;

					return true;
				}
			}
			return false;
		}

	private:
		struct FunctionCalcInfo
		{
			SkyDef::Function* function;
		};
		typedef map<String, FunctionCalcInfo>::type FunctionCalcInfoByName;

		class FunctionCalculator
		{
		public:
			FunctionCalculator(const SkyDef::Function& _function) 
				: mFunction( &_function ) {}

			const SkyDef::Function& getFunction() const
			{
				return *mFunction;
			}

			double getValue();

		private:
			SkyDef::Function*  mFunction;
			double             mValue;
		};




	////////////////////////////////////////////////////////////////////
		struct VisibleSky
		{
			SkyPtr   sky;
			Real     weight;
			bool     hasVisibleLayer;

			VisibleSky(SkyPtr _sky, Real _weight);
			~VisibleSky();
		};
		typedef vector<VisibleSky>::type VisibleSkies;		

		struct WeatherInfo
		{
			SkyPtr   parentSky;
			size_t   parentSkyStateCount;
			Weather* weather;
			Time     weatherStartTime;
			Time     weatherEndTime;
			Real     weatherWeight;
			SkyPtr   weatherOverlaySky;
			Time     lastUpdateTime;
			bool     inUse;

			WeatherInfo(const SkyPtr& _parentSky);
			~WeatherInfo()
		};
		typedef map<SkyPtr, WeatherInfo>::type WeatherInfoMap;

		typedef set<int>::type LayerIndices;

		struct BodyInfo
		{
			SkyBody*     skyBody;
			SkyPtr       parentSky;
			size_t       parentSkyStateCount;
			Real         weight;
			Radian       altitude;
			Radian       azimuth;
			Quaternion   orientation;
			Vector3      scale;
			ColourValue  ambientLightColourAdd;
			ColourValue  diffuseLightColour;
			ColourValue  specularLightColour;
			ColourValue  bodyColourMul;
			ColourValue  cloudColourMul;
			size_t       frameIndex;
			Real         distanceFromObserver;
			Vector3      directionFromObserver;
			int          layerIndex;
			Entity*      entity;
			Light*       light;
			SceneNode*   sceneNode;
			bool         visible;
			bool         inUse;
			SkyBody::ColourValueAtPointList atmosphereColourAtPointList[2];
			Real                            atmosphereColourRatio;

			BodyInfo(SkyBody* _body);
			~BodyInfo();
		};

		typedef map<SkyBody*, BodyInfo>::type BodyInfoMap;

		struct CloudLayerInfo
		{
			SkyCloudLayer* skyCloudLayer;
			SkyPtr         parentSky;
			size_t         parentSkyStateCount;
			Real           weight;
			Vector3        position;
			Quaternion     orientation;
			Vector3        scale;
			ColourValue    cloudColourMul;
			ColourValue    ambientLightColourMul;
			ColourValue    ambientLightColourAdd;
			ColourValue    diffuseLightColourMul;
			ColourValue    specularLightColourMul;
			int            layerIndex;
			Entity*        entity;
			SceneNode*     sceneNode;
			bool           visible;
			bool           inUse;

			CloudLayerInfo(SkyBody* _body);
			~CloudLayerInfo();
		};

		typedef map<SkyCloudLayer*, CloudLayerInfo>::type CloudLayerInfoMap;

		struct AtmosphereInfo
		{
			SkyAtmosphere* skyAtmosphere;
			SkyPtr         parentSky;
			size_t         parentSkyStateCount;
			Real           weight;
			int            layerIndex;
			ColourValue    ambientColourAdd;
			ColourValue    baseAirColour;
			MeshPtr        mesh;
			vector<Vector3>::type         points;
			HardwareVertexBufferSharedPtr colourBuffer;
			Entity*        entity;
			SceneNode*     sceneNode;
			bool           visible;
			bool           inUse;

			AtmosphereInfo(SkyAtmosphere* _atmosphere);
			~AtmosphereInfo();
		};

		typedef map<SkyAtmosphere*, AtmosphereInfo>::type AtmosphereInfoMap;

	private:
		SceneManagerExImpl*  mSceneManager;
		Camera*              mCamera;
		VisibleSkyObjs       mVisibleSkyObjs;
		WeatherInfoMap       mWeatherInfoMap;
		VisibleSkies         mVisibleSkies;
		LayerIndices         mLayerIndices;
		BodyInfoMap          mBodyInfoMap;
		CloudLayerInfoMap    mCloudLayerInfoMap;
		AtmosphereInfoMap    mAtmosphereInfoMap;
		SceneNode*           mSkyRootSceneNode;
	};

} // namespace GothOgre

#endif // SKY_PRODUCER_H