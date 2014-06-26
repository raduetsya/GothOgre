#include "GothOgre_Precompiled.h"
#include "GothOgre_PortalSystem.h"
#include "GothOgre_SceneManagerEx.h"
#include "GothOgre_PortalCreatorByLevelEntity.h"
#include "GothOgre_MtlNameUtil.h"

//-------------------------------------------------------------------------
GothOgre::PortalSystem* 
	Ogre::Singleton<GothOgre::PortalSystem>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// Portal
	//---------------------------------------------------------------------------
	Portal::Portal()
		: MovableObject()
	{
		init();
	}
	//---------------------------------------------------------------------------
	Portal::Portal(const String& _name)
		: MovableObject(_name)
	{
		init();
	}
	//---------------------------------------------------------------------------
	void Portal::init()
	{
		mArea = 0;
		mNeedUpdateDerived = false;
	}
	//---------------------------------------------------------------------------
	Portal::~Portal()
	{
		PortalSystem::getSingleton()._notifyPortalDestroyed(this);
	}
	//---------------------------------------------------------------------------
	const String& Portal::getMovableType() const
	{
		return PortalSystem::FACTORY_TYPE_NAME;
	}
	//---------------------------------------------------------------------------
	SceneManagerEx* Portal::_getManager() const 
	{
		return (SceneManagerEx*) MovableObject::_getManager();
	}
	//---------------------------------------------------------------------------
	void Portal::setMaterial(const MaterialPtr& _material)
	{
		if(mMaterial != _material)
		{
			MaterialPtr oldMaterial = mMaterial;
			String oldFirstSector = mFirstSector;
			String oldSecondSector = mSecondSector;
			
			mMaterial = _material;
			MtlPtr mtl = _getManager()->getMatGenInfo(_material).mtl;

			const String& mtlName = mtl->getName();
			if(!MtlNameUtil::isPortalName(mtlName))
				GOTHOGRE_EXCEPT("Portal's material doesn't match the naming convencion (P:_).");

			std::pair<String, String> sectors = MtlNameUtil::splitPortalName(mtlName);
			mFirstSector = sectors.first;
			mSecondSector = sectors.second;
	
			PortalSystem::getSingleton()._notifySectorsChanged(this, oldFirstSector, oldSecondSector);
			PortalSystem::getSingleton()._notifyMaterialChanged(this, oldMaterial);
		}
	}
	//---------------------------------------------------------------------------
	void Portal::setCenter(const Vector3& _center)
	{
		mCenter = _center;
		mNeedUpdateDerived = true;
	}
	//---------------------------------------------------------------------------
	void Portal::setNormal(const Vector3& _normal)
	{
		mNormal = _normal;
		mNeedUpdateDerived = true;
	}
	//---------------------------------------------------------------------------
	void Portal::setArea(Real _area)
	{
		mArea = _area;
	}
	//---------------------------------------------------------------------------
	const Vector3& Portal::_getDerivedCenter() const
	{
		if(mNeedUpdateDerived)
		{
			const_cast<Portal*>(this)->updateDerived();
		}
		return mDerivedCenter;
	}
	//---------------------------------------------------------------------------
	const Vector3& Portal::_getDerivedNormal() const
	{
		if(mNeedUpdateDerived)
		{
			const_cast<Portal*>(this)->updateDerived();
		}
		return mDerivedNormal;
	}
	//---------------------------------------------------------------------------
	void Portal::setVertices(const vector<Vector3>::type& _vertices)
	{
		mVertices = _vertices;
	}
	//---------------------------------------------------------------------------
	void Portal::_notifyMoved()
	{
		MovableObject::_notifyMoved();
		mNeedUpdateDerived = true;
	}
	//---------------------------------------------------------------------------
	void Portal::_notifyAttached(Node* _parent, bool _isTagPoint)
	{
		MovableObject::_notifyAttached(_parent, _isTagPoint);
		mNeedUpdateDerived = true;
	}
	//---------------------------------------------------------------------------
	void Portal::updateDerived()
	{
		SceneNode* sn = getParentSceneNode();
		if(sn)
		{
			mDerivedCenter = sn->convertWorldToLocalPosition(mCenter);
			mDerivedNormal = sn->convertWorldToLocalPosition(mCenter + mNormal) - mDerivedCenter;
		}
		mNeedUpdateDerived = false;
	}




	//---------------------------------------------------------------------------
	// PortalSystem
	//---------------------------------------------------------------------------
	PortalSystem::PortalSystem()
	{
	}
	//---------------------------------------------------------------------------
	PortalSystem::~PortalSystem()
	{
	}
	//---------------------------------------------------------------------------
	void PortalSystem::initialise()
	{
		Root::getSingleton().addMovableObjectFactory(this);
		registerGraphicSettingsListener();
	}
	//---------------------------------------------------------------------------
	void PortalSystem::shutdown()
	{
		Root::getSingleton().removeMovableObjectFactory(this);
		unregisterGraphicSettingsListener();
	}
	//---------------------------------------------------------------------------
	const String PortalSystem::FACTORY_TYPE_NAME = "Portal";
	//---------------------------------------------------------------------------
	const String& PortalSystem::getType() const
	{
		return FACTORY_TYPE_NAME;
	}
	//---------------------------------------------------------------------------
	Portal* PortalSystem::createPortal(SceneManagerEx* _sm)
	{
		return (Portal*) _sm->createMovableObject(FACTORY_TYPE_NAME);
	}
	//---------------------------------------------------------------------------
	void PortalSystem::destroyPortal(Portal* _portal)
	{
		if(_portal)
			_portal->_getManager()->destroyMovableObject(_portal);
	}
	//---------------------------------------------------------------------------
	MovableObject* PortalSystem::createInstanceImpl(const String& _name, const NameValuePairList* _params)
	{
		return OGRE_NEW Portal(_name);
	}
	//---------------------------------------------------------------------------
	void PortalSystem::destroyInstance(MovableObject* _obj)
	{
		OGRE_DELETE _obj;
	}
	//---------------------------------------------------------------------------
	PortalSystem::PortalList PortalSystem::createPortalsByLevelEntity(Entity* _levelEntity)
	{
		PortalCreatorByLevelEntity creator;
		PortalList portals = creator.execute(_levelEntity);
		return portals;
	}
	//---------------------------------------------------------------------------
	void PortalSystem::destroyPortals(PortalList& _portalList)
	{
		for(PortalList::iterator it = _portalList.begin();
			it != _portalList.end(); ++it)
		{
			destroyPortal(*it);
		}
		_portalList.clear();
	}
	//---------------------------------------------------------------------------
	PortalSystem::PortalIterator PortalSystem::getAdjacentPortalIterator(const String& _sector, SceneManagerEx* _sm) const
	{
		ScenePortalsBySM::const_iterator it = mScenePortalsBySM.find(_sm);
		if(it == mScenePortalsBySM.end())
		{
			static const PortalList emptyList;
			return (PortalIterator) emptyList;
		}

		const ScenePortals& scenePortals = it->second;
		const AdjacentPortals& adjacentPortals = scenePortals.adjacentPortals;

		AdjacentPortals::const_iterator itA = adjacentPortals.find(_sector);
		if(itA == adjacentPortals.end())
		{
			static const PortalList emptyList;
			return (PortalIterator) emptyList;
		}
		return (PortalIterator) itA->second;
	}
	//---------------------------------------------------------------------------
	void PortalSystem::_notifySectorsChanged(Portal* _portal, const String& _oldFirstSector, const String& _oldSecondSector)
	{
		const String& newFirstSector = _portal->getFirstSector();
		const String& newSecondSector = _portal->getSecondSector();

		// Get a reference to map of the adjacent portals
		SceneManagerEx* sm = _portal->_getManager();
		ScenePortalsBySM::iterator it = mScenePortalsBySM.find(sm);
		if(it == mScenePortalsBySM.end())
		{
			it = mScenePortalsBySM.insert(std::make_pair(sm, ScenePortals())).first;
		}
		ScenePortals& scenePortals = it->second;
		AdjacentPortals& adjacentPortals = scenePortals.adjacentPortals;
	
		// Remove the portal from the old list
		AdjacentPortals::iterator itA = adjacentPortals.find(_oldFirstSector);
		if(itA != adjacentPortals.end())
		{
			PortalList& portalList = itA->second;
			PortalList::iterator itP = std::find(portalList.begin(), portalList.end(), _portal);
			if(itP != portalList.end())
				portalList.erase(itP);
		}

		// Insert the portal to the new list
		itA = adjacentPortals.find(newFirstSector);
		if(itA == adjacentPortals.end())
		{
			itA = adjacentPortals.insert(std::make_pair(newFirstSector, PortalList())).first;
		}
		PortalList& portalList = itA->second;
		portalList.push_back(_portal);
	}
	//---------------------------------------------------------------------------
	void PortalSystem::_notifyPortalDestroyed(Portal* _portal)
	{
		// Portal destroyed, so it's necessary to remove reference to it
		// from the adjacent list
		SceneManagerEx* sm = _portal->_getManager();
		ScenePortalsBySM::iterator it = mScenePortalsBySM.find(sm);
		if(it != mScenePortalsBySM.end())
		{
			ScenePortals& scenePortals = it->second;
			AdjacentPortals& adjacentPortals = scenePortals.adjacentPortals;

			AdjacentPortals::iterator itA = adjacentPortals.find(_portal->getFirstSector());
			if(itA != adjacentPortals.end())
			{
				PortalList& portalList = itA->second;
				PortalList::iterator itP = std::find(portalList.begin(), portalList.end(), _portal);
				if(itP != portalList.end())
					portalList.erase(itP);

				if(portalList.empty())
					adjacentPortals.erase(itA);
			}

			if(adjacentPortals.empty())
				mScenePortalsBySM.erase(it);
		}
	}
	//---------------------------------------------------------------------------
	void PortalSystem::_notifyMaterialChanged(Portal* _portal, const MaterialPtr& _oldMaterial)
	{
		MaterialPtr material = _portal->getMaterial();
		SceneManagerEx* sm = _portal->_getManager();
		updateMaterialAlpha(material, sm);
	}
	//---------------------------------------------------------------------------
	void PortalSystem::setAlpha(Real _alpha)
	{
		GraphicSettings::getSingleton().setSetting("Alpha", "Portals", _alpha);
	}
	//---------------------------------------------------------------------------
	Real PortalSystem::getAlpha() const 
	{
		return mAlpha;
	}
	//---------------------------------------------------------------------------
	void PortalSystem::graphicSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		Real alpha = GraphicSettings::getSingleton().getSetting<Real>("Alpha", "Portals", 0.5f);
		if(alpha != mAlpha)
		{
			mAlpha = alpha;

			// Modify materials of totally all the portals
			ScenePortalsBySM::iterator it;
			for(it = mScenePortalsBySM.begin();
				it != mScenePortalsBySM.end(); ++it)
			{
				SceneManagerEx* sm = it->first;
				ScenePortals& scenePortals = it->second;
				AdjacentPortals& adjacentPortals = scenePortals.adjacentPortals;
				for(AdjacentPortals::iterator itA = adjacentPortals.begin();
					itA != adjacentPortals.end(); ++itA)
				{
					PortalList& portalList = itA->second;
					for(PortalList::iterator itP = portalList.begin();
						itP != portalList.end(); ++itP)
					{
						Portal* portal = *itP;
						updateMaterialAlpha(portal->getMaterial(), sm);
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	void PortalSystem::updateMaterialAlpha(const MaterialPtr& _material, SceneManagerEx* _sm)
	{
		MatGenParams matGenParams = _sm->getMatGenInfo(_material).matGenParams;
		matGenParams.setColourFactor( ColourValue(1, 1, 1, mAlpha) );
		_sm->updateMaterial(_material, matGenParams);
	}


} // namespace GothOgre






