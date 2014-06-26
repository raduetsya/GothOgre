#include "GothOgre_StdHeaders.h"
#include "GothOgre_EntityVisEx.h"


namespace GothOgre
{

	EntityVisEx::EntityVisEx()
		: Ogre::Entity()
	{
		mDistCoef	= 0.0f;
		mFactorCoef	= 0.0f;
	}
	//-----------------------------------------------------------------------
	EntityVisEx::EntityVisEx( const String& _name, const MeshPtr& _mesh)
		: Ogre::Entity(_name, _mesh)
	{
		mDistCoef	= 0.0f;
		mFactorCoef	= 0.0f;
	}
	//-----------------------------------------------------------------------
	void EntityVisEx::setRenderingDistance(Real _dist, Real _distCoef)
	{
		mUpperDistance = _dist;
		mDistCoef = _distCoef;
	}
	//-----------------------------------------------------------------------
	void EntityVisEx::setMeshLodBias(Real _factor, Real _factorCoef, ushort _maxDetailIndex, ushort _minDetailIndex)
	{
        mMeshLodFactorTransformed = mMesh->getLodStrategy()->transformBias(_factor);
        mMaxMeshLodIndex = _maxDetailIndex;
        mMinMeshLodIndex = _minDetailIndex;
		mFactorCoef = _factorCoef;
	}
	//-----------------------------------------------------------------------
	void EntityVisEx::_notifyCurrentCamera(Camera* _camera)
	{
		MovableObject_notifyCurrentCamera(_camera);
		Entity_notifyCurrentCamera(_camera);
	};
	//-----------------------------------------------------------------------
	void EntityVisEx::MovableObject_notifyCurrentCamera(Camera* _camera)
	{
		if (mParentNode)
		{
			if (_camera->getUseRenderingDistance() && mUpperDistance > 0)
			{
				AxisAlignedBox bounds = getWorldBoundingBox();
				Vector3 center = bounds.getCenter();
				Vector3 size = bounds.getSize();
				Real maxext = std::max(size.x, size.y, size.z);
				mUpperDistance = mDist + mDistCoef * maxext;
				mMeshLodFactorTransformed = mFactor + mFactorCoef * maxext;

				Real delta = _camera->getDerivedPosition().distance(center) - maxsize / 2 - mUpperDistance;
				mBeyondFarDistance = (delta > 0);
			}

            // Construct event object
            MovableObjectLodChangedEvent evt;
            evt.movableObject = this;
            evt.camera = cam;

            // Notify lod event listeners
            cam->getSceneManager()->_notifyMovableObjectLodChanged(evt);
		}

        mRenderingDisabled = mListener && !mListener->objectRendering(this, cam);
	}
	//-----------------------------------------------------------------------
	void EntityVisEx::Entity_notifyCurrentCamera(Camera* _camera)
	{
		// !!! This function's body was copy-pasted from Ogre::Entity::_notifyCurrentCamera !!!

        // Calculate the LOD
        if (mParentNode)
        {
            // Get mesh lod strategy
            const LodStrategy *meshStrategy = mMesh->getLodStrategy();
            // Get the appropriate lod value
            Real lodValue = meshStrategy->getValue(this, cam);
            // Bias the lod value
            Real biasedMeshLodValue = lodValue * mMeshLodFactorTransformed;


            // Get the index at this biased depth
            ushort newMeshLodIndex = mMesh->getLodIndex(biasedMeshLodValue);
            // Apply maximum detail restriction (remember lower = higher detail)
            newMeshLodIndex = std::max(mMaxMeshLodIndex, newMeshLodIndex);
            // Apply minimum detail restriction (remember higher = lower detail)
            newMeshLodIndex = std::min(mMinMeshLodIndex, newMeshLodIndex);

            // Construct event object
            EntityMeshLodChangedEvent evt;
            evt.entity = this;
            evt.camera = cam;
            evt.lodValue = biasedMeshLodValue;
            evt.previousLodIndex = mMeshLodIndex;
            evt.newLodIndex = newMeshLodIndex;

            // Notify lod event listeners
            cam->getSceneManager()->_notifyEntityMeshLodChanged(evt);

            // Change lod index
            mMeshLodIndex = evt.newLodIndex;

            // Now do material LOD
            lodValue *= mMaterialLodFactorTransformed;

            SubEntityList::iterator i, iend;
            iend = mSubEntityList.end();
            for (i = mSubEntityList.begin(); i != iend; ++i)
            {
                // Get sub-entity material
                const MaterialPtr& material = (*i)->mpMaterial;
                
                // Get material lod strategy
                const LodStrategy *materialStrategy = material->getLodStrategy();
                
                // Recalculate lod value if strategies do not match
                Real biasedMaterialLodValue;
                if (meshStrategy == materialStrategy)
                    biasedMaterialLodValue = lodValue;
                else
                    biasedMaterialLodValue = materialStrategy->getValue(this, cam) * materialStrategy->transformBias(mMaterialLodFactor);

                // Get the index at this biased depth
                unsigned short idx = material->getLodIndex(biasedMaterialLodValue);
                // Apply maximum detail restriction (remember lower = higher detail)
                idx = std::max(mMaxMaterialLodIndex, idx);
                // Apply minimum detail restriction (remember higher = lower detail)
                idx = std::min(mMinMaterialLodIndex, idx);

                // Construct event object
                EntityMaterialLodChangedEvent evt;
                evt.subEntity = (*i);
                evt.camera = cam;
                evt.lodValue = biasedMaterialLodValue;
                evt.previousLodIndex = (*i)->mMaterialLodIndex;
                evt.newLodIndex = idx;

                // Notify lod event listeners
                cam->getSceneManager()->_notifyEntityMaterialLodChanged(evt);

                // Change lod index
                (*i)->mMaterialLodIndex = evt.newLodIndex;

				// Also invalidate any camera distance cache
				(*i)->_invalidateCameraCache ();
            }


        }
        // Notify any child objects
        ChildObjectList::iterator child_itr = mChildObjectList.begin();
        ChildObjectList::iterator child_itr_end = mChildObjectList.end();
        for( ; child_itr != child_itr_end; child_itr++)
        {
            (*child_itr).second->_notifyCurrentCamera(cam);
        }		
	}
	//-----------------------------------------------------------------------
	String WorldObjectEntityFactory::FACTORY_TYPE_NAME = "EntityVisEx";
	//-----------------------------------------------------------------------
	const String& WorldObjectEntityFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	MovableObject* EntityFactory::createInstanceImpl( const String& name,
		const NameValuePairList* params)
	{
		// must have mesh parameter
		MeshPtr pMesh;
		if (params != 0)
		{
			String groupName = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;

			NameValuePairList::const_iterator ni;

			ni = params->find("resourceGroup");
			if (ni != params->end())
			{
				groupName = ni->second;
			}

			ni = params->find("mesh");
			if (ni != params->end())
			{
				// Get mesh (load if required)
				pMesh = MeshManager::getSingleton().load(
					ni->second,
					// autodetect group location
					groupName );
			}

		}
		if (pMesh.isNull())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"'mesh' parameter required when constructing an Entity.",
				"EntityFactory::createInstance");
		}

		return OGRE_NEW Entity(name, pMesh);

	}
	//-----------------------------------------------------------------------
	void EntityFactory::destroyInstance( MovableObject* obj)
	{
		OGRE_DELETE obj;
	}

} // namespace GothOgre