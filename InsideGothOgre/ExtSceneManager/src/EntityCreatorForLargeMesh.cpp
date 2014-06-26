#include "Precompiled.h"
#include "EntityCreatorForLargeMesh.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------------
	// EntityCreatorForLargeMesh::AABoxLimited - implementation
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::AABoxLimited::init(
		const Vector3& _min, const Vector3& _max, Real _sizeLimit)
	{
		mSizeLimit = _sizeLimit;

		mMinimum = _min;
		mMaximum = _max;

		mPossibleMaximum.x = mMinimum.x + mSizeLimit;
		mPossibleMaximum.y = mMinimum.y + mSizeLimit;
		mPossibleMaximum.z = mMinimum.z + mSizeLimit;
		mPossibleMinimum.x = mMaximum.x - mSizeLimit;
		mPossibleMinimum.y = mMaximum.y - mSizeLimit;
		mPossibleMinimum.z = mMaximum.z - mSizeLimit;
	}
	//--------------------------------------------------------------------------------
	bool EntityCreatorForLargeMesh::AABoxLimited::merge(
		const Vector3& _min, const Vector3& _max)
	{
		if(_max.x > mPossibleMaximum.x || _max.y > mPossibleMaximum.y || _max.z > mPossibleMaximum.z)
			return false;
		if(_min.x < mPossibleMinimum.x || _min.y < mPossibleMinimum.y || _min.z < mPossibleMinimum.z)
			return false;

		Vector3 newMinimum, newMaximum;
		
		newMinimum.x = std::min(mMinimum.x, _min.x);
		newMaximum.x = std::max(mMaximum.x, _max.x);
		if(newMaximum.x - newMinimum.x > mSizeLimit)
			return false;

		newMinimum.y = std::min(mMinimum.y, _min.y);
		newMaximum.y = std::max(mMaximum.y, _max.y);
		if(newMaximum.y - newMinimum.y > mSizeLimit)
			return false;

		newMinimum.z = std::min(mMinimum.z, _min.z);
		newMaximum.z = std::max(mMaximum.z, _max.z);
		if(newMaximum.z - newMinimum.z > mSizeLimit)
			return false;

		//if(newMinimum == mMinimum && newMaximum == mMaximum)
		//	return true;

		mMinimum = newMinimum;
		mMaximum = newMaximum;

		mPossibleMaximum.x = mMinimum.x + mSizeLimit;
		mPossibleMaximum.y = mMinimum.y + mSizeLimit;
		mPossibleMaximum.z = mMinimum.z + mSizeLimit;
		mPossibleMinimum.x = mMaximum.x - mSizeLimit;
		mPossibleMinimum.y = mMaximum.y - mSizeLimit;
		mPossibleMinimum.z = mMaximum.z - mSizeLimit;
		return true;
	}





	//--------------------------------------------------------------------------------
	// Creating entity list
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::createEntityList(EntityList& _entList, const MeshExPtr& _mesh, uint32 _queryMask)
	{
		mMaxClusterSize = GraphicSettings::getSingleton().getSetting("BspMaxClusterSize", "Advanced", 20000.f);
		mMaxSubClusterSize = GraphicSettings::getSingleton().getSetting("BspMaxSubClusterSize", "Advanced", 500.f);

		_mesh->load();

		mMesh = _mesh;
		mQueryMask = _queryMask;
		mResultEntityList = &_entList;

		MeshEx::SubMeshIterator it = mMesh->getSubMeshIterator();
		while(it.hasMoreElements())
		{
			mSubMesh = it.getNext();
			separateSubMesh();
			updateStatistics();
		}

		logStatistics();
		clear();
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::separateSubMesh()
	{
		lockBuffers();

		if(!mPositionPtr || !mIndexPtr)
			return;

		mIndexStart = mIndexData->indexStart;
		mIndexCount = mIndexData->indexCount;

		copyFaces();
		createClusters();
		calculateFaceIndices();
		writeNewFaceOrderToIndexBuffer();

		createDestVertexData();
		createDestIndexDatas();
		createDestMeshes();
		createDestEntities();
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::copyFaces()
	{
		mFaces.resize(0);
		mFaces.resize(mIndexCount / 3);

		size_t start = mIndexStart;
		size_t end = start + mIndexCount;

		switch(mIndexType)
		{
			case HardwareIndexBuffer::IT_16BIT:
			{
				size_t i = start; size_t curface = 0;
				while(i != end)
				{
					Face& face = mFaces[curface++];
					face.indexStart = i;
					face.vertexIndices[0] = ((const uint16*) mIndexPtr)[i++];
					face.vertexIndices[1] = ((const uint16*) mIndexPtr)[i++];
					face.vertexIndices[2] = ((const uint16*) mIndexPtr)[i++];
				}
				break;
			}
			case HardwareIndexBuffer::IT_32BIT:
			{
				size_t i = start; size_t curface = 0;
				while(i != end)
				{
					Face& face = mFaces[curface++];
					face.indexStart = i;
					face.vertexIndices[0] = ((const uint32*) mIndexPtr)[i++];
					face.vertexIndices[1] = ((const uint32*) mIndexPtr)[i++];
					face.vertexIndices[2] = ((const uint32*) mIndexPtr)[i++];
				}
				break;
			}
			default:
				GOTHOGRE_EXCEPT("Unsupported index type");
		}
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::createClusters()
	{
		mClusters.clear();
		for(size_t i = 0; i != mFaces.size(); ++i)
		{
			Face& face = mFaces[i];
			updateClustersByFace(face);
		}
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::updateClustersByFace(Face& _face)
	{
		Vector3 coords[3];
		const uint8* ptr0 = mPositionPtr + mPositionStart * mPositionSize + mPositionOffset;

		switch(mPositionType)
		{
			case VET_FLOAT3:
			case VET_FLOAT4:
			{
				for(size_t j = 0; j != 3; ++j)
				{
					const uint8* ptr = ptr0 + _face.vertexIndices[j] * mPositionSize;
					coords[j] = *(const Vector3*) ptr;
				}
				break;
			}
			default:
				GOTHOGRE_EXCEPT("Unsupported vertex type");
		}
		updateClustersByFace2(_face, coords);
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::updateClustersByFace2(Face& _face, const Vector3* _coords)
	{
		// Calculate the face's bounding box
		Vector3 faceABBmin = _coords[0];
		Vector3 faceABBmax = _coords[0];
		faceABBmin.makeFloor(_coords[1]);
		faceABBmin.makeFloor(_coords[2]);
		faceABBmax.makeCeil(_coords[1]);
		faceABBmax.makeCeil(_coords[2]);

		// Search a near cluster
		Cluster* foundCluster = nullptr;
		for(Clusters::iterator it = mClusters.begin(); it != mClusters.end(); ++it)
		{
			Cluster* cluster = &*it;
			if(cluster->bounds.merge(faceABBmin, faceABBmax))
			{
				foundCluster = cluster;
				break;
			}
		}

		if(!foundCluster)
		{
			mClusters.push_back(Cluster());
			Cluster* newCluster = &mClusters.back();
			newCluster->indexCount = 0;
			newCluster->bounds.init(faceABBmin, faceABBmax, mMaxClusterSize);
			foundCluster = newCluster;
		}

		foundCluster->indexCount += 3;

		// Search a near subcluster
		SubCluster* foundSubCluster = nullptr;
		SubClusters& subclusters = foundCluster->subclusters;
		for(SubClusters::iterator it2 = subclusters.begin(); it2 != subclusters.end(); ++it2)
		{
			SubCluster* subcluster = &*it2;
			if(subcluster->bounds.merge(faceABBmin, faceABBmax))
			{
				foundSubCluster = subcluster;
				break;
			}
		}

		if(!foundSubCluster)
		{
			subclusters.push_back(SubCluster());
			SubCluster* newSubCluster = &subclusters.back();
			newSubCluster->bounds.init(faceABBmin, faceABBmax, mMaxSubClusterSize);
			newSubCluster->indexCount = 0;
			foundSubCluster = newSubCluster;
		}

		foundSubCluster->indexCount += 3;
		_face.subcluster = foundSubCluster;
	}
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::calculateFaceIndices()
	{
		size_t currentIndex = mIndexStart;
		for(Clusters::iterator it = mClusters.begin(); it != mClusters.end(); ++it)
		{
			Cluster* cluster = &*it;
			cluster->indexStart = currentIndex;
			SubClusters& subclusters = cluster->subclusters;
			for(SubClusters::iterator it2 = subclusters.begin(); it2 != subclusters.end(); ++it2)
			{
				SubCluster* subcluster = &*it2;
				subcluster->indexStart = currentIndex;
				subcluster->currentIndex = currentIndex;
				currentIndex += subcluster->indexCount;
			}
		}
		
		for(size_t i = 0; i != mFaces.size(); ++i)
		{
			Face& face = mFaces[i];
			SubCluster* subcluster = face.subcluster;
			face.indexStart = subcluster->currentIndex;
			subcluster->currentIndex += 3;
		}
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::writeNewFaceOrderToIndexBuffer()
	{
		switch(mIndexType)
		{
			case HardwareIndexBuffer::IT_16BIT:
			{
				for(size_t i = 0; i != mFaces.size(); ++i)
				{
					const Face& face = mFaces[i];
					((uint16*) mIndexPtr)[face.indexStart + 0] = face.vertexIndices[0];
					((uint16*) mIndexPtr)[face.indexStart + 1] = face.vertexIndices[1];
					((uint16*) mIndexPtr)[face.indexStart + 2] = face.vertexIndices[2];
				}
				break;
			}
			case HardwareIndexBuffer::IT_32BIT:
			{
				for(size_t i = 0; i != mFaces.size(); ++i)
				{
					const Face& face = mFaces[i];
					((uint32*) mIndexPtr)[face.indexStart + 0] = face.vertexIndices[0];
					((uint32*) mIndexPtr)[face.indexStart + 1] = face.vertexIndices[1];
					((uint32*) mIndexPtr)[face.indexStart + 2] = face.vertexIndices[2];
				}
				break;
			}
			default:
				GOTHOGRE_EXCEPT("Unsupported index type");
		}
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::lockBuffers()
	{
		lockPositionBuffer();
		lockIndexBuffer();
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::unlockBuffers()
	{
		unlockPositionBuffer();
		unlockIndexBuffer();
	}
	//--------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::lockPositionBuffer()
	{
		VertexData* vertexData = mSubMesh->useSharedVertices ? mSubMesh->parent->sharedVertexData : mSubMesh->vertexData;
		if(vertexData == mVertexData)
			return; // continue using the same buffer

		if(!vertexData)
		{
			unlockPositionBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mVertexData = vertexData;
		mPositionStart  = mVertexData->vertexStart;

		const VertexElement* positionElement = mVertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);
		if(!positionElement)
		{
			unlockPositionBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mPositionOffset = positionElement->getOffset();
		mPositionType   = positionElement->getType();

		HardwareVertexBufferSharedPtr positionBuffer = mVertexData->vertexBufferBinding->getBuffer(positionElement->getSource());
		if(positionBuffer == mPositionBuffer)
			return; // continue using the same buffer

		unlockPositionBuffer();
		mPositionBuffer = positionBuffer;

		if(mPositionBuffer.isNull())
			return; // error, exiting with mIndexPtr == nullptr

		assert(mPositionPtr == nullptr);
		mPositionPtr = (uint8*) mPositionBuffer->lock( HardwareBuffer::HBL_READ_ONLY );
		mPositionSize = mPositionBuffer->getVertexSize();
	}
	//-----------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::unlockPositionBuffer()
	{
		if(mPositionPtr)
		{
			mPositionBuffer->unlock();
			mPositionPtr = nullptr;
		}
	}
	//-----------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::lockIndexBuffer()
	{
		IndexData* indexData = mSubMesh->indexData;
		if(indexData == mIndexData)
			return; // continue using the same buffer

		if(!indexData)
		{
			unlockIndexBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mIndexData = indexData;
		mIndexStart = mIndexData->indexStart;
		mIndexCount = mIndexData->indexCount;
	
		HardwareIndexBufferSharedPtr indexBuffer = mIndexData->indexBuffer;
		if(indexBuffer == mIndexBuffer)
			return; // continue using the same buffer

		unlockIndexBuffer();

		mIndexBuffer = indexBuffer;
		if(mIndexBuffer.isNull())
			return; // error, exiting with mIndexPtr == nullptr

		assert(mIndexPtr == nullptr);
		mIndexPtr = (uint8*) mIndexBuffer->lock( HardwareBuffer::HBL_READ_ONLY );
		mIndexSize = mIndexBuffer->getIndexSize();
		mIndexType = mIndexBuffer->getType();
	}
	//-----------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::unlockIndexBuffer()
	{
		if(mIndexPtr)
		{
			mIndexBuffer->unlock();
			mIndexPtr = nullptr;
		}
	}
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::createDestVertexData()
	{
		VertexDataByVertexData::iterator it = mClonedVertexDataByOriginal.find(mVertexData);
		if(it == mClonedVertexDataByOriginal.end())
		{
			VertexData* clone = mVertexData->clone(false);
			it = mClonedVertexDataByOriginal.insert(std::make_pair(mVertexData, clone)).first;
		}
		mDestVertexData = it->second;
	}
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::createDestIndexDatas()
	{
		for(Clusters::iterator it = mClusters.begin(); it != mClusters.end(); ++it)
		{
			Cluster* cluster = &*it;
			IndexData* destIndexData = OGRE_NEW IndexData;
			destIndexData->indexBuffer = mIndexBuffer;
			destIndexData->indexStart = cluster->indexStart;
			destIndexData->indexCount = cluster->indexCount;
			cluster->destIndexData = destIndexData;
		}
	}
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::createDestMeshes()
	{
		size_t clusterIndex = 0;
		for(Clusters::iterator it = mClusters.begin(); it != mClusters.end(); ++it, ++clusterIndex)
		{
			Cluster* cluster = &*it;
			const MtlPtr& mtl = mSubMesh->getMtl();
			const MatGenParams& matGenParams = mSubMesh->getMatGenParams();
			
			String destMeshName = mMesh->getName() + ": " + mtl->getName() + 
				": " + StrConv::toString(clusterIndex) + "\x05";
			MeshExPtr destMesh = MeshManagerEx::getSingleton().createManual( destMeshName );
			
			SubMeshEx* destSubMesh = destMesh->createSubMesh();
			destSubMesh->useSharedVertices = true;
			destSubMesh->indexData = cluster->destIndexData;
			destSubMesh->setMtl(mtl);
			destSubMesh->setMatGenParams(matGenParams);

			destMesh->_setBounds(cluster->bounds.getAxisAlignedBox());
			destMesh->sharedVertexData = mDestVertexData;

			cluster->destMesh = destMesh;
		}
	}
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::createDestEntities()
	{
		for(Clusters::iterator it = mClusters.begin(); it != mClusters.end(); ++it)
		{
			Cluster* cluster = &*it;
			Entity* entity = mSceneManager->createEntity(cluster->destMesh, mQueryMask);
			if(entity->getNumSubEntities() > 0)
			{
				SubEntity* subEntity = entity->getSubEntity(0);
				SubSubEntityList& ssEntList = getSubSubEntityList(subEntity);
				SubClusters& subclusters = cluster->subclusters;
				ssEntList.resize(0);
				ssEntList.reserve(subclusters.size());
				for(SubClusters::iterator it2 = subclusters.begin(); it2 != subclusters.end(); ++it2)
				{
					SubCluster* subcluster = &*it2;
					SubSubEntity ssEnt;
					ssEnt.parent = subEntity;
					ssEnt.bounds = subcluster->bounds.getAxisAlignedBox();
					ssEnt.indexStart = subcluster->indexStart;
					ssEnt.indexCount = subcluster->indexCount;
					ssEntList.push_back(ssEnt);
				}
			}
			cluster->destEntity = entity;
			mResultEntityList->push_back(entity);
		}
	}
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::updateStatistics()
	{
		for(Clusters::iterator it = mClusters.begin(); it != mClusters.end(); ++it)
		{
			Cluster* cluster = &*it;
			mTotalClusterSize += cluster->bounds.getSize();
			++mTotalNumClusters;
			for(SubClusters::iterator it2 = cluster->subclusters.begin(); it2 != cluster->subclusters.end(); ++it2)
			{
				SubCluster* subcluster = &*it2;
				mTotalSubClusterSize += subcluster->bounds.getSize();
				++mTotalNumSubClusters;
			}
		}
		mTotalNumFaces += mFaces.size();
	}
	//---------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::logStatistics()
	{
		String prefix = mMesh->getName() + " - EntityCreatorForLargeMesh: ";
		GOTHOGRE_INFO(prefix << "number of clusters: " << mTotalNumClusters);
		GOTHOGRE_INFO(prefix << "number of subclusters: " << mTotalNumSubClusters);

		if(mTotalNumSubClusters != 0)
		{
			Vector3 avgClusterSize = mTotalClusterSize / (Real) mTotalNumClusters;
			Vector3 avgSubClusterSize = mTotalSubClusterSize / (Real) mTotalNumSubClusters;
			Real avgSubClustersPerCluster = (Real) mTotalNumSubClusters / (Real) mTotalNumClusters;
			Real avgFacesPerCluster = (Real) mTotalNumFaces / (Real) mTotalNumClusters;
			Real avgFacesPerSubCluster = (Real) mTotalNumFaces / (Real) mTotalNumSubClusters;
			GOTHOGRE_INFO(prefix << "avg. number of subclusters per cluster: " << avgSubClustersPerCluster);
			GOTHOGRE_INFO(prefix << "avg. number of faces per cluster: " << avgFacesPerCluster);
			GOTHOGRE_INFO(prefix << "avg. number of faces per subcluster: " << avgFacesPerSubCluster);
			GOTHOGRE_INFO(prefix << "avg. size of cluster: " << avgClusterSize);
			GOTHOGRE_INFO(prefix << "avg. size of subcluster: " << avgSubClusterSize);
		}
	}
	//---------------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::clearStatistics()
	{
		mTotalClusterSize = Vector3::ZERO;
		mTotalSubClusterSize = Vector3::ZERO;
		mTotalNumClusters = 0;
		mTotalNumSubClusters = 0;
		mTotalNumFaces = 0;
	}
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::clear()
	{
		unlockBuffers();

		mVertexData = nullptr;
		mPositionBuffer.setNull();
		mIndexData = nullptr;
		mIndexBuffer.setNull();

		mClonedVertexDataByOriginal.clear();
		mDestVertexData = nullptr;

		mClusters.clear();
		mFaces.clear();

		mSubMesh = nullptr;
		mMesh.setNull();

		clearStatistics();
	}



	//---------------------------------------------------------------------------
	// Destroying of entity lists
	//---------------------------------------------------------------------------
	void EntityCreatorForLargeMesh::destroyEntityList(EntityList& _entList)
	{
		// Vertex data can be shared, collect all vertex datas
		set<VertexData*>::type vertexDatas;

		// Destroy all entities, keep vertex datas alive.
		for(EntityList::iterator it = _entList.begin(); 
			it != _entList.end(); ++it)
		{
			Entity* entity = *it;
			const MeshPtr& mesh = entity->getMesh();
			VertexData* vd = mesh->sharedVertexData;
			if(vd)
			{
				vertexDatas.insert(vd);
				mesh->sharedVertexData = nullptr;
			}
			Mesh::SubMeshIterator it2 = mesh->getSubMeshIterator(); 
			while(it2.hasMoreElements())
			{
				SubMesh* subMesh = it2.getNext();
				vd = subMesh->vertexData;
				if(vd)
				{
					vertexDatas.insert(vd);
					subMesh->vertexData = nullptr;
				}
			}
			mSceneManager->destroyEntity(entity);
		}

		// Destroy all vertex datas
		for(set<VertexData*>::type::iterator it3 = vertexDatas.begin();
			it3 != vertexDatas.end(); ++it3)
		{
			VertexData* vd = *it3;
			OGRE_DELETE vd;
		}

		// Clear a list by reference
		_entList.clear();
	}




	//---------------------------------------------------------------------------
	// Retrieving subsubentities from entity
	//---------------------------------------------------------------------------
	SubSubEntityList& EntityCreatorForLargeMesh::getSubSubEntityList(SubEntity* _subEnt)
	{
		UserObjectBindings& uob = _subEnt->getUserObjectBindings();
		static const String ANY_NAME = "SSEntsList";
		const Any& any = uob.getUserAny(ANY_NAME);
		if(!any.isEmpty())
		{
			const SubSubEntityList& lst = *any_cast<SubSubEntityList>(&any);
			return const_cast<SubSubEntityList&>(lst);
		}
		else
		{
			uob.setUserAny(ANY_NAME, (AnyLite) SubSubEntityList());
			const Any& any = uob.getUserAny(ANY_NAME);
			const SubSubEntityList& lst = *any_cast<SubSubEntityList>(&any);
			return const_cast<SubSubEntityList&>(lst);
		}
	}



	//--------------------------------------------------------------------------------
	// Ctor/dtor
	//--------------------------------------------------------------------------------
	EntityCreatorForLargeMesh::EntityCreatorForLargeMesh( SceneManagerExImpl* _sceneManager )
		: mSceneManager( _sceneManager )
	{
		mSubMesh = nullptr;
		mVertexData = nullptr;
		mIndexData = nullptr;
		mPositionPtr = nullptr;
		mIndexPtr = nullptr;
		mDestVertexData = nullptr;
		mResultEntityList = nullptr;

		mTotalClusterSize = Vector3::ZERO;
		mTotalSubClusterSize = Vector3::ZERO;
		mTotalNumClusters = 0;
		mTotalNumSubClusters = 0;
		mTotalNumFaces = 0;
	}
	//--------------------------------------------------------------------------------
	EntityCreatorForLargeMesh::~EntityCreatorForLargeMesh()
	{
		clear();
	}


} // namespace GothOgre


	//---------------------------------------------------------------------------------
/*	void MeshAndBspLoadContext::loadMaterials()
	{
		// Load all materials.
		// (It's not necessary; Ogre can load all materials attached
		// to a mesh automatically when the mesh will be added to scene node.
		// However the loading of materials can be a long-time operation,
		// so it's better to make it with progress bar.)

		// перенести в SceneManagerExImpl::createEntityListForLargeMesh:
		Progress& progress = Progress::getSingleton();
		progress.setPosition(15);

		size_t numMaterials = mSubMeshInfos.size();
		mChildOperation2 = progress.beginChildOperation(15, 100);
		progress.setRange(0, numMaterials);

		for(size_t i = 0; i != numMaterials; ++i)
		{
			MaterialPtr material = mSubMeshInfos[i].material;
			material->load();
			progress.setPosition(i);
		}

		progress.endChildOperation(mChildOperation2);
		mChildOperation2 = nullptr;
		clear();
	}
*/