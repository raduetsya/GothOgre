#include "Precompiled.h"
#include "ExtSceneManager.h"
#include "ExtSceneManagerFactory.h"
#include "ExtSceneManagerFrameStats.h"
#include "CameraInfo.h"
#include "ExtSceneNode.h"
#include "BspPart.h"
#include "NonBspPart.h"

#include <intrin.h>
#pragma intrinsic(__rdtsc)

namespace GothOgre
{
	
	ExtSceneManager::ExtSceneManager(const String& _name)
		: SceneManager(_name)
	{
		mFrameStats = new ExtSceneManagerFrameStats(this);
		mCheckVisibilityCount = 0;
	}
	//---------------------------------------------------------------------------	
	ExtSceneManager::~ExtSceneManager()
	{
		delete mFrameStats;
	}
	//---------------------------------------------------------------------------	
	const String& ExtSceneManager::getTypeName() const
	{
		return ExtSceneManagerFactory::FACTORY_TYPE_NAME;
	}
	//---------------------------------------------------------------------------	
	void ExtSceneManager::_findVisibleObjects(Camera* _camera,  
		VisibleObjectsBoundsInfo* _visibleBounds, bool _onlyShadowCasters)
	{
		RenderQueue* renderQueue = getRenderQueue();
		CameraInfo cameraInfo( _camera, getBspRenderingDistance() );
		++mCheckVisibilityCount;

		for(list<BspPart>::type::iterator it1 = mBspParts.begin();
			it1 != mBspParts.end(); ++it1)
		{
			BspPart* bspPart = &*it1;
			bspPart->findVisibleObjects(&cameraInfo, renderQueue, mCheckVisibilityCount);
		}

		for(list<NonBspPart>::type::iterator it2 = mNonBspParts.begin();
			it2 != mNonBspParts.end(); ++it2)
		{
			NonBspPart* nonBspPart = &*it2;
			nonBspPart->findVisibleObjects(&cameraInfo, renderQueue, mCheckVisibilityCount);
		}
	}
	//-----------------------------------------------------------------------
	SceneNode* ExtSceneManager::createSceneNodeImpl()
	{
		return OGRE_NEW FastSceneNode(this);
	}
	//-----------------------------------------------------------------------
	SceneNode* ExtSceneManager::createSceneNodeImpl(const String& name)
	{
		return OGRE_NEW FastSceneNode(this, name);
	}
	//-----------------------------------------------------------------------
	void ExtSceneManager::bspAdded(const BspPtr& _bsp)
	{
		mBspParts.push_front(BspPart());
		BspPart* bspPart = &mBspParts.front();
		bspPart->setBsp(_bsp);
	}
	//-----------------------------------------------------------------------
	void ExtSceneManager::bspRemoved(const BspPtr& _bsp)
	{
		for(list<BspPart>::type::iterator it2 = mBspParts.begin();
			it2 != mBspParts.end(); ++it2)
		{
			BspPart* bspPart = &*it2;
			if(bspPart->getBsp() == _bsp)
			{
				mBspParts.erase(it2);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------
	void ExtSceneManager::bspRenderingDistanceChanged()
	{
	}
	//-----------------------------------------------------------------------
	void ExtSceneManager::nonBspPartMaxSizeChanged()
	{
		mNonBspPartMaxHalfSize = getNonBspPartMaxSize() / 2;
	}
	//-----------------------------------------------------------------------
	RaySceneQuery* ExtSceneManager::createRayQueryImpl()
	{
		return SceneManager::createRayQueryImpl();
	}
	//-----------------------------------------------------------------------
/*	const String& ExtSceneManager::getSectorByPoint()
	{
		if(!mSectorByPointValid)
		{
			mSectorByPoint.clear();
			for(list<BspPart>::type::iterator it1 = mBspParts.begin();
				it1 != mBspParts.end(); ++it1)
			{
				BspPart* bspPart = &*it1;
				mSectorByPoint = bspPart->findSector(mPoint);
				if(!mSectorByPoint.empty())
					break;
			}
			mSectorByPointValid = true;
		}
		return mSectorByPoint;
	}
*/

	//-----------------------------------------------------------------------
	const String& ExtSceneManager::getSector(MovableObject* _obj)
	{
		return StringUtil::BLANK;
	}
	//-----------------------------------------------------------------------
	bool ExtSceneManager::isUnderwater(MovableObject* _obj)
	{
		return false;
	}
} // namespace GothOgre