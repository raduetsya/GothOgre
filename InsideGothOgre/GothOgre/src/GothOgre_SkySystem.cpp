#include "GothOgre_Precompiled.h"
#include "GothOgre_SkySystem.h"

//-------------------------------------------------------------------------
GothOgre::SkySystem* 
	Ogre::Singleton<GothOgre::SkySystem>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// Sky
	//---------------------------------------------------------------------------
	Sky::Sky()
		: MovableObject()
	{
		init();
	}
	//---------------------------------------------------------------------------
	Sky::Sky(const String& _name)
		: MovableObject(_name)
	{
		init();
	}
	//---------------------------------------------------------------------------
	void Sky::init()
	{
		mSkyBoundingBox.setInfinite();
		mSkyBoundingBoxInLocalSpace = false;
		mInnerPart = 0.9f;
		mWeight = 1.0f;
		mPriority = 0;

		setCastShadows(false);
		setQueryFlags(0);
		setVisibilityFlags(0);
	}
	//---------------------------------------------------------------------------
	Sky::~Sky()
	{
	}
	//---------------------------------------------------------------------------
	const String& Sky::getMovableType() const
	{
		return SkySystem::FACTORY_TYPE_NAME;
	}
	//---------------------------------------------------------------------------
	void Sky::setSkyDef(const SkyDefPtr& _skyDef)
	{
		mSkyDef = _skyDef;
	}
	//---------------------------------------------------------------------------
	SkyDefPtr Sky::getSkyDef() const
	{
		return mSkyDef;
	}
	//---------------------------------------------------------------------------
	void Sky::setSkyBoundingBox(const AxisAlignedBox& _box, bool _localSpace)
	{
		mSkyBoundingBox = _box;
		mSkyBoundingBoxInLocalSpace = _localSpace;
	}
	//---------------------------------------------------------------------------
	const AxisAlignedBox& Sky::getSkyBoundingBox() const
	{
		return mSkyBoundingBox;
	}
	//---------------------------------------------------------------------------
	bool Sky::isSkyBoundingBoxInLocalSpace() const
	{
		return mSkyBoundingBoxInLocalSpace;
	}
	//---------------------------------------------------------------------------
	void Sky::setInnerPart(Real _innerPart)
	{
		mInnerPart = _innerPart;
	}
	//---------------------------------------------------------------------------
	Real Sky::getInnerPart() const
	{
		return mInnerPart;
	}
	//---------------------------------------------------------------------------
	void Sky::setWeight(Real _weight)
	{
		mWeight = _weight;
	}
	//---------------------------------------------------------------------------
	Real Sky::getWeight() const
	{
		return mWeight;
	}
	//---------------------------------------------------------------------------
	void Sky::setPriority(int _priority)
	{
		mPriority = _priority;
	}
	//---------------------------------------------------------------------------
	int Sky::getPriority() const
	{
		return mPriority;
	}



	//---------------------------------------------------------------------------
	// SkySystem
	//---------------------------------------------------------------------------
	SkySystem::SkySystem()
	{
	}
	//---------------------------------------------------------------------------
	SkySystem::~SkySystem()
	{
	}
	//---------------------------------------------------------------------------
	void SkySystem::initialise()
	{
		// Register movable object factory
		Root::getSingleton().addMovableObjectFactory(this);

		// Register as listener
		registerFrameListener();
	}
	//---------------------------------------------------------------------------
	void SkySystem::shutdown()
	{
		// Unregister movable object factory
		if(Root::getSingletonPtr())
			Root::getSingleton().removeMovableObjectFactory(this);

		// Clear visibility information
		mVisibleSkiesMap.clear();

		// Unregister as listener
		unregisterFrameListener();
	}
	//---------------------------------------------------------------------------
	const String& SkySystem::getType() const
	{
		return FACTORY_TYPE_NAME;
	}
	//---------------------------------------------------------------------------
	const String SkySystem::FACTORY_TYPE_NAME = "Sky";
	//---------------------------------------------------------------------------
	Sky* SkySystem::createSky(SceneManager* _sceneManager)
	{
		return (Sky*) _sceneManager->createMovableObject(FACTORY_TYPE_NAME);
	}
	//---------------------------------------------------------------------------
	void SkySystem::destroySky(Sky* _sky)
	{
		_sky->_getManager()->destroyMovableObject(_sky);
	}
	//-------------------------------------------------------------------------
	MovableObject* SkySystem::createInstanceImpl(const String& _name, const NameValuePairList* _params)
	{
		return OGRE_NEW Sky(_name);
	}
	//-------------------------------------------------------------------------
	void SkySystem::destroyInstance(MovableObject* _obj)
	{
		OGRE_DELETE _obj;
	}
	//---------------------------------------------------------------------------
	SkySystem::VisibleSkyIterator SkySystem::getVisibleSkyIterator(Camera* _camera) const
	{
		VisibleSkiesMap::const_iterator it = mVisibleSkiesMap.find(_camera);
		if(it == mVisibleSkiesMap.end())
		{
			static VisibleSkyList sEmptyList;
			return (VisibleSkyIterator) sEmptyList;
		}
		const VisibleSkies& vs = it->second;
		return (VisibleSkyIterator) vs.list;
	}
	//---------------------------------------------------------------------------
	bool SkySystem::isVisible(Camera* _camera, const String& _skyName) const
	{
		VisibleSkyIterator it = getVisibleSkyIterator(_camera);
		while(it.hasMoreElements())
		{
			if(it.getNext() == _skyName)
				return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	void SkySystem::_addToVisibleList(Camera* _camera, const String& _skyName)
	{
		VisibleSkiesMap::iterator it = mVisibleSkiesMap.find(_camera);
		if(it == mVisibleSkiesMap.end())
		{
			it = mVisibleSkiesMap.insert(std::make_pair(_camera, VisibleSkies())).first;
		}
		VisibleSkies& vs = it->second;
		vs.usedInCurrentFrame = true;
		vs.list.push_back(_skyName);
	}
	//---------------------------------------------------------------------------
	void SkySystem::frameEnded(const FrameEvent& _evt)
	{
		deleteUnusedInCurrentFrame();
		resetUsedInCurrentFrame();
	}
	//---------------------------------------------------------------------------
	void SkySystem::deleteUnusedInCurrentFrame()
	{
		VisibleSkiesMap::iterator it = mVisibleSkiesMap.begin();
		while(it != mVisibleSkiesMap.end())
		{
			VisibleSkiesMap::iterator itCur = it++;
			VisibleSkies& vs = itCur->second;
			if( !vs.usedInCurrentFrame )
				mVisibleSkiesMap.erase(itCur);
		}
	}
	//---------------------------------------------------------------------------
	void SkySystem::resetUsedInCurrentFrame()
	{
		VisibleSkiesMap::iterator it = mVisibleSkiesMap.begin();
		while(it != mVisibleSkiesMap.end())
		{
			VisibleSkiesMap::iterator itCur = it++;
			VisibleSkies& vs = itCur->second;
			vs.usedInCurrentFrame = false;
		}
	}


} // namespace GothOgre