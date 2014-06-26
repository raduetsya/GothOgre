#include "GothOgre_StdHeaders.h"
#include "GothOgre_BspEntity.h"
#include "GothOgre_MeshManager.h"
#include "GothOgre_GraphicsSettings.h"

namespace GothOgre
{
	BspEntity::BspEntity()
		: Entity()
	{
	}
	//----------------------------------------------------------------------------
	BspEntity::BspEntity(const String& _name, const BspPtr& _bsp)
		: Entity()
	{
		mName = _name;
		mBsp = _bsp;
		_initialise();
	}
	//----------------------------------------------------------------------------
	BspEntity::~BspEntity()
	{
	}
	//----------------------------------------------------------------------------
	void BspEntity::setRenderingDistance(Real _dist)
	{
		Entity::setRenderingDistance(_dist);
		setNeedFindVisibleFaces(true);
	}
	//----------------------------------------------------------------------------
	void BspEntity::_initialise(bool forceReinitialise)
	{
		if (forceReinitialise)
		{
			_deinitialise();
			destroyMesh();
		}

		if (mInitialised)
			return;

		// Initialise mesh and submeshes
		createMesh();

		mBspStateCount = mBsp->getStateCount();

		// Invalidate mLastCameraPosition to search visible faces at the next render.
		setNeedFindVisibleFaces(true);

		// Build subentity list and set mInitialised to true
		Entity::_initialise();
	}
	//----------------------------------------------------------------------------
	void BspEntity::setNeedFindVisibleFaces(bool _needFind)
	{
		mNeedFindVisibleFaces = _needFind;
	}
	//----------------------------------------------------------------------------
	void BspEntity::_updateRenderQueue( RenderQueue* _queue )
	{
		// Do nothing if not initialised yet
		if (!mInitialised)
			return;

		// Check mesh state count, will be incremented if reloaded
		if (mBsp->getStateCount() != mBspStateCount)
		{
			// force reinitialise
			_initialise(true);
		}

		// Find visible faces using BSP tree
		findVisibleFaces();

        // Add each visible SubEntity to the queue
		addSubEntitiesToRenderQueue(_queue);
	}
	//----------------------------------------------------------------------------
	void BspEntity::addSubEntitiesToRenderQueue(RenderQueue* _queue)
	{
		for(size_t i = 0; i != mSubEntityList.size(); ++i)
		{
			SubEntity* subEntity = mSubEntityList[i];
            if(subEntity->isVisible() && mIndexDatas[i] && mIndexDatas[i]->indexCount != 0)
            {
				if (mRenderQueuePrioritySet)
				{
					assert(mRenderQueueIDSet == true);
					_queue->addRenderable(subEntity, mRenderQueueID, mRenderQueuePriority);
				}
                else if(mRenderQueueIDSet)
                {
                    _queue->addRenderable(subEntity, mRenderQueueID);
                }
                else
                {
                    _queue->addRenderable(subEntity);
                }
            }
        }
	}
	//----------------------------------------------------------------------------
	const AxisAlignedBox& BspEntity::getBoundingBox() const
    {
		return mBsp->getRootBspNode()->getBoundingBox();
    }
	//----------------------------------------------------------------------------
	Real BspEntity::getBoundingRadius() const
    {
		return getBoundingBox().getSize().length() / 2;
    }
	//----------------------------------------------------------------------------
	void BspEntity::destroyMesh()
	{
		if(!mMesh.isNull())
		{
			MeshManager::getSingleton().remove(mMesh);
		}
	}
	//----------------------------------------------------------------------------
	void BspEntity::createMesh()
	{
		// Make unique name
		StringStream ss;
		ss << "\x03" << mBsp->getName() << "_" << this;
		String meshName = ss.str();

		// Create and initialise mesh and submeshes
		mMesh = MeshManager::getSingleton().createManual(meshName);
		createSharedVertices();
		createSubMeshes();
	}
	//----------------------------------------------------------------------------
	void BspEntity::createSharedVertices()
	{
		// Create buffer.
		HardwareVertexBufferSharedPtr buffer =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			32 /* 12(pos) + 12(normal) + 8(uv) */, 
			mBsp->getNumVertices(), HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);

		float* bufferPtr = (float*)buffer->lock(HardwareBuffer::HBL_DISCARD);
		
		// Write normals and texture coordinates to the buffer.
		for(ulong i = 0; i != mBsp->getNumVertices(); ++i)
		{
			float* p = &bufferPtr[i * 8];
			const Bsp::Vertex& v = mBsp->getVertex(i);
			p[0] = v.getPosition().x;
			p[1] = v.getPosition().y;
			p[2] = v.getPosition().z;
			p[3] = v.getNormal().x;
			p[4] = v.getNormal().y;
			p[5] = v.getNormal().z;
			p[6] = v.getUVMapping().x;
			p[7] = v.getUVMapping().y;
		}

		buffer->unlock();
	
		// Initialize shared vertex data
		mMesh->sharedVertexData = new VertexData();
		mMesh->sharedVertexData->vertexStart = 0;
		mMesh->sharedVertexData->vertexCount = mBsp->getNumVertices();

		mMesh->sharedVertexData->vertexDeclaration->addElement(0, 0, 
			VET_FLOAT3, VES_POSITION, 0);
		mMesh->sharedVertexData->vertexDeclaration->addElement(1, 12, 
			VET_FLOAT3, VES_NORMAL, 0);
		mMesh->sharedVertexData->vertexDeclaration->addElement(2, 24, 
			VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

		mMesh->sharedVertexData->vertexBufferBinding->setBinding(0, buffer);
		mMesh->sharedVertexData->vertexBufferBinding->setBinding(1, buffer);
		mMesh->sharedVertexData->vertexBufferBinding->setBinding(2, buffer);
	}
	//----------------------------------------------------------------------------
	void BspEntity::createSubMeshes()
	{
		size_t numMaterials = mBsp->getNumMaterials();
		mIndexDatas.resize(numMaterials, nullptr);

		vector<size_t>::type numFacesByMaterial;
		numFacesByMaterial.resize(numMaterials, 0);
		for(size_t i = 0; i != mBsp->getNumFaces(); ++i)
		{
			const Bsp::Face& face = mBsp->getFace(i);
			numFacesByMaterial[face.getMaterialIndex()]++;
		}

		for(size_t j = 0; j != numMaterials; ++j)
		{
			size_t numIndices = numFacesByMaterial[j] * 3;
			if(numIndices != 0)
			{
				const String& materialName = mBsp->getMaterial(j)->getName();
				
				HardwareIndexBufferSharedPtr buffer = 
					HardwareBufferManager::getSingleton().createIndexBuffer(
					HardwareIndexBuffer::IT_32BIT, numIndices, 
					HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false);

				SubMesh* subMesh = mMesh->createSubMesh();
				subMesh->useSharedVertices = true;
				subMesh->setMaterialName(materialName);
				
				IndexData* indexData = subMesh->indexData;
				indexData->indexStart = 0;
				indexData->indexCount = 0;
				indexData->indexBuffer = buffer;
				mIndexDatas[j] = indexData;
			}
		}
	}
	//----------------------------------------------------------------------------
	vector<Plane>::type BspEntity::getFrustumPlanes(Camera* _camera)
	{
		GraphicsSettings& gs = GraphicsSettings::getSingleton();
		Real bspDistance = gs.getSetting("RenderingDistance", "Bsp", 30000.f);

		if(bspDistance == Math::POS_INFINITY)
		{
			if(_camera->getFarClipDistance() != 0)
				bspDistance = _camera->getFarClipDistance();
		}
		
		vector<Plane>::type ret;
		const Plane* planes = _camera->getFrustumPlanes();

		for(size_t i = 0; i != 6; ++i)
		{
			Plane plane = planes[i];
			if(i == FRUSTUM_PLANE_FAR)
			{
				if(bspDistance == Math::POS_INFINITY)
					continue;
				
				Vector3 normal = -_camera->getDirection();
				Vector3 point = _camera->getDerivedPosition() + _camera->getDirection() * bspDistance;
				plane = Plane(normal, point);
			}
			ret.push_back(plane);
		}
		return ret;
	}
	//----------------------------------------------------------------------------
	void BspEntity::findVisibleFaces()
	{
		Camera*    camera       = _getManager()->getCurrentViewport()->getCamera();
		SceneNode* bspSceneNode = getParentSceneNode();

		Vector3    cameraPosition    = camera->getDerivedPosition();
		Quaternion cameraOrientation = camera->getDerivedOrientation();
		Vector3    bspPosition       = bspSceneNode->_getDerivedPosition();
		Quaternion bspOrientation    = bspSceneNode->_getDerivedOrientation();

		if(mNeedFindVisibleFaces 
			|| cameraPosition != mLastCameraPosition
			|| cameraOrientation != mLastCameraOrientation
			|| bspPosition != mLastBspPosition
			|| bspOrientation != mLastBspOrientation)
		{
			VisibleFacesSearch search(mBsp, &mIndexDatas[0], getFrustumPlanes(camera));
			search.run();

			mLastCameraPosition = cameraPosition;
			mLastCameraOrientation = cameraOrientation;
			mLastBspPosition = bspPosition;
			mLastBspOrientation = bspOrientation;
			mNeedFindVisibleFaces = false;
		}
	}
	//----------------------------------------------------------------------------
	BspEntity::VisibleFacesSearch::VisibleFacesSearch(const BspPtr& _bsp, IndexData** _indexDatas, const vector<Plane>::type& _frustumPlanes)
	{
		size_t* planeIndices = mFrustumPlaneIndices;
		for(size_t i = 0; i != _frustumPlanes.size(); ++i)
		{
			const Plane& plane = _frustumPlanes[i];
			const Vector3& n = plane.normal;
			Vector3 an = Vector3(Math::Abs(n.x), Math::Abs(n.y), Math::Abs(n.z));
			mFrustumPlanes[i] = plane;
			mFrustumPlaneAbsNormals[i] = an;
			*planeIndices++ = i;
		}
		*planeIndices = -1; // end marker

		mBsp = _bsp;
		uint32& index0 = _bsp->getFaceIndex(0);
		mFaceIndices = &index0;
		mNumFaceIndices = _bsp->getNumFaceIndices();

		Bsp::Face& face0 = _bsp->getFace(0);
		mFaces = &face0;

		mIndexDatas = _indexDatas;
		mNumFaces = _bsp->getNumFaces();
		mNumMaterials = _bsp->getNumMaterials();
		mFaceVisibilities = new uint8[mNumFaces];
		mIndexBuffers = new uint32*[mNumMaterials];
	}
	//----------------------------------------------------------------------------
	BspEntity::VisibleFacesSearch::~VisibleFacesSearch()
	{
		if(mFaceVisibilities)
			delete[] mFaceVisibilities;

		if(mIndexBuffers)
			delete[] mIndexBuffers;
	}
	//----------------------------------------------------------------------------
	void BspEntity::VisibleFacesSearch::run()
	{
		lockBuffers();
		enumVisibleFaces(mBsp->getOptimizedRootNode(), mFrustumPlaneIndices);
		unlockBuffers();			
	}
	//----------------------------------------------------------------------------
	void BspEntity::VisibleFacesSearch::lockBuffers()
	{
		// reset visibility flags
		memset(mFaceVisibilities, 0, mNumFaces);

		// lock index buffers
		for(size_t j = 0; j != mNumMaterials; ++j)
		{
			IndexData* indexData = mIndexDatas[j];
			if(indexData)
			{
				mIndexBuffers[j] = (uint32*) indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD);
				indexData->indexCount = 0;
			}
		}
	}
	//----------------------------------------------------------------------------
	void BspEntity::VisibleFacesSearch::unlockBuffers()
	{
		// unlock index buffers
		size_t totalNumIndices = 0;
		for(size_t j = 0; j != mNumMaterials; ++j)
		{
			IndexData* indexData = mIndexDatas[j];
			if(indexData)
			{
				indexData->indexBuffer->unlock();
				mIndexBuffers[j] = nullptr;
				totalNumIndices += indexData->indexCount;
			}
		}
		
		mNumVisibleFaces = totalNumIndices / 3;
	}
	//----------------------------------------------------------------------------
	bool BspEntity::VisibleFacesSearch::isVisibleBox(const Vector3& _boxCenter, const Vector3& _boxHalfSize, const size_t* _planeIndices, size_t* _planeIndices2) const
	{
		// For each plane, see if all points are on the negative side
		// If so, object is not visible
		size_t planeIndex;
		while((planeIndex = *_planeIndices++) != -1)
		{
			// Calculate the distance between box centre and the plane
			const Plane& plane = mFrustumPlanes[planeIndex];
			Real dist = plane.getDistance(_boxCenter);

			// Calculate the maximise allows absolute distance for
			// the distance between box centre and plane
			const Vector3& an = mFrustumPlaneAbsNormals[planeIndex];
			Real maxAbsDist = an.dotProduct(_boxHalfSize);

			if (dist < -maxAbsDist) // NEGATIVE_SIDE
				return false;

			if (dist < +maxAbsDist) // BOTH_SIDE
			{
				*_planeIndices2++ = planeIndex;
			}
		}
		*_planeIndices2 = -1;
		return true;
	}
	//----------------------------------------------------------------------------
	void BspEntity::VisibleFacesSearch::enumVisibleFaces(Bsp::BspNode* _bspNode, const size_t* _planeIndices)
	{
		size_t planeIndices2[NUM_FRUSTUM_PLANES + 1];
		if(isVisibleBox(_bspNode->getBoundingBoxCenter(), _bspNode->getBoundingBoxHalfSize(), _planeIndices, planeIndices2))
		{
			if(_bspNode->getNumFaceIndices() != 0)
				addFaces(_bspNode->getFirstFaceIndex(), _bspNode->getNumFaceIndices());
			
			Bsp::BspNode* back = _bspNode->getOptimizedBack();
			if(back)
				enumVisibleFaces(back, planeIndices2);

			Bsp::BspNode* front = _bspNode->getOptimizedFront();
			if(front)
				enumVisibleFaces(front, planeIndices2);
		}
	}
	//----------------------------------------------------------------------------
	void BspEntity::VisibleFacesSearch::addFaces(size_t _firstFaceIndex, size_t _numFaceIndices)
	{
		for(size_t i = _firstFaceIndex; i != _firstFaceIndex + _numFaceIndices; ++i)
		{
			size_t faceIndex = mFaceIndices[i];
			if(!mFaceVisibilities[faceIndex])
			{
				mFaceVisibilities[faceIndex] = 1;
				const Bsp::Face& face = mFaces[faceIndex];
				size_t materialIndex = face.getMaterialIndex();
				IndexData* indexData = mIndexDatas[materialIndex];
				uint32* indexBuffer = mIndexBuffers[materialIndex] + indexData->indexCount;
				*indexBuffer = face.getVertexIndex(0);
				*++indexBuffer = face.getVertexIndex(1);
				*++indexBuffer = face.getVertexIndex(2);
				indexData->indexCount += 3;
			}
		}
	}

} // namespace GothOgre
