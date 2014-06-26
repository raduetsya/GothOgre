#include "Precompiled.h"
#include "ZenBspVisualCreator.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// Creating of clusters
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createClusters()
	{
		mMaxClusterSize = GraphicSettings::getSingleton().getSetting("BspClusterMaxSize", "Advanced", 10000.f);
		mMaxSubClusterSize = GraphicSettings::getSingleton().getSetting("BspSubClusterMaxSize", "Advanced", 100.f);

		mClusterListsByMatIndex.clear();
		mClusterListsByMatIndex.resize(mBsp->getNumMaterials());
		mSubClustersByFaceIndex.assign(mBsp->getNumFaces(), nullptr);
		
		createClustersForLeaves(mBsp->getRootNode());
		calculateNewFaceIndices();
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createClustersForLeaves(Bsp::NodeBase* _nodebase)
	{
		if(_nodebase)
		{
			if(_nodebase->isNode())
			{
				Bsp::Node* node = (Bsp::Node*) _nodebase;
				createClustersForLeaves(node->getFront());
				createClustersForLeaves(node->getBack());
			}
			else
			{
				Bsp::Leaf* leaf = (Bsp::Leaf*) _nodebase;
				updateClustersByLeaf(leaf);
			}
		}
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::updateClustersByLeaf(Bsp::Leaf* _leaf)
	{
		size_t start = _leaf->getFirstFaceIndex();
		size_t end = start + _leaf->getNumFaceIndices();
		for(size_t i = start; i != end; ++i)
		{
			size_t faceIndex = mBsp->getFaceIndex(i);
			updateClustersByFace(faceIndex);
		}
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::updateClustersByFace(size_t _faceIndex)
	{
		if(mSubClustersByFaceIndex[_faceIndex])
			return;

		// Get face's coordinates
		const Bsp::Face& face = mBsp->getFace(_faceIndex);
		size_t matIndex = face.getMaterialIndex();
		if(matIndex == -1)
			return;

		const Vector3& pos0 = mBsp->getVertex(face.getVertexIndex(0)).getPosition();
		const Vector3& pos1 = mBsp->getVertex(face.getVertexIndex(1)).getPosition();
		const Vector3& pos2 = mBsp->getVertex(face.getVertexIndex(2)).getPosition();

		// Search a near cluster
		Cluster* foundCluster = nullptr;
		ClusterList& clusters = mClusterListsByMatIndex[matIndex];
		for(ClusterList::iterator it = clusters.begin(); it != clusters.end(); ++it)
		{
			Cluster* cluster = &*it;
			
			AxisAlignedBox newPossibleBounds = cluster->bounds;
			newPossibleBounds.merge(pos0);
			newPossibleBounds.merge(pos1);
			newPossibleBounds.merge(pos2);
			
			if(newPossibleBounds == cluster->bounds)
			{
				foundCluster = cluster;
				break;
			}
			else
			{
				Vector3 size3 = newPossibleBounds.getSize();
				if(size3.x <= mMaxClusterSize && size3.y <= mMaxClusterSize && size3.z <= mMaxClusterSize)
				{
					cluster->bounds = newPossibleBounds;
					foundCluster = cluster;
					break;
				}
			}
		}

		if(!foundCluster)
		{
			clusters.push_back(Cluster());
			Cluster* newCluster = &clusters.back();
			newCluster->numNewFaceIndices = 0;
			newCluster->bounds.setNull();
			newCluster->bounds.merge(pos0);
			newCluster->bounds.merge(pos1);
			newCluster->bounds.merge(pos2);
			foundCluster = newCluster;
		}

		++foundCluster->numNewFaceIndices;

		// Search a near subcluster
		SubCluster* foundSubCluster = nullptr;
		SubClusterList& subClusters = foundCluster->subClusterList;
		for(SubClusterList::iterator it2 = subClusters.begin(); it2 != subClusters.end(); ++it2)
		{
			SubCluster* subCluster = &*it2;
			
			AxisAlignedBox newPossibleBounds = subCluster->bounds;
			newPossibleBounds.merge(pos0);
			newPossibleBounds.merge(pos1);
			newPossibleBounds.merge(pos2);
			
			if(newPossibleBounds == subCluster->bounds)
			{
				foundSubCluster = subCluster;
				break;
			}
			else
			{
				Vector3 size3 = newPossibleBounds.getSize();
				if(size3.x <= mMaxSubClusterSize && size3.y <= mMaxSubClusterSize && size3.z <= mMaxSubClusterSize)
				{
					subCluster->bounds = newPossibleBounds;
					foundSubCluster = subCluster;
					break;
				}
			}
		}

		if(!foundSubCluster)
		{
			subClusters.push_back(SubCluster());
			SubCluster* newSubCluster = &subClusters.back();
			newSubCluster->bounds.setNull();
			newSubCluster->bounds.merge(pos0);
			newSubCluster->bounds.merge(pos1);
			newSubCluster->bounds.merge(pos2);
			newSubCluster->numNewFaceIndices = 0;
			foundSubCluster = newSubCluster;
		}

		++foundSubCluster->numNewFaceIndices;
		mSubClustersByFaceIndex[_faceIndex] = foundSubCluster;
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::calculateNewFaceIndices()
	{
		size_t newFaceIndex = 0;
		for(size_t matIndex = 0; matIndex != mClusterListsByMatIndex.size(); ++matIndex)
		{
			ClusterList& clusters = mClusterListsByMatIndex[matIndex];
			for(ClusterList::iterator it = clusters.begin(); it != clusters.end(); ++it)
			{
				Cluster* cluster = &*it;
				cluster->firstNewFaceIndex = newFaceIndex;
				SubClusterList& subClusters = cluster->subClusterList;
				for(SubClusterList::iterator it2 = subClusters.begin(); it2 != subClusters.end(); ++it2)
				{
					SubCluster* subCluster = &*it2;
					subCluster->firstNewFaceIndex = newFaceIndex;
					subCluster->currentNewFaceIndex = newFaceIndex;
					newFaceIndex = subCluster->numNewFaceIndices;
				}
			}
		}
		
		mNewFaceIndices.clear();
		mNewFaceIndices.resize(mBsp->getNumFaces(), -1);
		for(size_t i = 0; i != mSubClustersByFaceIndex.size(); ++i)
		{
			SubCluster* subCluster = mSubClustersByFaceIndex[i];
			mNewFaceIndices[subCluster->currentNewFaceIndex++] = i;
		}
	}



	//---------------------------------------------------------------------------
	// Creating of meshes
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createMeshes()
	{
		createSharedVertexData();
		createSharedIndexBuffer();
		createMeshesForClusters();
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createSharedVertexData()
	{
		mSharedVertexData = nullptr;

		// Prepare a buffer with positions, normals, and texture coordinates.
		size_t numVertices = mBsp->getNumVertices();
		mSharedVertexBuffer = HardwareBufferManager::getSingleton().createVertexBuffer(
			32 /* 12(pos) + 12(normal) + 8(uv) */, numVertices, 
			HardwareBuffer::HBU_STATIC, false);

		float* bufferPtr = (float*) mSharedVertexBuffer->lock(HardwareBuffer::HBL_DISCARD);
		for(size_t i = 0; i != numVertices; ++i)
		{
			const Bsp::Vertex& vertex = mBsp->getVertex(i);
			const Vector3& position = vertex.getPosition();
			const Vector3& normal = vertex.getNormal();
			const Vector2& uv = vertex.getUVMapping();
			*bufferPtr++ = position.x;
			*bufferPtr++ = position.y;
			*bufferPtr++ = position.z;
			*bufferPtr++ = normal.x;
			*bufferPtr++ = normal.y;
			*bufferPtr++ = normal.z;
			*bufferPtr++ = uv.x;
			*bufferPtr++ = uv.y;
		}
		mSharedVertexBuffer->unlock();

		// Initialise shared vertex data.
		mSharedVertexData = new VertexData();
		mSharedVertexData->vertexStart = 0;
		mSharedVertexData->vertexCount = numVertices;

		mSharedVertexData->vertexDeclaration->addElement(0, 0, 
			VET_FLOAT3, VES_POSITION, 0);
		mSharedVertexData->vertexDeclaration->addElement(1, 12, 
			VET_FLOAT3, VES_NORMAL, 0);
		mSharedVertexData->vertexDeclaration->addElement(2, 24, 
			VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

		mSharedVertexData->vertexBufferBinding->setBinding(0, mSharedVertexBuffer);
		mSharedVertexData->vertexBufferBinding->setBinding(1, mSharedVertexBuffer);
		mSharedVertexData->vertexBufferBinding->setBinding(2, mSharedVertexBuffer);
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createSharedIndexBuffer()
	{
		size_t numFaceIndices = mNewFaceIndices.size();
		size_t numVertexIndices = numFaceIndices * 3;

		mSharedIndexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_32BIT, numVertexIndices, 
			HardwareBuffer::HBU_STATIC, false);

		uint32* bufferPtr = (uint32*) mSharedIndexBuffer->lock(HardwareBuffer::HBL_DISCARD);

		for(size_t j = 0; j != numFaceIndices; ++j)
		{
			const Bsp::Face& face = mBsp->getFace(mNewFaceIndices[j]);
			*bufferPtr++ = face.getVertexIndex(0);
			*bufferPtr++ = face.getVertexIndex(1);
			*bufferPtr++ = face.getVertexIndex(2);
		}

		mSharedIndexBuffer->unlock();
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createMeshesForClusters()
	{
		for(size_t matIndex = 0; matIndex != mClusterListsByMatIndex.size(); ++matIndex)
		{
			const MaterialPtr material = mBsp->getMaterial(matIndex);
			ClusterList& clusters = mClusterListsByMatIndex[matIndex];
			size_t clusterIndex = 0;
			for(ClusterList::iterator it = clusters.begin(); it != clusters.end(); ++it, ++clusterIndex)
			{
				Cluster* cluster = &*it;
				String meshName = mBsp->getName() + ": " + material->getName() + 
					": " + StringConv::toString(clusterIndex) + "\x05";
				MeshPtr mesh = MeshManagerEx::getSingleton().createManual(meshName);
				mesh->sharedVertexData = mSharedVertexData;
				SubMesh* submesh = mesh->createSubMesh();
				submesh->useSharedVertices = true;
				IndexData* indexData = new IndexData;
				indexData->indexBuffer = mSharedIndexBuffer;
				indexData->indexStart = cluster->firstNewFaceIndex * 3;
				indexData->indexCount = cluster->numNewFaceIndices * 3;
				submesh->indexData = indexData;
				submesh->setMaterialName(material->getName());
				mesh->_setBounds(cluster->bounds);
				cluster->mesh = mesh;
			}
		}
	}




	//---------------------------------------------------------------------------
	// Creating of entities and scene nodes
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createEntities()
	{
		SceneManagerEx* sm = (SceneManagerEx*) (mMainSceneNode->getCreator());
		for(size_t matIndex = 0; matIndex != mClusterListsByMatIndex.size(); ++matIndex)
		{
			ClusterList& clusters = mClusterListsByMatIndex[matIndex];
			for(ClusterList::iterator it = clusters.begin(); it != clusters.end(); ++it)
			{
				Cluster* cluster = &*it;
				Entity* entity = sm->createEntity(cluster->mesh, SceneQueryMask::VOBLEVELCOMPO_BSP);
				if(cluster->subClusterList.size() > 1 && entity->getNumSubEntities() > 0)
				{
					SubEntity* subEnt = entity->getSubEntity(0);
					SubClusterList& subClusters = cluster->subClusterList;
					for(SubClusterList::iterator it2 = subClusters.begin(); it2 != subClusters.end(); ++it2)
					{
						SubCluster* subCluster = &*it2;
						SubSubEntity* ssEnt = sm->createSubSubEntity(subEnt);
						ssEnt->bounds = subCluster->bounds;
						ssEnt->indexStart = subCluster->firstNewFaceIndex;
						ssEnt->indexCount = subCluster->numNewFaceIndices;
					}
					cluster->subEntity = subEnt;
				}
				cluster->entity = entity;
			}
		}
	}
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::createSceneNodes()
	{
		SceneManagerEx* sm = (SceneManagerEx*) (mMainSceneNode->getCreator());
		for(size_t matIndex = 0; matIndex != mClusterListsByMatIndex.size(); ++matIndex)
		{
			ClusterList& clusters = mClusterListsByMatIndex[matIndex];
			for(ClusterList::iterator it = clusters.begin(); it != clusters.end(); ++it)
			{
				Cluster* cluster = &*it;
				SceneNode* child = mMainSceneNode->createChildSceneNode();
				child->attachObject(cluster->entity);
				cluster->sceneNode = child;
			}
		}
	}



	//---------------------------------------------------------------------------
	// Log statistics
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::logStatistics()
	{
		size_t numClusters = 0;
		size_t maxFacesPerCluster = 0;
		size_t minFacesPerCluster = (size_t)-1;
		Vector3 sumClusterSize = Vector3::ZERO;
		Vector3 maxClusterSize = Vector3::ZERO;
		Vector3 minClusterSize = Vector3(Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);
		size_t numSubClusters = 0;
		size_t minSubClustersPerCluster = 0;
		size_t maxSubClustersPerCluster = (size_t)-1;

		for(size_t matIndex = 0; matIndex != mClusterListsByMatIndex.size(); ++matIndex)
		{
			ClusterList& clusters = mClusterListsByMatIndex[matIndex];
			for(ClusterList::iterator it = clusters.begin(); it != clusters.end(); ++it)
			{
				Cluster* cluster = &*it;
				++numClusters;
				
				size_t numFaces = cluster->numNewFaceIndices;
				if(maxFacesPerCluster < numFaces)
					maxFacesPerCluster = numFaces;
				if(minFacesPerCluster > numFaces)
					minFacesPerCluster = numFaces;
	
				Vector3 sz = cluster->bounds.getSize();
				sumClusterSize += sz;
				minClusterSize.makeFloor(sz);
				maxClusterSize.makeCeil(sz);

				size_t numSC = cluster->subClusterList.size();
				numSubClusters += numSC;
				if(maxSubClustersPerCluster < numSC)
					maxSubClustersPerCluster = numSC;
				if(minSubClustersPerCluster > numSC)
					minSubClustersPerCluster = numSC;
			}
		}

		if(!numClusters)
			return;

		size_t numFaces = mBsp->getNumFaces();
		size_t avgFacesPerCluster = numFaces / numClusters;
		size_t avgSubClustersPerCluster = numSubClusters / numClusters;
		Vector3 avgClusterSize = sumClusterSize / (Real) numClusters;

		String prefix = mBsp->getName() + " - ZenBspVisualCreator: ";
		GOTHOGRE_INFO(prefix << "number of clusters: " << numClusters);
		
		GOTHOGRE_INFO(prefix << "clusters' sizes: "
			<< "min: (" << StringConv::toString(minClusterSize) << "), " 
			<< "avg: (" << StringConv::toString(avgClusterSize) << "), "
			<< "max: (" << StringConv::toString(maxClusterSize) << ").");

		GOTHOGRE_INFO(prefix << "faces per cluster: "
			<< "min: " << minFacesPerCluster << ", "
			<< "avg: " << avgFacesPerCluster << ", "
			<< "max: " << maxFacesPerCluster);

		GOTHOGRE_INFO(prefix << "subclusters per cluster: "
			<< "min: " << minSubClustersPerCluster << ", "
			<< "avg: " << avgSubClustersPerCluster << ", "
			<< "max: " << maxSubClustersPerCluster);
	}




	//---------------------------------------------------------------------------
	// Main part
	//---------------------------------------------------------------------------
	void ZenBspVisualCreator::execute()
	{
		createClusters();
		createMeshes();
		createEntities();
		createSceneNodes();
		logStatistics();
	}
	//---------------------------------------------------------------------------
	ZenBspVisualCreator::~ZenBspVisualCreator()
	{
	}
	//---------------------------------------------------------------------------
	ZenBspVisualCreator::ZenBspVisualCreator(SceneNode* _mainSceneNode, BspPtr _bsp)
	{
		mMainSceneNode = _mainSceneNode;
		mBsp = _bsp;
	}




} // namespace GothOgre