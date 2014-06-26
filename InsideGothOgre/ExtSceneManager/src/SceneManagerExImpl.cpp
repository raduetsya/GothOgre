#include "Precompiled.h"
#include "SceneManagerExImpl.h"
#include "SceneManagerExImplFactory.h"
#include "MaterialGeneratorImpl.h"
#include "RaySceneQueryExImpl.h"
#include "PointInformator.h"
#include "MovableObjectInformator.h"
#include "EntityCreator.h"
#include "EntityCreatorForLargeMesh.h"
#include "SkyCalculator.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------	
	// Constructor, destructor
	//---------------------------------------------------------------------------	
	SceneManagerExImpl::SceneManagerExImpl(const String& _name)
		: SceneManagerEx(_name)
	{
		mMaterialGenerator         = new MaterialGeneratorImpl(this);
		mPointInformator           = new PointInformator(this);
		mMovableObjectInformator   = new MovableObjectInformator(this);
		mEntityCreator             = new EntityCreator(this);
		mEntityCreatorForLargeMesh = new EntityCreatorForLargeMesh(this);
		mSkyCalculator             = new SkyCalculator(this);

		mFarClipDistance = 20000.f;

		registerGraphicSettingsListener();
	}
	//---------------------------------------------------------------------------	
	SceneManagerExImpl::~SceneManagerExImpl()
	{
		clearScene();

		mSceneNodeBST.logStatistics("SceneManagerExImpl SceneNodeBST");
		//mLightBST.logStatistics("SceneManagerExImpl LightBST");

		delete mMaterialGenerator;
		delete mPointInformator;
		delete mMovableObjectInformator;
		delete mEntityCreator;
		delete mEntityCreatorForLargeMesh;
		delete mSkyCalculator;
	}
	//---------------------------------------------------------------------------	
	const String& SceneManagerExImpl::getTypeName() const
	{
		return SceneManagerExImplFactory::FACTORY_TYPE_NAME;
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::clearScene()
	{
		SceneManagerEx::clearScene();
		NewSceneNode* root = (NewSceneNode*) getRootSceneNode();
		if(root)
			root->removeFromBST();
	}




	//---------------------------------------------------------------------------	
	// Reading graphics settings from file graphics.cfg
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::graphicSettingsChanged( const ConfigSettingsEvent& _evt )
	{
		// MaxRadiusList for LightBST
		LightBST::RealList defaultLightBSTMaxRadii;
		defaultLightBSTMaxRadii.clear();
		defaultLightBSTMaxRadii.push_back(5000);   // 50m
		defaultLightBSTMaxRadii.push_back(50000);  // 500m
		LightBST::RealList liBSTMaxRadii = GraphicSettings::getSingleton().getSetting("LightBSTMaxRadiusList", "Expert", defaultLightBSTMaxRadii);
		mLightBST.setMaxRadiusList(liBSTMaxRadii);

		// MaxRadiusList for SceneNodeBST
		SceneNodeBST::RealList defaultMaxRadii;
		defaultMaxRadii.push_back(1000);   // 10m
		defaultMaxRadii.push_back(3000);   // 30m
		defaultMaxRadii.push_back(10000);  // 100m
		defaultMaxRadii.push_back(30000);  // 300m
		defaultMaxRadii.push_back(100000); // 1km
		SceneNodeBST::RealList maxRadii = GraphicSettings::getSingleton().getSetting("SceneNodeBSTMaxRadiusList", "Expert", defaultMaxRadii);
		mSceneNodeBST.setMaxRadiusList( maxRadii );

		// DistanceMinRadiusPairList
		SceneNodeBST::RealPairList defaultDMRPList;
		defaultDMRPList.clear();
		defaultDMRPList.push_back(std::make_pair( (Real) 0.0, (Real) 0 ));
		defaultDMRPList.push_back(std::make_pair( (Real) 0.2, (Real) 0 ));
		defaultDMRPList.push_back(std::make_pair( (Real) 0.7, (Real) 500 ));
		defaultDMRPList.push_back(std::make_pair( (Real) 1.0, (Real) 1000 ));
		mDistanceMinRadiusPairList = GraphicSettings::getSingleton().getSetting("DistanceMinRadiusPairList", "Expert", defaultDMRPList);

		Real sightRange = GraphicSettings::getSingleton().getSetting("SightRange", "Basic", 20000.f);
		for(size_t i = 0; i != mDistanceMinRadiusPairList.size(); ++i)
			mDistanceMinRadiusPairList[i].first *= sightRange;

		// Far clipping on hardware
		mNoHardwareFarClipping = GraphicSettings::getSingleton().getSetting("NoHardwareFarClipping", "Expert", false);

		// Polygon mode
		mPolygonMode = GraphicSettings::getSingleton().getSetting("PolygonMode", "", PM_SOLID);
	}



	//---------------------------------------------------------------------------	
	// Main rendering routine
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::_renderScene( Camera* _camera, Viewport* _vp, bool _includeOverlays )
	{
		//OutputDebugString("PreRender started\n");

		// Pre-render
		preRender( _vp );

		//OutputDebugString("PreRender completed\n");

		// Call parent for rendering
		SceneManager::_renderScene(_camera, _vp, _includeOverlays);

		//OutputDebugString("PostRender started\n");

		// Post-render
		postRender( _vp );

		//OutputDebugString("PostRender completed\n");
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::preRender( Viewport* _viewport )
	{
		_viewport->getCamera()->setPolygonMode(mPolygonMode);
		if(mNoHardwareFarClipping)
			_viewport->getCamera()->setFarClipDistance(0);
		else
			_viewport->getCamera()->setFarClipDistance(mFarClipDistance);
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::postRender( Viewport* _viewport )
	{
	}




	//---------------------------------------------------------------------------	
	// Visible objects' searching
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::_setFarClipDistance(Real _farClipDistance)
	{
		mFarClipDistance = _farClipDistance;
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::_findVisibleObjects(Camera* _camera,  
		VisibleObjectsBoundsInfo* _visibleBounds, bool _onlyShadowCasters)
	{
		FindVisibleObjects fvo;
		fvo.renderQueue = getRenderQueue();
		fvo.visibleBounds = _visibleBounds;
		fvo.camera = _camera;
		fvo.onlyShadowCasters = _onlyShadowCasters;

		mSceneNodeBST.findFrustumIntersects(
			SceneNodeBST::FrustumInfo(_camera, 
			mFarClipDistance,
			mDistanceMinRadiusPairList),
			this, &fvo);
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::enumerateFrustumIntersects(SceneNodeBST::ElementHandle _handle, Real _distance, void* _param) 
	{
		NewSceneNode* sn = mSceneNodeBST.getData(_handle);
		RenderQueue* queue = getRenderQueue();
		FindVisibleObjects& fvo = *(FindVisibleObjects*) _param;

		SceneNode::ObjectIterator itMO = sn->getAttachedObjectIterator();
		while(itMO.hasMoreElements())
		{
			MovableObject* mo = itMO.getNext();
			fvo.renderQueue->processVisibleObject(mo, fvo.camera, fvo.onlyShadowCasters, fvo.visibleBounds);
		}	
	}


	
	
	//---------------------------------------------------------------------------	
	// Scene nodes
	//---------------------------------------------------------------------------	
	SceneNode* SceneManagerExImpl::createSceneNodeImpl()
	{
		return OGRE_NEW NewSceneNode(this);
	}
	//---------------------------------------------------------------------------	
	SceneNode* SceneManagerExImpl::createSceneNodeImpl(const String& _name)
	{
		return OGRE_NEW NewSceneNode(this, _name);
	}
	//---------------------------------------------------------------------------	
	SceneNodeBST& SceneManagerExImpl::_getSceneNodeBST()
	{
		return mSceneNodeBST;
	}



	//---------------------------------------------------------------------------	
	// Lights
	//---------------------------------------------------------------------------	
	LightBST& SceneManagerExImpl::_getLightBST()
	{
		return mLightBST;
	}
	//---------------------------------------------------------------------------	
	Light* SceneManagerExImpl::createLight()
	{
		// TODO
		return SceneManagerEx::createLight(); 
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::findLightsAffectingFrustum(const Camera* _camera)
	{
		SceneManagerEx::findLightsAffectingFrustum(_camera);
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::_updateSceneGraph( Camera* _camera) 
	{
		SceneManagerEx::_updateSceneGraph(_camera);
	}




	//-----------------------------------------------------------------------
	// Entities
	//-----------------------------------------------------------------------
	Entity* SceneManagerExImpl::createEntity(const MeshExPtr& _mesh, uint32 _queryMask)
	{
		return mEntityCreator->createEntity( _mesh, _queryMask );
	}
	//-----------------------------------------------------------------------
	void SceneManagerExImpl::destroyEntity(Entity* _entity)
	{
		SceneManager::destroyEntity(_entity);
	}
	//-----------------------------------------------------------------------
	SubSubEntityList& SceneManagerExImpl::getSubSubEntityList(SubEntity* _subEnt)
	{
		return mEntityCreatorForLargeMesh->getSubSubEntityList(_subEnt);
	}
	//-----------------------------------------------------------------------
	SceneManagerEx::EntityList SceneManagerExImpl::createEntityListForLargeMesh(const MeshExPtr& _mesh, uint32 _queryMask)
	{
		SceneManagerEx::EntityList entList;
		mEntityCreatorForLargeMesh->createEntityList(entList, _mesh, _queryMask);
		return entList;
	}
	//-----------------------------------------------------------------------
	void SceneManagerExImpl::destroyEntityList(EntityList& _entList)
	{
		mEntityCreatorForLargeMesh->destroyEntityList(_entList);
	}



	//---------------------------------------------------------------------------	
	// Materials
	//---------------------------------------------------------------------------	
	MaterialPtr SceneManagerExImpl::generateMaterial(const MtlPtr& _mtl, const MatGenParams& _matGenParams)
	{
		return mMaterialGenerator->generateMaterial(_mtl, _matGenParams);
	}
	//---------------------------------------------------------------------------	
	MaterialPtr SceneManagerExImpl::generateMaterial(const MatGenInfo& _matGenInfo)
	{
		return mMaterialGenerator->generateMaterial(_matGenInfo);
	}
	//---------------------------------------------------------------------------	
	SceneManagerEx::MaterialIterator SceneManagerExImpl::getGeneratedMaterialIterator(const MtlPtr& _mtl)
	{
		return mMaterialGenerator->getGeneratedMaterialIterator(_mtl);
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::updateMaterials(const MtlPtr& _mtl)
	{
		mMaterialGenerator->updateMaterials(_mtl);
	}
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::updateMaterial(const MaterialPtr& _material, const MatGenParams& _matGenParams)
	{
		mMaterialGenerator->updateMaterial(_material, _matGenParams);
	}
	//---------------------------------------------------------------------------	
	const MatGenInfo& SceneManagerExImpl::getMatGenInfo(const MaterialPtr& _material)
	{
		return mMaterialGenerator->getMatGenInfo(_material);
	}
	//---------------------------------------------------------------------------	
	MaterialGenerator* SceneManagerExImpl::_getMaterialGenerator()
	{
		return mMaterialGenerator;
	}



	//---------------------------------------------------------------------------	
	// Ray query
	//---------------------------------------------------------------------------	
	RaySceneQueryExPtr SceneManagerExImpl::createRayQueryEx()
	{
		return (RaySceneQueryExPtr) OGRE_NEW RaySceneQueryExImpl(this);
	}
	//---------------------------------------------------------------------------	
	SceneUnderwaterInfo SceneManagerExImpl::getUnderwaterInfo(const Vector3& _point)
	{
		return mPointInformator->getUnderwaterInfo(_point);
	}
	//---------------------------------------------------------------------------	
	SceneUnderwaterInfo SceneManagerExImpl::getUnderwaterInfo(MovableObject* _object)
	{
		return mMovableObjectInformator->getUnderwaterInfo(_object);
	}
	//---------------------------------------------------------------------------	
	SceneSectorInfo SceneManagerExImpl::getSectorInfo(const Vector3& _point)
	{
		return mPointInformator->getSectorInfo(_point);
	}
	//---------------------------------------------------------------------------	
	SceneSectorInfo SceneManagerExImpl::getSectorInfo(MovableObject* _object)
	{
		return mMovableObjectInformator->getSectorInfo(_object);
	}
	//---------------------------------------------------------------------------	
	SceneFloorInfo SceneManagerExImpl::getFloorInfo(const Vector3& _point)
	{
		return mPointInformator->getFloorInfo(_point);
	}
	//---------------------------------------------------------------------------	
	SceneFloorInfo SceneManagerExImpl::getFloorInfo(MovableObject* _object)
	{
		return mMovableObjectInformator->getFloorInfo(_object);
	}





	//---------------------------------------------------------------------------	
	// Sky
	//---------------------------------------------------------------------------	
	void SceneManagerExImpl::_queueSkiesForRendering( Camera* _camera )
	{
		mSkyCalculator->calculate(_camera);
	//	mSkyVisualizer->queueForRendering(mSkyCalculator);
	}




} // namespace GothOgre
