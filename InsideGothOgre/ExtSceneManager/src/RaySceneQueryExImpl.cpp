#include "Precompiled.h"
#include "RaySceneQueryExImpl.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------------
	RaySceneQueryExImpl::RaySceneQueryExImpl(SceneManagerExImpl* _sceneManager)
		: RaySceneQueryEx(_sceneManager)
	{
		mEntity = nullptr;
		mSubEntity = nullptr;
		mSubMesh = nullptr;
	}
	//-----------------------------------------------------------------------------
	RaySceneQueryExImpl::~RaySceneQueryExImpl()
	{
	}
	//-----------------------------------------------------------------------------
	void RaySceneQueryExImpl::executeImpl()
	{
		if(mRay.getDirection() == Vector3::ZERO)
			return;

		OgreProfile("RaySceneQueryEx::execute");

		// Make a transformation matrix to transform the world space
		// to the coordinate system where the ray will have origin (0, 0, 0)
		// and direction (0, 0, -1).
		Vector3 xAxis, yAxis, zAxis;
		zAxis = -mRay.getDirection();
		if(zAxis.positionEquals(Vector3::UNIT_Y))
		{
			xAxis = (Vector3::UNIT_X).crossProduct(zAxis);
			yAxis = zAxis.crossProduct(xAxis);
		}
		else
		{
			xAxis = (Vector3::UNIT_Y).crossProduct(zAxis);
			yAxis = zAxis.crossProduct(xAxis);
		}
		Vector3 org = mRay.getOrigin();
		mRayToWorldTransform = Matrix4(
			xAxis.x, yAxis.x, zAxis.x, org.x,
			xAxis.y, yAxis.y, zAxis.y, org.y,
			xAxis.z, yAxis.z, zAxis.z, org.z,
			      0,       0,       0, 1);
		
		mWorldToRayTransform = mRayToWorldTransform.inverseAffine();

		// The matrix mWorldToRayTransform satisfy the following conditions:
		// 1) mWorldToRayTransform * mRay.getOrigin() == Vector3::ZERO
		// 2) mWorldToRayTransform * (mRay.getOrigin() + mRay->getDirection) == Vector3::NEGATIVE_UNIT_Z

		// Find all intersections.
		SceneManagerExImpl* sm = (SceneManagerExImpl*) mParentSceneMgr;
		SceneNodeBST& bst = sm->_getSceneNodeBST();
		bst.findRayIntersects(mRay, this);

		// Sort by distance.
		std::sort(mResult.begin(), mResult.end());

		// Remove multiple intersections.
		if(!mMultipleIntersections)
			removeMultipleIntersections();

		// Limit number of elements in the result.
		if(mMaxResults != 0 && mMaxResults < mResult.size())
			mResult.resize(mMaxResults);
	}
	//-----------------------------------------------------------------------------
	void RaySceneQueryExImpl::removeMultipleIntersections()
	{
		RaySceneQueryExResult newResult;
		newResult.reserve(mResult.size());
		for(size_t i = 0; i != mResult.size(); ++i)
		{
			MovableObject* movable = mResult[i].movable;
			bool found = false;
			for(size_t j = 0; j != newResult.size(); ++j)
				if(newResult[j].movable == movable)
				{
					found = true;
					break;
				}
			if(!found)
				newResult.push_back(mResult[i]);
		}
		mResult.swap(newResult);
	}
	//-----------------------------------------------------------------------------
	void RaySceneQueryExImpl::enumerateRayIntersects(ElementHandle _handle, Real _distance, void* _param)
	{
		SceneManagerExImpl* sm = (SceneManagerExImpl*) mParentSceneMgr;
		SceneNodeBST& bst = sm->_getSceneNodeBST();
		NewSceneNode* sn = bst.getData(_handle);

		if(!mListener->checkSceneNode(sn))
			return;

		// Ray in local-space has not been calculated yet.
		bool localRayOk = false;
		bool rayTransformOk = false; 
		Matrix4 localToWorld;
		Matrix4 worldToLocal;

		SceneNode::ObjectIterator it = sn->getAttachedObjectIterator();
		while(it.hasMoreElements())
		{
			MovableObject* obj = it.getNext();
			if(obj->getVisible() &&
				(obj->getQueryFlags() & mQueryMask) != 0 &&
				(obj->getMovableType() == EntityFactory::FACTORY_TYPE_NAME))
			{
				if(!mListener->checkMovable(obj))
					continue;

				// Calculate local coordinates of the ray
				const AxisAlignedBox& localBox = obj->getBoundingBox();
				if(!localRayOk)
				{
					localToWorld = sn->_getFullTransform();
					worldToLocal = localToWorld.inverse();
					Vector3 start = mRay.getOrigin();
					Vector3 end = start + mRay.getDirection();
					start = worldToLocal * start;
					end = worldToLocal * end;
					mLocalRay.setOrigin(start);
					mLocalRay.setDirection(end - start);
					localRayOk = true;
				}

				// Fast check the local bounding box.
				if(!mLocalRay.intersects(localBox).first)
					continue;

				// Calculate a matrix to transform from the local space to the ray space.
				if(!rayTransformOk)
				{
					// Make a transformation matrix to transform the local space
					// to the coordinate system where the ray will have origin (0, 0, 0)
					// and direction (0, 0, 1).
					mLocalToRayTransform = mWorldToRayTransform * localToWorld;
					rayTransformOk = true;
				}

				// Slow check each face of the entity.
				mEntity = (Entity*) obj;
				intersectRayEntity();
			}
		}
	}
	//-----------------------------------------------------------------------------
	void RaySceneQueryExImpl::intersectRayEntity()
	{
		mMeshReadHelper.setMesh(mEntity->getMesh());

		for(mSubEntityIndex = 0; mSubEntityIndex != mEntity->getNumSubEntities(); ++mSubEntityIndex)
		{
			mSubEntity = mEntity->getSubEntity(mSubEntityIndex);
			if(!mListener->checkSubEntity(mSubEntity))
				continue;

			mSubMeshIndex = mSubEntityIndex;
			mSubMesh = mSubEntity->getSubMesh();
			mMaterial = mSubEntity->getMaterial();
			if(!mMaterial.isNull())
			{
				inspectMaterial();
				if(!mMaterialTransparent)
					intersectRaySubMesh();
			}
		}
	}
	//-----------------------------------------------------------------------------
	void RaySceneQueryExImpl::inspectMaterial()
	{
		bool foundCullingMode = false;
		mMaterialTransparent = true;

		Technique* technique = mMaterial->getBestTechnique();
		Technique::PassIterator it = technique->getPassIterator();
		while(it.hasMoreElements())
		{
			Pass* pass = it.getNext();
			CompareFunction alphaRejectFunction = pass->getAlphaRejectFunction();
			if(alphaRejectFunction != CMPF_ALWAYS_FAIL)
			{
				mMaterialTransparent = false;
				CullingMode passCullingMode = pass->getCullingMode();
				if(!foundCullingMode)
				{
					mMaterialCullingMode = passCullingMode;
					foundCullingMode = true;
				}
				else if(mMaterialCullingMode != passCullingMode || passCullingMode == CULL_NONE)
				{
					mMaterialCullingMode = CULL_NONE;
				}
			}
		}

		switch(mMaterialCullingMode)
		{
			case CULL_CLOCKWISE:     mClockwiseCoef = 1; break;
			case CULL_ANTICLOCKWISE: mClockwiseCoef = -1; break;
			case CULL_NONE:          mClockwiseCoef = 0; break;
		}
	}
	//-----------------------------------------------------------------------------
	void RaySceneQueryExImpl::intersectRaySubMesh()
	{
		size_t numFaces = mMeshReadHelper.getNumFaces(mSubMeshIndex);
		for(mFaceIndex = 0; mFaceIndex != numFaces; ++mFaceIndex)
		{
			MeshReadHelper::Face face = mMeshReadHelper.getFace(mSubMeshIndex, mFaceIndex);
			mFaceVertices[0] = mMeshReadHelper.getVertex(face.i[0]);
			mFaceVertices[1] = mMeshReadHelper.getVertex(face.i[1]);
			mFaceVertices[2] = mMeshReadHelper.getVertex(face.i[2]);
			if(intersectRayFace())
			{
				addResultEntry();
			}
		}
	}
	//-----------------------------------------------------------------------------
	bool RaySceneQueryExImpl::intersectRayFace()
	{
		// Get the positions of the face's vertices
		const Vector3& A0 = mFaceVertices[0];
		const Vector3& B0 = mFaceVertices[1];
		const Vector3& C0 = mFaceVertices[2];
		
		// Transform the positions to the ray space
		// (i.e. the coordinate system where the ray will have origin (0, 0, 0)
		// and direction (0, 0, -1) ).
		Vector3 A = mLocalToRayTransform * A0;
		Vector3 B = mLocalToRayTransform * B0;

		// Project the face to plane z = 0, i.e. remove the third coordinate.
		// If the ray intersects the face then the projected face contains point (0, 0).
		// Calculation:

		// Check the first edge
		Vector3 AB = B - A;
		Real posRelAB = A.x * AB.y - A.y * AB.x;

		if(posRelAB * mClockwiseCoef < 0)
			return false; // Return if the projected face doesn't contain point (0, 0).

		// Check another two edges
		Vector3 C = mLocalToRayTransform * C0;
		Vector3 BC = C - B;
		Vector3 CA = A - C;
		Real posRelBC = B.x * BC.y - B.y * BC.x;
		if(posRelBC * mClockwiseCoef < 0)
			return false; // Return if the projected face doesn't contain point (0, 0).

		Real posRelCA = C.x * CA.y - C.y * CA.x;
		if(posRelCA * mClockwiseCoef < 0)
			return false; // Return if the projected face doesn't contain point (0, 0).

		if(mClockwiseCoef == 0)
		{
			// Check if posRelAB, posRelBC, and posRelCA have the same sign
			if(posRelAB * posRelBC < 0 || posRelBC * posRelCA < 0)
				return false;
		}

		// Now we need to calculate z-coordinate of the intersection point.

		// Calculate normal without normalize.
		Vector3 normal = AB.crossProduct(BC);
		Real denom = normal.z;
        if (fabs(denom) < std::numeric_limits<Real>::epsilon())
        {
			// Degenerative face
			return false;
        }

		// The intersection point has coordinates (0, 0, z)
		// in the ray coordinate system.
		Real z = normal.dotProduct(A) / denom;
		if (z > 0)
        {
            // Intersection is behind origin
			return false;
        }
		mDistance = -z;

		// Calculate the normalized normal in the world space
		mFaceNormal = normal / normal.length();
		mFaceNormal = mRayToWorldTransform * mFaceNormal;
		if(mClockwiseCoef < 0)
			mFaceNormal *= mClockwiseCoef;

		return true;		
	}
	//-----------------------------------------------------------------------------
	void RaySceneQueryExImpl::addResultEntry()
	{
		RaySceneQueryExResultEntry resultEntry;
		resultEntry.movable        = mEntity;
		resultEntry.subEntityIndex = mSubEntityIndex;
		resultEntry.faceIndex      = mFaceIndex;
		resultEntry.distance       = mDistance;
		resultEntry.material       = mMaterial;
		resultEntry.normal         = mFaceNormal;

		if(mListener->checkResultEntry(resultEntry))
			mResult.push_back(resultEntry);
	}

} // namespace GothOgre
