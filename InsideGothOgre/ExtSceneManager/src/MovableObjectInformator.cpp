#include "Precompiled.h"
#include "MovableObjectInformator.h"
#include "SceneManagerExImpl.h"
#include "RaySceneQueryExImpl.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	MovableObjectInformator::MovableObjectInformator(SceneManagerExImpl *_sceneManager)
		: mSceneManager(_sceneManager)
	{
		registerFrameListener();
	}
	//-----------------------------------------------------------------------
	MovableObjectInformator::~MovableObjectInformator()
	{
	}
	//-----------------------------------------------------------------------
	void MovableObjectInformator::frameStarted(const FrameEvent& _evt)
	{
		ObservedObjects::iterator it = mObservedObjects.begin();
		while(it != mObservedObjects.end())
		{
			ObservedObjects::iterator itCur = it++;
			Info* info = &(itCur->second);
			if(!info->flags && !info->oldFlags)
			{
				// remove unused entry
				mObservedObjects.erase(itCur);
			}
			else
			{
				// reset flags
				info->oldFlags = info->flags;
				info->flags = 0;
			}
		}
	}
	//-----------------------------------------------------------------------
	const Vector3& MovableObjectInformator::getDerivedPosition(MovableObject* _obj)
	{
		// Different procedures to get position for frustums and non-frustums.
		if(_obj->getTypeFlags() & SceneManager::FRUSTUM_TYPE_MASK)
		{
			Frustum* frustum = (Frustum*) _obj;
			const Vector3& position = frustum->getPositionForViewUpdate();
			return position;
		}
		else
		{
			Node* node = _obj->getParentNode();
			const Vector3& position = (node ? node->_getDerivedPosition() : Vector3::ZERO);
			return position;
		}
	}
	//-----------------------------------------------------------------------
	MovableObjectInformator::Info* MovableObjectInformator::getInfo(MovableObject* _object)
	{
		ObservedObjects::iterator it = mObservedObjects.find(_object);
		if(it == mObservedObjects.end())
		{
			it = mObservedObjects.insert(std::make_pair(_object, Info())).first;
		}
		Info* info = &(it->second);
		return info;
	}
	//-----------------------------------------------------------------------
	SceneUnderwaterInfo MovableObjectInformator::getUnderwaterInfo(MovableObject* _obj)
	{
		Info* info = getInfo(_obj);
		if(!(info->flags & FLAG_UNDERWATER_INFO))
		{
			const Vector3& newPosition = getDerivedPosition(_obj);
			const SceneUnderwaterInfo& oldUWInfo = info->underwaterInfo;
			SceneUnderwaterInfo newUWInfo = mSceneManager->getUnderwaterInfo(newPosition);
			
			if(newUWInfo.waterMaterial.isNull() && !oldUWInfo.waterMaterial.isNull())
			{
				// Special case: underwater cave.
				// The position is underwater in fact, however raycasting 
				// in the upward direction meets a non-water material.

				// Search ceiling to determine if this is an underwater cave or not.
				Vector3 upVector = mSceneManager->getUpDirection();
				RaySceneQueryExPtr query = mSceneManager->createRayQueryEx();
				query->setRay( Ray(newPosition, upVector) );
				query->setQueryMask(SceneQueryMask::LEVEL_BSP);
				query->setMaxResults(1);
				RaySceneQueryExResult& qr = query->execute();
				
				if(qr.size() == 1)
				{
					// This is an underwater cave
					// We continue using the latest water material,
					// and trying to guess new immersion depth.
					Real oldDepth = oldUWInfo.immersionDepth;
					const Vector3& oldPosition = oldUWInfo.position;
					Real newDepth = oldDepth + (oldPosition - newPosition).dotProduct(upVector);
					newUWInfo.waterMaterial = oldUWInfo.waterMaterial;
					newUWInfo.immersionDepth = newDepth;
				}
			}

			info->underwaterInfo = newUWInfo;
			info->flags |= FLAG_UNDERWATER_INFO;
		}
		return info->underwaterInfo;
	}
	//-----------------------------------------------------------------------
	SceneSectorInfo MovableObjectInformator::getSectorInfo(MovableObject* _obj)
	{
		Info* info = getInfo(_obj);
		if(!(info->flags & FLAG_SECTOR_INFO))
		{
			const Vector3& position = getDerivedPosition(_obj);
			info->sectorInfo = mSceneManager->getSectorInfo(position);
			info->flags |= FLAG_SECTOR_INFO;
		}
		return info->sectorInfo;
	}
	//-----------------------------------------------------------------------
	SceneFloorInfo MovableObjectInformator::getFloorInfo(MovableObject* _obj)
	{
		Info* info = getInfo(_obj);
		if(!(info->flags & FLAG_FLOOR_INFO))
		{
			const Vector3& position = getDerivedPosition(_obj);
			info->floorInfo = mSceneManager->getFloorInfo(position);
			info->flags |= FLAG_FLOOR_INFO;
		}
		return info->floorInfo;
	}

} // namespace GothOgre