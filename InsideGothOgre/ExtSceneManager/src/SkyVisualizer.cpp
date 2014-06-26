#include "Precompiled.h"
#include "SkyProducer.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	class FunctionCalcInfo
	{
	public:
		Real getArgument();
		Real getValue();

	private:
		SkyDef::Function*  mFunction;
		Real               mLastValue;
		Real               mLastArgument;
	};
	//-----------------------------------------------------------------------
	void 
	FunctionCalculator::FunctionCalculator(const SkyDef::Function& _function)
	{
		mFunction = &_function;
		mLastArgument = 0;
		mLastValue = mFunction.initValue;
	}
	//-----------------------------------------------------------------------
	Real SkyProducer::getFunctionArgument()
	{
		switch(mFunction->argument)
		{
			case SkyDef::Function::WORLD_TIME:
			{
				argument = (Real) WorldTime::getSingleton().getCurrentTime();
				break;
			}
			case SkyDef::Function::WORLD_TIME_REL:
			{
				argument = (Real) WorldTime::getSingleton().getCurrentTime();
				argument -= mFunction->skyName
				break;
			}
			case SkyDef::Function::SYSTEM_TIME:
			{
				argument = (Real) WorldTime::getSingleton().getCurrentTime();
				break;
			}
			case SkyDef::Function::ALTITUDE_OF_BODY:
			{
				const String& bodyName = mFunction->bodyName;

			}
			case SkyDef::Function::UNDERWATER_DEPTH:
			{

			}
			case SkyDef::Function::INDOOR_DEPTH:
			{


			}
	}
	//-----------------------------------------------------------------------
	Real FunctionCalculator::getValue()
	{
		Real argument = getArgument();

		Real value;
		switch(mFunction->type)
		{
			case CONSTANT:
			{
				functionValue = mFunction->value;
				break;
			}
			case LINEAR:
			{
				Real
				value = (getArgument() - mLastArgument) * mFunction->rate;
				break;
			}
		}

		mLastValue = value;
		mLastArgument = 
		return functionValue;
	}



	//-----------------------------------------------------------------------
	// Utilities
	//-----------------------------------------------------------------------

	// Calculates weight (0..1) of a point relative to a bounding box.
	// The function returns 0 if a point is outside a specified bounding box;
	// the function returns 1 if a point is inside the inner part of the bounding box;
	// the function returns a value from 0 to 1 elsewhere.
	static Real calculatePointWeight(const Vector3& _pos, const AxisAlignedBox& aab, Real _innerPart)
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
			if(_halfSize[i] == 0)
				return 0;
			f[i] = Math::Abs((_pos[i] - center[i]) / halfSize[i]);
		}

		Real fmax = std::max(f[0], std::max(f[1], f[2]));
		if(f >= 1)
			return 0;
		else if(f <= _innerPart)
			return 1;
		else // _innerPart < f < 1
			return (1 - f) / (1 - _innerPart);
	}



	//--------------------------------------------------------------------------------------
	// Searching visible sky objects (by bounding boxes)
	//--------------------------------------------------------------------------------------
	void SkyProducer::findVisibleSkyObjs(Camera* _camera)
	{
		mCamera = _camera;
		mVisibleSkyObjs.clear();

		// The camera's position
		const Vector3& worldPos = _camera->getPositionForViewUpdate();

		// Find sky objects which bounding boxes contains the camera
		SceneManager::MovableObjectIterator moIt = mSceneManager->getMovableObjectIterator(
			SkySystem::FACTORY_TYPE_NAME);
		while(moIt.hasMoreElements())
		{
			SkyObj* skyObj = (SkyObj*) (moIt->getNext());
			if(skyObj->getVisible() && 
				(skyObj->getVisibilityFlags() & mSceneManager->_getCombinedVisibilityMask()))
			{
				Real innerPart = skyObj->getInnerPart();
				Real weight = 0;
				if(skyObj->isSkyBoundingBoxInLocalSpace())
				{
					if(skyObj->isInScene())
					{
						Vector3 localPos = _camera->getParentNode()->convertWorldToLocalPosition(worldPos);
						const AxisAlignedBox& localBBox = skyObj->getSkyBoundingBox();
						weight = calculatePointWeight(localPos, localBBox, innerPart);
					}
				}
				else
				{
					const AxisAlignedBox& worldBBox = skyObj->getSkyBoundingBox();
					weight = calculatePointWeight(worldPos, worldBBox, innerPart);
				}

				if(weight > 0)
					mVisibleSkyObjs.push_back(VisibleSkyObj(skyObj, weight));
			}
		}

		// Sort the founded sky objects by priority (in ascending order)
		std::sort(mVisibleSkyObjs.begin(), mVisibleSkyObjs.end());
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::VisibleSkyObj::VisibleSkyObj(SkyObj* _skyObj, Real _weight) 
		: skyObj(_skyObj), weight(_weight) 
	{
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::VisibleSkyObj::~VisibleSkyObj()
	{
	}
	//--------------------------------------------------------------------------------------
	bool SkyProducer::VisibleSkyObj::operator <(const VisibleSkyObj& _rhs) const 
	{
		return skyObj->getPriority() < _rhs.skyObj->getPriority();
	}



	//--------------------------------------------------------------------------------------
	// Searching visible skies (by bounding boxes)
	//--------------------------------------------------------------------------------------
	void SkyProducer::findVisibleSkies()
	{
		mVisibleSkies.clear();
		for(VisibleSkyObjs::iterator it = mVisibleSkyObjs.begin(); 
			it != mVisibleSkyObjs.end(); ++it)
		{
			VisibleSkyObj& sow = mVisibleSkyObjs[i];
			SkyObj* skyObj = sow.skyObj;
			Real weight = sow.weight;
			SkyPtr sky = skyObj->getSky();
			addVisibleSky(sky, weight);
		}
	}
	//--------------------------------------------------------------------------------------
	void SkyProducer::addVisibleSky(const SkyPtr& _sky, Real _weight)
	{
		mVisibleSkies.push_back(VisibleSky(_sky, _weight));
		updateWeather(_sky, _weight);
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::VisibleSky::VisibleSky(SkyPtr _sky, Real _weight) 
	{
		sky = _sky;
		weight = _weight;
		hasVisibleLayer = false;
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::VisibleSky::~VisibleSky()
	{
	}




	//--------------------------------------------------------------------------------------
	// Updating the current weather
	//--------------------------------------------------------------------------------------
	void SkyProducer::updateWeather(const SkyPtr& _parentSky, Real _parentSkyWeight)
	{
		WeatherInfoMap::iterator itWI = mWeatherInfoMap.find(_parentSky);
		if(itWI == mWeatherInfoMap.end())
		{
			itWI = mWeatherInfoMap.insert(std::make_pair(_parentSky, 
				WeatherInfo(_parentSky))).first;
		}
	
		WeatherInfo& wi = itWI->second;
		updateWeatherInfo(wi);

		if(wi.weather)
		{
			addVisibleSky(wi.weatherOverlaySky, wi.weatherWeight * _parentSkyWeight);
		}
	}
	//--------------------------------------------------------------------------------------
	void SkyProducer::updateWeatherInfo(WeatherInfo& _wi, Real _parentSkyWeight)
	{
		// The parent sky was changed, we need recalculate the weather too.
		if(_wi.parentSkyStateCount != _wi.parentSky->getStateCount())
		{
			_wi.weather = nullptr;
			_wi.weatherOverlaySky.setNull();
			_wi.parentSkyStateCount = _wi.parentSky->getStateCount();
		}

		// Check the end time.
		Time curTime = WorldTime::getCurrentTime();
		if(_wi.weatherEndTime > curTime)
		{
			_wi.weather = nullptr;
			_wi.weatherOverlaySky.setNull();
		}

		// Find an appropriate weather for starting.
		if(!_wi.weather)
		{
			Sky::WeatherIterator itW = _sky->getWeatherIterator();
			while(itW.hasMoreElements())
			{
				SkyWeather* weather = itW.getNext();
				const SkyWeather::ChanceAndDurationForTimeList& list = weather->getChanceAndDurationForTimeList();
				Real ratio;
				std::pair<size_t, size_t> interval;
				interval = findIntervalForTime(curTime, list, &ratio);
				if(ratio >= 0)
				{
					const SkyWeather::ChanceAndDurationForTime& elem0 = list[interval.first];
					const SkyWeather::ChanceAndDurationForTime& elem1 = list[interval.second];

					float startChancePerSecond = 
						elem0.startChancePerSecond * ratio +
						elem1.startChancePerSecond * (1-ratio);
					
					Time elapsedTime = curTime - _wi.lastUpdateTime;
					if(PreciseUnitRandom() <= startChancePerSecond * elapsedTime)
					{
						Time minDuration = 
							elem0.minDurationIfStart * ratio +
							elem1.minDurationIfStart * (1-ratio); 
						Time maxDuration = 
							elem0.maxDurationIfStart * ratio +
							elem1.maxDurationIfStart * (1-ratio);
						Time duration = (Time) Math::RangeRandom(minDuration, maxDuration);

						_wi.weather = weather;
						_wi.weatherStartTime = curTime;
						_wi.weatherEndTime = curTime + duration;
						_wi.weatherOverlaySky = SkyManagerEx::getSingleton().create(weather->getWeatherName());
						break;
					}
				}
			}
		}

		// Calculate the weather's weight.
		if(_wi.weather)
		{
			float f0 = 0;
			if(_wi.weather->getGrowTime() != 0)
				f0 = (curTime - _wi.weatherStartTime) / _wi.weather->getGrowTime();

			float f1 = 0;
			if(_wi.weather->getDiminishTime() != 0)
				f1 = (_wi.weatherEndTime - curTime) / _wi.weather->getDiminishTime();

			_wi.weatherWeight = Math::Clamp(std::min(f0, f1), 0.0f, 1.0f);
		}

		// Set the new time of the last update
		_wi.lastUpdateTime = curTime;

		// Set the weather info as used.
		_wi.inUse = true;
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::WeatherInfo::WeatherInfo(const SkyPtr& _parentSky)
	{
		parentSky = _parentSky;
		parentSkyStateCount = _parentSky->getStateCount() - 1;
		weather = nullptr;
		inUse = false;
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::WeatherInfo::~WeatherInfo()
	{
	}





	//--------------------------------------------------------------------------------------
	// Fog and background, calculations
	//-----------------------------------------------------------------------
	void SkyProducer::calculateFog()
	{
		ColourValue fogColour = ColourValue(0, 0, 0, 0);
		Real fogNearDistance = 0;
		Real fogFarDistance = 0;
		Real lastWeight = 0;

		for(VisibleSkies::reverse_iterator itVS = mVisibleSkies.rend();
			itVS != mVisibleSkies.rbegin(); ++itVS)
		{
			VisibleSky& vs = *itVS;
			Real preWeight = vs.weight;
			const SkyPtr& sky = vs.sky;
			SkyFog* skyFog = sky->getSkyFog();
			
			if(preWeight > 0 && skyFog)
			{
				vs.hasVisibleLayer = true;
				Real weight = (1 - lastWeight) * preWeight;
				lastWeight = weight;

				fogNearDistance += weight * skyFog->getNearDistance();
				fogFarDistance  += weight * skyFog->getFarDistance();
				if(skyFog->getColour().a != 0)
				{
					fogColour += weight * skyFog->getColour();
				}

				if(preWeight == 1)
					break;
			}
		}

		if(fogColour.a != 0)
			mSceneManager->setFog(FOG_LINEAR, fogColour, 0, fogNearDistance, fogFarDistance);
		else
			mSceneManager->setFog(FOG_NONE);
	}
	//-----------------------------------------------------------------------
	void SkyProducer::calculateBackgroundColour()
	{
		ColourValue backgroundColour = ColourValue(0, 0, 0, 0);
		Real lastWeight = 0;

		for(VisibleSkies::reverse_iterator itVS = mVisibleSkies.rend();
			itVS != mVisibleSkies.rbegin(); ++itVS)
		{
			VisibleSky& vs = *itVS;
			Real preWeight = vs.weight;
			const SkyPtr& sky = vs.sky;
			SkyBackground* skyBackground = sky->getSkyBackground();
			
			if(preWeight > 0 && skyBackground)
			{
				vs.hasVisibleLayer = true;
				Real weight = (1 - lastWeight) * preWeight;
				lastWeight = weight;
			
				if(skyBackground->getColour().a != 0)
				{
					backgroundColour += weight * skyBackground->getColour();
				}

				if(preWeight == 1)
					break;
			}

		}

		backgroundColour.a = 1;
		mSceneManager->getCurrentViewport()->setBackgroundColour(backgroundColour);
	}



	//--------------------------------------------------------------------------------------
	// Building the list of all layer indices
	//--------------------------------------------------------------------------------------
	void SkyProducer::buildListOfAllLayerIndices()
	{
		mLayerIndices.clear();
		for(VisibleSkies::reverse_iterator itVS = mVisibleSkies.rend();
			itVS != mVisibleSkies.rbegin(); ++itVS)
		{
			VisibleSky& vs = *itVS;
			const SkyPtr& sky = vs.sky;
			SkyAtmosphere* skyAtmosphere = sky->getSkyAtmosphere();
			mLayerIndices.insert(skyAtmosphere->getLayerIndex());
			Sky::BodyIterator bodyIt = sky->getBodyIterator();
			while(bodyIt.hasMoreElements())
			{
				SkyBody* skyBody = bodyIt.getNext();
				mLayerIndices.insert(skyBody->getLayerIndex());
			}
			Sky::CloudLayerIterator cloudLayerIt = sky->getCloudIterator();
			while(cloudLayerIt.hasMoreElements())
			{
				SkyCloudLayer* cloudLayer = cloudLayerIt.getNext();
				mLayerIndices.insert(cloudLayer->getLayerIndex());
			}
		}
	}




	//--------------------------------------------------------------------------------------
	// Moving the skies' root scene node to the current position of the camera
	//--------------------------------------------------------------------------------------
	void SkyProducer::moveSkyRootSceneNode()
	{
		// Create the scene node if it's not created yet.
		if(!mSkyRootSceneNode)
		{
			mSkyRootSceneNode = mSceneManager->createSceneNode();
		}

		// Reposition the scene node.
		const Vector3& cameraPosition = mCamera->getDerivedPosition();
		mSkyRootSceneNode->setPosition(cameraPosition);
		mSkyRootSceneNode->setFixedYawAxis(true, mSceneManager->getNorthDirection());
		mSkyRootSceneNode->setDirection(mSceneManager->getUpDirection(), TS_LOCAL, Vector3::UNIT_Z);
	}




	//--------------------------------------------------------------------------------------
	// Celestial bodies, calculations
	//--------------------------------------------------------------------------------------
	void SkyProducer::calculateBodies()
	{
		// Set "visible" to false for all sky bodies.
		// Then "visible" will be set to true for sky bodies
		// which are visible for the current camera.
		for(BodyInfoMap::iterator itBI = mSkyBodyInfoMap.begin(); 
			itBI != mSkyBodyInfoMap.end(); ++itBI)
		{
			BodyInfo& bi = itBI->second;
			bi.visible = false;
		}

		// Iterate through all layer indices.
		// The weights are calculated for different layers independently.
		for(LayerIndices::iterator itLI = mLayerIndices.begin(); 
			itLI != mLayerIndices.end(); ++itLI)
		{
			int layerIndex = *itLI;
			
			Real lastWeight = 0;

			for(VisibleSkies::reverse_iterator itVS = mVisibleSkies.rend();
				itVS != mVisibleSkies.rbegin(); ++itVS)
			{
				VisibleSky& vs = *itVS;
				const SkyPtr& sky = vs.sky;

				// Calculate whether the sky contains a sky body in the current layer
				bool presentSkyBody = false;
				Sky::BodyIterator itBody = sky->getBodyIterator();
				while(itBody.hasMoreElements())
				{
					SkyBody* body = itBody.getNext();
					if(body->getLayerIndex() == layerIndex)
					{
						presentSkyBody = true;
						break;
					}
				}

				// Calculate sky bodies which are in the current layer
				Real preWeight = vs.weight;
				if(preWeight > 0 && presentSkyBody)
				{
					vs.hasVisibleLayer = true;
					Real weight = (1 - lastWeight) * preWeight;
					lastWeight = weight;

					Sky::BodyIterator itBody = sky->getBodyIterator();
					while(itBody.hasMoreElements())
					{
						SkyBody* body = itBody.getNext();
						if(body->getLayerIndex() == layerIndex)
						{
							BodyInfoMap::iterator itBI = mBodyInfoMap.find(body);
							if(itBI == mSkyBodyInfoMap.end())
							{
								itBI = mSkyBodyInfoMap.insert(std::make_pair(body, BodyInfo(body))).first;
							}
							BodyInfo& bi = itBI->second;
							bi.weight = weight;
							bi.visible = true;
							bi.inUse = true;
							calculateBody(bi);
						}
					}

					if(preWeight == 1)
						break;
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void SkyProducer::calculateBody(BodyInfo& _bi)
	{
		SkyBody* body = _bi.skyBody;
		Time curTime = WorldTime::getSingleton().getCurrentTime();

		// Process changes in the source sky
		if(_bi.parentSkyStateCount != _bi.parentSky->getStateCount())
		{
			if(_bi.entity)
			{
				mSceneManager->destroyEntity(_bi.entity);
				_bi.entity = nullptr;
			}
			_bi.entity = mSceneManager->createEntityEx(body->getMesh(), SceneQueryMask::SKY);

			if(!_bi.sceneNode)
			{
				_bi.sceneNode = mSkyRootSceneNode->createChildSceneNode();
			}
			_bi.sceneNode->attachObject(_bi.entity);
			_bi.distanceFromObserver = body->getDistanceFromObserver();
			_bi.layerIndex = body->getLayerIndex();
			_bi.parentSkyStateCount = _bi.parentSky->getStateCount();
		}

		// Calculate new coordinates of the celesial body.
		const SkyBody::PositionForTimeList& listP = body->getPositionForTimeList();
		Real ratioP;
		std::pair<size_t, size_t> intervalP;
		intervalP = findIntervalForTime(listP, curTime, &ratioP);
		if(ratioP >= 0)
		{
			// Linear interpolation
			SkyBody::PositionForTime& elemP0 = listP[intervalP.first];
			SkyBody::PositionForTime& elemP1 = listP[intervalP.second];
			_bi.altitude    = elemP0.altitude    * ratioP + elemP1.altitude    * (1-ratioP);
			_bi.azimuth     = elemP0.azimuth     * ratioP + elemP1.azimuth     * (1-ratioP);
			_bi.orientation = elemP0.orientation * ratioP + elemP1.orientation * (1-ratioP);
			_bi.scale       = elemP0.scale       * ratioP + elemP1.scale       * (1-ratioP);
		}
		else
		{
			// Cound not interpolate coordinates, use default coordinates
			_bi.altitude = Math::HALF_PI;
			_bi.azimuth = 0;
			_bi.orientation = Quaternion::IDENTITY;
			_bi.scale = Vector3::UNIT_SCALE;
		}

		// Calculate contribution to colours
		const SkyBody::ColourForAltitudeList& listC = body->getColourForAltitudeList();
		Real ratioC;
		std::pair<size_t, size_t> intervalC;
		intervalC = findIntervalForAltitude(listC, _bi.altitude, &ratioC);
		if(ratioC >= 0)
		{
			// Linear interpolation
			SkyBody::ColourForAltitude& elemC0 = listC[intervalC.first];
			SkyBody::ColourForAltitude& elemC1 = listC[intervalC.second];
			_bi.ambientLightColourAdd = elemC0.ambientLightColourAdd * ratioC + elemC1.ambientLightColourAdd * (1-ratioC);
			_bi.diffuseLightColour    = elemC0.diffuseLightColour    * ratioC + elemC1.diffuseLightColour    * (1-ratioC);
			_bi.specularLightColour   = elemC0.specularLightColour   * ratioC + elemC1.specularLightColour   * (1-ratioC);
			_bi.bodyColourMul         = elemC0.bodyColourMul         * ratioC + elemC1.bodyColourMul         * (1-ratioC);
			_bi.cloudColourMul        = elemC0.cloudColourMul        * ratioC + elemC1.cloudColourMul        * (1-ratioC);
			_bi.atmosphereColourAtPointList0 = elemC0.atmosphereColourAtPointList;
			_bi.atmosphereColourAtPointList1 = elemC1.atmosphereColourAtPointList;
			_bi.atmosphereColourRatio = ratioC;
		}
		else
		{
			// Cound not interpolate colours, use default values
			_bi.ambientLightColourAdd = ColourValue(0, 0, 0, 0);
			_bi.diffuseLightColour    = ColourValue(0, 0, 0, 0);
			_bi.specularLightColour   = ColourValue(0, 0, 0, 0);
			_bi.bodyColourMul         = ColourValue(1, 1, 1, 1);
			_bi.cloudColourMul        = ColourValue(1, 1, 1, 1);
			_bi.atmosphereColourAtPointList0.clear();
			_bi.atmosphereColourAtPointList1.clear();
			_bi.atmosphereColourRatio = 0;
		}

		// Use the weight in the calculation
		_bi.ambientLightColourAdd *= _bi.weight;
		_bi.diffuseLightColour    *= _bi.weight;
		_bi.specularLightColour   *= _bi.weight;
		_bi.bodyColourMul         *= _bi.weight;
		_bi.cloudColourMul = _bi.cloudColourMultiplier * _bi.weight + ColourValue::White * (1 - _bi.weight);

		for(ColourValueAtPointList::iterator it0 = _bi.atmosphereColourAtPointList0.begin();
			it0 != _bi.atmosphereColourAtPointList0.end(); ++it0)
		{
			(*it0).atmosphereColourAdd *= _bi.weight;
		}
		for(ColourValueAtPointList::iterator it1 = _bi.atmosphereColourAtPointList1.begin();
			it1 != _bi.atmosphereColourAtPointList1.end(); ++it1)
		{
			(*it1).atmosphereColourAdd *= _bi.weight;
		}

		// Calculate phase of the celestial body
		const SkyBody::PhaseForTimeList& listF = body->getPhaseForTimeList();
		Real ratioF;
		std::pair<size_t, size_t> intervalF;
		intervalF = findIntervalForTime(listF, curTime, &ratioF);
		if(ratioF >= 0)
		{
			// Linear interpolation
			SkyBody::PhaseForTime& elemF0 = listF[intervalF.first];
			SkyBody::PhaseForTime& elemF1 = listF[intervalF.second];
			_bi.frameIndex = (size_t) floor(elemF0.frameIndex * ratioF + elemF1.frameIndex * (1-ratioF) + 0.5);
		}
		else
		{
			// Cound not interpolate phase, use default values
			_bi.frameIndex = -1;
		}

		// Apply a material to the celestial body
		MatGenParams mtlApplyParams;
		mtlApplyParams.setColourMultiplier(_bi.bodyColourMul);
		mtlApplyParams.setCurrentFrame(_bi.frameIndex);
		MaterialPtr material = mSceneManager->generateMaterial(_bi.mtl, mtlApplyParams);
		_bi.entity->setMaterial(material);

		// Reposition the scene node
		Vector3 directionFromObserver = Vector3::UNIT_Z * Math::Cos(_info.altitude)
			+ (Vector3::UNIT_Y * Math::Cos(_info.azimuth) 
			+ Vector3::UNIT_X * Math::Sin(_info.asimuth)) * Math::Sin(_info.altitude);

		_bi.sceneNode->setPosition(directionFromObserver * _bi.distanceFromObserver);
		_bi.sceneNode->setFixedYawAxis(true, Vector3::UNIT_Y);
		_bi.sceneNode->setDirection(-directionFromObserver, TS_WORLD, Vector3::UNIT_Z);
		_bi.sceneNode->rotate(_bi.orientation, TS_LOCAL);
		_bi.sceneNode->setScale(_bi.scale);

		// Setup the directional light
		if(_bi.directLightColour.r   != 0 || _bi.directLightColour.g   != 0 || _bi.directLightColour.b   != 0
		|| _bi.specularLightColour.r != 0 || _bi.specularLightColour.g != 0 || _bi.specularLightColour.b != 0
		{
			if(!_bi.light)
			{
				_bi.light = mSceneManager->createLight();
				_bi.sceneNode->attachObject(_info.light);
				_bi.light->setType(LT_DIRECTIONAL);
				_bi.light->setDirection(Vector3::UNIT_Z); // the direction of the light is local to the scene node
			}
			_bi.light->setDiffuseColour(_info.diffuseLightColour);
			_bi.light->setSpecularColour(_info.specularLightColour);
			_bi.light->setVisible(true);
		}
		else
		{
			if(_bi.light)
			{
				_bi.light->setVisible(false);
			}
		}

		// Calculate influence to atmosphere's colour

		const SkyBody::ColourForAltitudeList& listC = body->getColourForAltitudeList();
		Real ratioC;
		std::pair<size_t, size_t> intervalC;
		intervalC = findIntervalForAltitude(listC, _bi.altitude, &ratioC);
		if(ratioC >= 0)	
	}
	//-----------------------------------------------------------------------
	SkyProducer::BodyInfo::BodyInfo(SkyBody* _body)
	{
		skyBody = body;
		entity = nullptr;
		light = nullptr;
		sceneNode = nullptr;
		parentSky = _body->getSky();
		parentSkyStateCount = parentSky->getStateCount() - 1;
	}
	//-----------------------------------------------------------------------
	SkyProducer::BodyInfo::~BodyInfo()
	{
		if(entity)
			entity->_getManager()->destroyEntity(entity);

		if(light)
			light->_getManager()->destroyLight(light);

		if(sceneNode)
			sceneNode->getCreator()->destroySceneNode(sceneNode);
	}




	//--------------------------------------------------------------------------------------
	// Cloud layers
	//--------------------------------------------------------------------------------------
	void SkyProducer::calculateCloudLayers()
	{
		// Set "visible" to false for all sky bodies.
		// Then "visible" will be set to true for sky bodies
		// which are visible for the current camera.
		for(CloudLayerInfoMap::iterator itCLI = mCloudLayerInfoMap.begin(); 
			itCLI != mCloudLayerInfoMap.end(); ++itCLI)
		{
			CloudLayerInfo& cli = itCLI->second;
			cli.visible = false;
		}

		// Iterate through all layer indices.
		// The weights are calculated for different layers independently.
		for(LayerIndices::iterator itLI = mLayerIndices.begin(); 
			itLI != mLayerIndices.end(); ++itLI)
		{
			int layerIndex = *itLI;
			
			Real lastWeight = 0;

			for(VisibleSkies::reverse_iterator itVS = mVisibleSkies.rend();
				itVS != mVisibleSkies.rbegin(); ++itVS)
			{
				VisibleSky& vs = *itVS;
				const SkyPtr& sky = vs.sky;

				// Calculate whether the sky contains a cloud layer with the current layer index
				bool presentCloudLayer = false;
				Sky::CloudLayerIterator itCloudLayer = sky->getCloudLayerIterator();
				while(itCloudLayer.hasMoreElements())
				{
					SkyCloudLayer* cloudLayer = itCloudLayer.getNext();
					if(cloudLayer->getLayerIndex() == layerIndex)
					{
						presentCloudLayer = true;
						break;
					}
				}
				
				Real preWeight = vs.weight;
				if(preWeight > 0 && presentCloudLayer)
				{
					vs.hasVisibleLayer = true;
					Real weight = (1 - lastWeight) * preWeight;
					lastWeight = weight;
					
					Sky::CloudLayerIterator itCloudLayer = sky->getSkyCloudLayerIterator();
					while(itCloudLayer.hasMoreElements())
					{
						SkyCloudLayer* cloudLayer = itCloudLayer.getNext();
						CloudLayerInfoMap::iterator itCLI = mSkyCloudLayerInfoMap.find(cloudLayer);
						if(itCLI == mCloudLayerInfoMap.end())
						{
							itCLI = mCloudLayerInfoMap.insert(std::make_pair(cloudLayer, CloudLayerInfo(cloudLayer))).first;
						}
						CloudLayerInfo& cli = itCLI->second;
						cli.weight = weight;
						cli.visible = true;
						cli.inUse = true;
						calculateCloudLayer(cli, weight);
					}

					if(preWeight == 1)
						break;
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void SkyProducer::calculateCloudLayer(CloudLayerInfo& _cli)
	{
		SkyCloudLayer* cloudLayer = _cli.skyCloudLayer;
		Time curTime = WorldTime::getSingleton().getCurrentTime();

		// Process changes in the source sky
		if(_cli.parentSkyStateCount != _cli.parentSky->getStateCount())
		{
			if(_info.entity)
			{
				mSceneManager->destroyEntity(_cli.entity);
				_cli.entity = nullptr;
			}
			_cli.entity = mSceneManager->createEntityEx(cloudLayer->getMesh(), SceneQueryMask::SKY);
			
			if(!_cli.sceneNode)
			{
				_cli.sceneNode = mSkyRootSceneNode->createChildSceneNode();
			}
			_cli.sceneNode->attachObject(_cli.entity);
			_cli.layerIndex = cloudLayer->getLayerIndex();
			_cli.parentSkyStateCount = _cli.parentSky->getStateCount();
		}

		// Calculate new coordinates of the cloud layer.
		const SkyCloudLayer::PositionForTimeList& listP = cloudLayer->getPositionForTimeList();
		Real ratioP;
		std::pair<size_t, size_t> intervalP;
		intervalP = findIntervalForTime(listP, curTime, &ratioP);
		if(ratioP >= 0)
		{
			// Linear interpolation
			SkyBody::PositionForTime& elemP0 = listP[intervalP.first];
			SkyBody::PositionForTime& elemP1 = listP[intervalP.second];
			_cli.position    = elemP0.position    * ratioP + elemP1.position    * (1-ratioP);
			_cli.orientation = elemP0.orientation * ratioP + elemP1.orientation * (1-ratioP);
			_cli.scale       = elemP0.scale       * ratioP + elemP1.scale       * (1-ratioP);
		}
		else
		{
			// Cound not interpolate phase, use default values
			_cli.position = Vector3::ZERO;
			_cli.orientation = Quaternion::IDENTITY;
			_cli.scale = Vector3::UNIT_SCALE;
		}

		// Contribution to colours from the cloud layer
		_cli.cloudColourMul         = cloudLayer->getCloudColourMul();
		_cli.ambientLightColourMul  = cloudLayer->getAmbientLightColourMul();
		_cli.ambientLightColourAdd  = cloudLayer->getAmbientLightColourAdd();
		_cli.diffuseLightColourMul  = cloudLayer->getDiffuseLightColourMul();
		_cli.specularLightColourMul = cloudLayer->getSpecularLightColourMul();

		// Use the weight in the calculation
		_cli.cloudColourMul        *= _cli.weight;
		_cli.ambientLightColourAdd *= _cli.weight;
		_cli.ambientLightColourMul  = _cli.ambientLightColourMul  * _cli.weight + ColourValue::White * (1 - _cli.weight);
		_cli.diffuseLightColourMul  = _cli.diffuseLightColourMul  * _cli.weight + ColourValue::White * (1 - _cli.weight);
		_cli.specularLightColourMul = _cli.specularLightColourMul * _cli.weight + ColourValue::White * (1 - _cli.weight);

		// Modify lights from celestial bodies (clouds can make the scene darker).
		for(BodyInfoMap::iterator itBI = mBodyInfoMap.begin(); 
			itBI != mBodyInfoMap.end(); ++itBI)
		{
			BodyInfo& bi = itBI->second;
			if(bi.visible)
			{
				_cli.cloudColourMul *= bi.cloudColourMul;
				if(bi.layerIndex < _cli.layerIndex)
				{
					Light* light = bi.light;
					light->setDiffuseColour(light->getDiffuseColour() * _cli.diffuseLightColourMul);
					light->setSpecularColour(light->getSpecularColour() * _cli.specularLightColourMul);
				}
			}
		}

		// Apply a material to the cloud layer
		MatGenParams mtlApplyParams;
		mtlApplyParams.setColourMultiplier(_info.cloudColourMul);
		MaterialPtr material = mSceneManager->generateMaterial(_cli.mtl, mtlApplyParams);
		_cli.entity->setMaterial(material);

		// Reposition the scene node
		_cli.sceneNode->setPosition(_cli.position);
		_cli.sceneNode->setOrientation(_cli.orientation);
		_cli.sceneNode->setScale(_cli.scale);
	}
	//-----------------------------------------------------------------------
	SkyProducer::CloudLayerInfo::CloudLayerInfo(SkyCloudLayer* _cloudLayer)
	{
		skyCloudLayer = _cloudLayer;
		entity = nullptr;
		sceneNode = nullptr;
		parentSky = _cloudLayer->getSky();
		parentSkyStateCount = parentSky->getStateCount() - 1;
	}
	//-----------------------------------------------------------------------
	SkyProducer::CloudLayerInfo::~CloudLayerInfo()
	{
		if(entity)
			entity->_getManager()->destroyEntity(entity);

		if(sceneNode)
			sceneNode->getCreator()->destroySceneNode(sceneNode);
	}







	//--------------------------------------------------------------------------------------
	// Atmosphere
	//--------------------------------------------------------------------------------------
	void SkyProducer::calculateAtmospheres()
	{
		// Set "visible" to false for all sky bodies.
		// Then "visible" will be set to true for sky bodies
		// which are visible for the current camera.
		for(AtmosphereInfoMap::iterator itATI = mAtmosphereInfoMap.begin(); 
			itATI != mAtmosphereInfoMap.end(); ++itATI)
		{
			AtmosphereInfo& ati = itATI->second;
			ati.visible = false;
		}

		// Iterate through all layer indices.
		// The weights are calculated for different layers independently.
		for(LayerIndices::iterator itLI = mLayerIndices.begin(); 
			itLI != mLayerIndices.end(); ++itLI)
		{
			int layerIndex = *itLI;
			
			Real lastWeight = 0;

			for(VisibleSkies::reverse_iterator itVS = mVisibleSkies.rend();
				itVS != mVisibleSkies.rbegin(); ++itVS)
			{
				VisibleSky& vs = *itVS;
				const SkyPtr& sky = vs.sky;

				// Calculate whether the sky contains a cloud layer with the current layer index
				bool presentAtmosphere = false;
				Sky::AtmosphereIterator itAtmosphere = sky->getAtmosphereIterator();
				while(itAtmosphere.hasMoreElements())
				{
					SkyAtmosphere* atmosphere = itAtmosphere.getNext();
					if(atmosphere->getLayerIndex() == layerIndex)
					{
						presentAtmosphere = true;
						break;
					}
				}
				
				Real preWeight = vs.weight;
				if(preWeight > 0 && presentAtmosphere)
				{
					vs.hasVisibleLayer = true;
					Real weight = (1 - lastWeight) * preWeight;
					lastWeight = weight;
					
					Sky::AtmosphereIterator itAtmosphere = sky->getSkyAtmosphereIterator();
					while(itAtmosphere.hasMoreElements())
					{
						SkyAtmosphere* atmosphere = itAtmosphere.getNext();
						AtmosphereInfoMap::iterator itATI = mSkyAtmosphereInfoMap.find(atmosphere);
						if(itATI == mAtmosphereInfoMap.end())
						{
							itATI = mAtmosphereInfoMap.insert(std::make_pair(atmosphere, AtmosphereInfo(atmosphere))).first;
						}
						AtmosphereInfo& ati = itATI->second;
						ati.weight = weight;
						ati.visible = true;
						ati.inUse = true;
						calculateAtmosphere(ati, weight);
					}

					if(preWeight == 1)
						break;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	void SkyProducer::calculateAtmosphere(AtmosphereInfo& _ati)
	{
		SkyAtmosphere* atmosphere = _ati.skyAtmosphere;

		// Process changes in the source sky
		if(_ati.parentSkyStateCount != _ati.parentSky->getStateCount())
		{
			if(_info.entity)
			{
				mSceneManager->destroyEntity(_ati.entity);
				_cli.entity = nullptr;
			}
			createAtmosphereMesh(_ati);
			_ati.entity = mSceneManager->createEntityEx(_ati.mesh, SceneQueryMask::SKY);
			
			if(!_ati.sceneNode)
			{
				_ati.sceneNode = mSkyRootSceneNode->createChildSceneNode();
			}
			_ati.sceneNode->attachObject(_ati.entity);
			_ati.layerIndex = atmosphere->getLayerIndex();
			_ati.parentSkyStateCount = _ati.parentSky->getStateCount();
		}

		// Base colours
		_ati.baseAirColour         = atmosphere->getBaseAirColour();
		_ati.ambientLightColourMul = atmosphere->getAmbientLightColourMul();
		_ati.ambientLightColourAdd = atmosphere->getAmbientLightColourAdd();

		// Use the weight in the calculation
		_ati.baseAirColour         *= _ati.weight;
		_ati.ambientLightColourAdd *= _ati.weight;
		_ati.ambientLightColourMul  = _ati.ambientLightColourMul  * _ati.weight + ColourValue::White * (1 - _ati.weight);

		// Recalculate colours of the atmosphere mesh
		calculateAtmosphereColours(_ati);
	}
	//--------------------------------------------------------------------------------------
	void SkyProducer::createAtmosphereMesh(AtmosphereInfo& _ati)
	{
		// Destroy the old mesh
		if(!_ati.mesh.isNull())
		{
			MeshManagerEx::getSingleton().remove(_ati.mesh);
			_ati.mesh.setNull();
			_ati.colourBuffer.setNull();
		}

		// Get the parameters
		Real radius = atmosphere->getHemisphereRadius();
		Real depth  = atmosphere->getDepthUnderHorizont();

		// Level of details - number of segments per Pi/2
		static size_t NUM_SEGMENTS = 8;

		// Number of vertices and number of faces depend on NUM_SEGMENTS
		size_t numVertices = (NUM_SEGMENTS + 1) * 4 * NUM_SEGMENTS + 1;
		size_t numFaces = (NUM_SEGMENTS + 1) * 4 * NUM_SEGMENTS * 2;

		// Calculate coordinates of vertices of a new mesh
		_ati.points.resize(0);
		_ati.points.reserve(numVertices);
		size_t ia, ib;
		for(ia = 0; ia != NUM_SEGMENTS + 2; ++ia)
		{
			Real z, r;
			if(ia != NUM_SEGMENTS + 1)
			{
				Radian alpha = Math::Sin(ia * Math::HALF_PI / NUM_SEGMENTS);
				z = radius * Math::Cos(alpha);
				r = radius * Math::Sin(alpha);
			}
			else
			{
				z = -depth;
				r = radius;
			}

			for(ib = 0; ib != (ia != 0 ? NUM_SEGMENTS * 4 : 1); ++ib)
			{
				Radian beta = Math::HALF_PI * ib / NUM_SEGMENTS;
				Real x = r * Math::Cos(beta);
				Real y = r * Math::Sin(beta);
				_ati.points.push_back(Vector3(x, y, z));

				Vector3 normal = Vector3(-x, -y, -z).normalisedCopy();
			}
		}

		// Generate a new mesh
		String meshName = "AtmosphereMesh" + StrConv::toString( (int)(void*) _ati->atmosphere);
		MeshPtr mesh = MeshManagerEx::getSingleton().createManual(meshName);
		_ati.mesh = mesh;

		// Copy vertices' coordinates
		mesh->sharedVertexData->vertexCount = numVertices;
		mesh->sharedVertexData->vertexStart = 0;

		// Prepare a buffer with positions of vertices
		// normals are not needed because dynamic lighting is not used.
		HardwareVertexBufferSharedPtr vbuffer =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			12 /* 12(pos) */, numVertices, 
			HardwareBuffer::HBU_STATIC, false);

		float* vbufferPtr = (float*)vbuffer->lock(HardwareBuffer::HBL_DISCARD);
		for(size_t iv = 0; iv != _ati.points.size(); ++iv)
		{
			const Vector3& pos = _ati.points[iv];
			*vbufferPtr++ = pos.x;
			*vbufferPtr++ = pos.y;
			*vbufferPtr++ = pos.z;
		}
		vbuffer->unlock();

		// Prepare a buffer with colours of vertices.
		HardwareVertexBufferSharedPtr cBuffer =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			4 /* 4(RGBA) */, numVertices, 
			HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false);
		_ati.colourBuffer = cBuffer;

		// Initialise shared vertex data.
		mesh->sharedVertexData = new VertexData();
		mesh->sharedVertexData->vertexStart = 0;
		mesh->sharedVertexData->vertexCount = numVertices;

		mesh->sharedVertexData->vertexDeclaration->addElement(0, 0, 
			VET_FLOAT3, VES_POSITION, 0);
		mesh->sharedVertexData->vertexDeclaration->addElement(1, 0, 
			VET_COLOUR, VES_DIFFUSE, 0);

		mesh->sharedVertexData->vertexBufferBinding->setBinding(0, vbuffer);
		mesh->sharedVertexData->vertexBufferBinding->setBinding(1, cbuffer);

		// Generate faces
		HardwareIndexBufferSharedPtr ibuffer = 
			HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_32BIT, numFaces * 3, 
			HardwareBuffer::HBU_STATIC, false);

		uint32* ibufferPtr = (uint32*)
			ibuffer->lock(HardwareBuffer::HBL_DISCARD);

		for(ia = 1; ia != NUM_SEGMENTS + 2; ++ia)
		{
			size_t ofs = (ia - 1) * NUM_SEGMENTS * 4 + 1;
			for(ib = 0; ib != NUM_SEGMENTS * 4; ++ib)
			{
				size_t idx0 = ib + ofs;
				size_t idx1 = (ib + 1) % (NUM_SEGMENTS * 4) + ofs;
				*ibufferPtr++ = idx[0];
				*ibufferPtr++ = idx[1];
				if(ia != 1)
				{
					size_t idx2 = idx1 - NUM_SEGMENTS * 4;
					size_t idx3 = idx0 - NUM_SEGMENTS * 4;
					*ibufferPtr++ = idx2;
					*ibufferPtr++ = idx0;
					*ibufferPtr++ = idx2;
					*ibufferPtr++ = idx3;
				}
				else
				{
					size_t idx2 = 0;
					*ibufferPtr++ = idx2;
				}
			}
		}

		ibuffer->unlock();

		SubMesh* subMesh = mesh->createSubMesh();
		subMesh->useSharedVertices = true;
		IndexData* indexData = subMesh->indexData;
		indexData->indexStart = 0;
		indexData->indexCount = numFaces * 3;
		indexData->indexBuffer = ibuffer;
	}
	//--------------------------------------------------------------------------------------
	void SkyProducer::calculateAtmosphereColours(AtmosphereInfo& ati)
	{
		HardwareVertexBufferSharedPtr cBuffer = _ati.colourBuffer;
		uint32* cbufferPtr = (uint32*)
			cbuffer->lock(HardwareBuffer::HBL_DISCARD);

		for(size_t i = 0; i != _ati.colours.size(); ++i)
		{
			const Vector3& dir = _ati.points[i];
			ColourValue clr = calculateAtmosphereColour(dir, _ati.baseAirColour);
			Root::getSingleton().convertColourValue(clr, cbufferPtr++);
		}

		cBuffer->unlock();
	}
	//--------------------------------------------------------------------------------------
	ColourValue SkyProducer::calculateAtmosphereColour(const Vector3& _directionFromObserver, const ColourValue& _baseColour) const
	{
		ColourValue colourAccum = _baseColour;
		for(BodyInfoMap::iterator itBI = mBodyInfoMap.begin(); 
			itBI != mBodyInfoMap.end(); ++itBI)
		{
			BodyInfo& bi = itBI->second;
			if(bi.visible)
			{
				Radian angle = bi.directionFromObserver.angleBetween(_directionFromObserver);

				ColourValue twoColours[2];
				for(int u = 0; u != 2; ++u]
				{
					const SkyBody::ColourValueAtPointList& list = bi.atmosphereColourAtPointList[u];
					Real ratio;
					std::pair<size_t, size_t> interval;
					interval = findIntervalForAngleBetweenPointAndBody(angle, list, &ratio);
					if(ratio >= 0)
					{
						SkyBody::ColourValueAtPoint& elem0 = list[interval.first];
						SkyBody::ColourValueAtPoint& elem1 = list[interval.second];
						twoColours[u] = elem0.atmosphereColourAdd * ratio + elem1.atmosphereColourAdd * (1-ratio);
					}
					else
					{
						twoColours[u] = ColourValue::Black;
					}
				}

				ColourValue colourAdd = twoColours[0] * bi.atmosphereColourRatio + twoColours[1] * (1-bi.atmosphereColourRatio);
				colourAccum += colourAdd;
			}
		}

		return colourAccum;
	}
	//-----------------------------------------------------------------------
	SkyProducer::AtmosphereInfo::AtmosphereInfo(SkyAtmosphere* _atmosphere)
	{
		skyAtmosphere = _atmosphere;
		entity = nullptr;
		sceneNode = nullptr;
		parentSky = _cloudLayer->getSky();
		parentSkyStateCount = parentSky->getStateCount() - 1;
	}
	//-----------------------------------------------------------------------
	SkyProducer::AtmosphereInfo::~AtmosphereInfo()
	{
		if(entity)
			entity->_getManager()->destroyEntity(entity);

		if(sceneNode)
			sceneNode->getCreator()->destroySceneNode(sceneNode);

		if(mesh)
			MeshManagerEx::getSingleton().remove(mesh);
	}
	
	
	
	
	//-----------------------------------------------------------------------
	// Scene's ambient colour, calculation
	//-----------------------------------------------------------------------
	void SkyProducer::calculateAmbientColour()
	{
		ColourValue ambientColour = ColourValue(0, 0, 0, 0);

		// Iterate through all layer indices.
		for(LayerIndices::iterator itLI = mLayerIndices.begin(); 
			itLI != mLayerIndices.end(); ++itLI)
		{
			int layerIndex = *itLI;

			// Ambient light from celestial bodies
			for(BodyInfoMap::iterator itBI = mBodyInfoMap.begin(); 
				itBI != mBodyInfoMap.end(); ++itBI)
			{
				BodyInfo& bi = *itBI;
				if(bi.layerIndex == layerIndex && bi.visible)
				{
					ambientColour += bi.ambientLightColourAdd;
				}
			}

			// Ambient light absorption due to an atmosphere's presence
			for(AtmosphereInfoMap::iterator itATI = mAtmosphereInfoMap.begin(); 
				itATI != mAtmosphereInfoMap.end(); ++itATI)
			{
				AtmosphereInfo& ati = *itATI;
				if(ati.layerIndex == layerIndex && ati.visible)
				{
					ambientColour = ambientColour * ati.ambientLightColourMul + ati.ambientLightColourAdd;
				}
			}

			// Ambient light absorption due to clouds' presence
			for(CloudLayerInfoMap::iterator itCLI = mCloudLayerInfoMap.begin(); 
				itCLI != mCloudLayerInfoMap.end(); ++itCLI)
			{
				CloudLayerInfo& cli = *itCLI;
				if(cli.layerIndex == layerIndex && cli.visible)
				{
					ambientColour = ambientColour * cli.ambientLightColourMul + cli.ambientLightColourAdd;
				}
			}
		}

		// Write the new ambient colour to the scene manager.
		mSceneManager->setAmbientLight(ambientColour);
	}
	


	//-----------------------------------------------------------------------
	// Sending list of visible skies to SkySystem
	//-----------------------------------------------------------------------
	void SkyProducer::notifySkySystemAboutVisibleSkies()
	{
		SkySystem::getSingleton()._clearVisibleSkyList(_camera);
		for(VisibleSkies::reverse_iterator itVS = mVisibleSkies.rend();
			itVS != mVisibleSkies.rbegin(); ++itVS)
		{
			VisibleSky& vs = *itVS;
			if(vs.hasVisibleLayer)
			{
				SkySystem::getSingleton()._addToVisibleSkyList(mCamera, vs.sky->getName());
			}
		}
	}




	//-----------------------------------------------------------------------
	// SkyProducer - main part
	//-----------------------------------------------------------------------
	void SkyProducer::queueForRendering(Camera* _camera)
	{
		// Find visible skies, updating weathers.
		findVisibleSkyObjs(_camera);
		findVisibleSkies();
		
		// Move the skies' root scene node to the camera's position
		moveSkyRootSceneNode();
		gatherLayerIndices();
		
		// Calculate different sky's aspects and modify the scene
		// (create necessary entities) to display them.
		calculateBodies();
		calculateAtmospheres();
		calculateCloudLayers();
		calculateAmbientColour();
		calculateBackgroundColour();
		calculateFog();

		// SkySystem needs to know which skies are visible now.
		notifySkySystemAboutVisibleSkies();
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::SkyProducer(SceneManagerExImpl *_sm)
	{
		mSceneManager = _sm;
		mSkyRootSceneNode = nullptr;
		mCamera = nullptr;
	}
	//--------------------------------------------------------------------------------------
	SkyProducer::~SkyProducer()
	{
		if(mSkyRootSceneNode)
			mSceneManager->destroySceneNode(mSkyRootSceneNode);
	}
	//-----------------------------------------------------------------------
	void SkyProducer::frameEnded(const FrameEvent& _evt)
	{
		// Remove unused entries from mSkyCloudLayerInfoMap
		{
			CloudLayerInfoMap::iterator it = mCloudLayerInfoMap.begin(); 
			while(it != mCloudLayerInfoMap.end())
			{
				CloudLayerInfoMap::iterator itCur = it++;
				CloudLayerInfo& info = itCur->second;
				if(info.inUse)
				{
					info.inUse = false;
				}
				else
				{
					mCloudLayerInfoMap.erase(itCur);
				}
			}
		}

		// Remove unused entries from mSkyBodyInfoMap
		{
			BodyInfoMap::iterator it = mBodyInfoMap.begin(); 
			while(it != mBodyInfoMap.end())
			{
				BodyInfoMap::iterator itCur = it++;
				BodyInfo& info = itCur->second;
				if(info.inUse)
				{
					info.inUse = false;
				}
				else
				{
					mBodyInfoMap.erase(itCur);
				}
			}
		}

		// Remove unused entries from mSkyAtmosphereInfoMap
		{
			AtmosphereInfoMap::iterator it = mAtmosphereInfoMap.begin(); 
			while(it != mAtmosphereInfoMap.end())
			{
				AtmosphereInfoMap::iterator itCur = it++;
				AtmosphereInfo& info = itCur->second;
				if(info.inUse)
				{
					info.inUse = false;
				}
				else
				{
					mAtmosphereInfoMap.erase(itCur);
				}
			}
		}

		// Remove unused entries from mWeatherInfoMap
		{
			WeatherInfoMap::iterator it = mWeatherInfoMap.begin(); 
			while(it != mWeatherInfoMap.end())
			{
				WeatherInfoMap::iterator itCur = it++;
				WeatherInfo& info = itCur->second;
				if(info.inUse)
				{
					info.inUse = false;
				}
				else
				{
					mWeatherInfoMap.erase(itCur);
				}
			}
		}
	}


} // namespace GothOgre