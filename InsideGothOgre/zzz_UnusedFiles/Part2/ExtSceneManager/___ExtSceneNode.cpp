#include "Precompiled.h"
#include "ExtSceneNode.h"
#include "ExtSceneManager.h"
#include "BspPart.h"
#include "NonBspPart.h"
#include "CameraInfo.h"

#include <intrin.h>
#pragma intrinsic(__rdtsc)

namespace GothOgre
{
	FastSceneNode::FastSceneNode(SceneManager* _creator)
		: SceneNode(_creator)
	{
		mCheckVisibilityCount = 0;
	}
	//----------------------------------------------------------------------------
	FastSceneNode::FastSceneNode(SceneManager* _creator, const String& _name)
		: SceneNode(_creator, _name)
	{
		mCheckVisibilityCount = 0;
	}
	//----------------------------------------------------------------------------
	FastSceneNode::~FastSceneNode()
	{
		_removeFromParts();
	}
	//----------------------------------------------------------------------------
	unsigned __int64 FastSceneNode::sUpdateBoundsTime = 0;
	//----------------------------------------------------------------------------
	void FastSceneNode::_updateBounds()
	{
		unsigned __int64 t = __rdtsc();

		// Reset bounds first
        mWorldAABB.setNull();

        // Update bounds from own attached objects
        ObjectMap::iterator i;
        for (i = mObjectsByName.begin(); i != mObjectsByName.end(); ++i)
        {
			MovableObject* mo = i->second;
            mWorldAABB.merge(mo->getWorldBoundingBox(true));
        }

		//
		if(mWorldAABB.isFinite())
		{
			mWorldAABBCenter = mWorldAABB.getCenter();
			mWorldAABBHalfSize = mWorldAABB.getHalfSize();
		}
		else if(mWorldAABB.isInfinite())
		{
			mWorldAABBCenter = Vector3::ZERO;
			mWorldAABBHalfSize = Vector3(Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);
		}
		else
		{
			mWorldAABBCenter = Vector3(Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);
			mWorldAABBHalfSize = Vector3::ZERO;
		}

		//
		_removeFromParts();

		//
		if(!mWorldAABB.isNull())
			_addToParts();

		t = __rdtsc() - t;
		sUpdateBoundsTime += t;
	}
	//--------------------------------------------------------------------------
	void FastSceneNode::_addToParts()
	{
		bool allInside;
		_addToBspParts(&allInside);
		if(!allInside)
			_addToNonBspPart();
	}
	//--------------------------------------------------------------------------
	void FastSceneNode::_addToBspParts(bool* _allInside)
	{
		ExtSceneManager* sceneManager = static_cast<ExtSceneManager*>(getCreator());
		for(list<BspPart>::type::iterator it = sceneManager->mBspParts.begin();
			it != sceneManager->mBspParts.end(); ++it)
		{
			BspPart* bspPart = &*it;
			bool allInside2;
			bspPart->addSceneNodeIfIntersects(this, &allInside2);
			if(allInside2)
			{
				*_allInside = true;
				return;
			}
		}

		*_allInside = false;
	}
	//--------------------------------------------------------------------------
	void FastSceneNode::_addToNonBspPart()
	{
		ExtSceneManager* sceneManager = static_cast<ExtSceneManager*>(getCreator());
		for(list<NonBspPart>::type::iterator it = sceneManager->mNonBspParts.begin();
			it != sceneManager->mNonBspParts.end(); ++it)
		{
			NonBspPart* nonBspPart = &*it;
			if(!nonBspPart->containsSceneNodes())
			{
				nonBspPart->setCenter(mWorldAABBCenter);
				nonBspPart->setHalfSize(mWorldAABBHalfSize);
				nonBspPart->addSceneNode(this);
				return;
			}

			const Vector3& nonBspPart_center = nonBspPart->getCenter();
			const Vector3& nonBspPart_halfSize = nonBspPart->getHalfSize();
			const Vector3& nonBspPart_maxHalfSize = sceneManager->getNonBspPartMaxHalfSize();
			if(nonBspPart_halfSize.x > nonBspPart_maxHalfSize.x
				|| nonBspPart_halfSize.y > nonBspPart_maxHalfSize.y
				|| nonBspPart_halfSize.z > nonBspPart_maxHalfSize.z)
			{
				continue;
			}

			Vector3 d = mWorldAABBCenter - nonBspPart_center;
			d.x = Math::Abs(d.x);
			d.y = Math::Abs(d.y);
			d.z = Math::Abs(d.z);

			Vector3 new_halfSize = d + mWorldAABBHalfSize;
			if(new_halfSize.x <= nonBspPart_halfSize.x
				&& new_halfSize.y <= nonBspPart_halfSize.y
				&& new_halfSize.z <= nonBspPart_halfSize.z)
			{
				nonBspPart->addSceneNode(this);
				return;
			}
			
			if(new_halfSize.x <= nonBspPart_maxHalfSize.x
				&& new_halfSize.y <= nonBspPart_maxHalfSize.y
				&& new_halfSize.z <= nonBspPart_maxHalfSize.z)
			{
				new_halfSize.x = std::max(new_halfSize.x, nonBspPart_halfSize.x);
				new_halfSize.y = std::max(new_halfSize.y, nonBspPart_halfSize.y);
				new_halfSize.z = std::max(new_halfSize.z, nonBspPart_halfSize.z);

				nonBspPart->setHalfSize(new_halfSize);
				nonBspPart->addSceneNode(this);
				return;
			}
		}

		sceneManager->mNonBspParts.push_front(NonBspPart());
		NonBspPart* nonBspPart = &(sceneManager->mNonBspParts.front());
		nonBspPart->setCenter(mWorldAABBCenter);
		nonBspPart->setHalfSize(mWorldAABBHalfSize);
		nonBspPart->addSceneNode(this);		
	}
	//--------------------------------------------------------------------------
	void FastSceneNode::_removeFromParts()
	{
		_removeFromBspParts();
		_removeFromNonBspPart();
	}
	//--------------------------------------------------------------------------
	void FastSceneNode::_removeFromBspParts()
	{
		BspPart::removeSceneNodeFromAllBspParts(this);
	}
	//--------------------------------------------------------------------------
	void FastSceneNode::_removeFromNonBspPart()
	{
		if(mPositionInNonBspPart.nonBspPart)
		{
			mPositionInNonBspPart.nonBspPart->removeSceneNode(mPositionInNonBspPart.iterator);
			mPositionInNonBspPart.nonBspPart = nullptr;
		}
	}
	//--------------------------------------------------------------------------
	void FastSceneNode::_checkSceneNodeVisibility(const CameraInfo* _cameraInfo, 
			RenderQueue* _renderQueue, size_t _checkVisibilityCount,
			const size_t* _planeIndices)
	{
		if(mCheckVisibilityCount != _checkVisibilityCount)
		{
			Real distance;
			if(_cameraInfo->isVisibleBox(mWorldAABBCenter, mWorldAABBHalfSize, 
				_planeIndices, &distance))
			{
				// Add all entities
				SceneNode::ObjectIterator itMO = getAttachedObjectIterator();
				while(itMO.hasMoreElements())
				{
					MovableObject* mo = itMO.getNext();
					if(mo->getRenderingDistance() > distance)
						mo->_updateRenderQueue(_renderQueue);
				}				
			}
			mCheckVisibilityCount = _checkVisibilityCount;
		}
	}

} // GothOgre