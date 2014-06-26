#include "Precompiled.h"
#include "BspPart.h"
#include "CameraInfo.h"
#include "VisibleBspLeavesSearch.h"
#include "ExtSceneNode.h"

namespace GothOgre
{
	BspPart::BspPart()
	{
		mEntity = nullptr;
		mSceneNodesRepositioned = false;
	}
	//-----------------------------------------------------------------------------
	BspPart::~BspPart()
	{
		reset();
	}
	//-----------------------------------------------------------------------------
	void BspPart::reset()
	{
		destroyEntity();
		destroyMesh();
		destroyListsOfSceneNodes();
		mSceneNodesRepositioned = false;
	}
	//-----------------------------------------------------------------------------
	void BspPart::setBsp(const BspPtr& _bsp)
	{
		if(mBsp != _bsp)
		{
			reset();
			mBsp = _bsp;
			createListsOfSceneNodes();
			createMesh();
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::destroyMesh()
	{
		if(!mMesh.isNull())
		{
			MeshManager::getSingleton().remove(mMesh);
			mMesh.setNull();
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::createMesh()
	{
		if(mMesh.isNull() && !mBsp.isNull())
		{
			// Make unique name
			StringStream ss;
			ss << "\x03" << mBsp->getName() << "_" << this;
			String meshName = ss.str();

			// Create and initialise mesh and submeshes
			mMesh = MeshManager::getSingleton().createManual(meshName);
			createSharedVertices();
			createSubMeshes();

			mBspStateCount = mBsp->getStateCount();
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::createSharedVertices()
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
	
		// Initialise shared vertex data
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
	//-----------------------------------------------------------------------------
	void BspPart::createSubMeshes()
	{
		size_t totalNumIndices = mBsp->getNumFaces() * 3;
		HardwareIndexBufferSharedPtr buffer = HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_32BIT, totalNumIndices, 
			HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false);

		size_t numMaterials = mBsp->getNumMaterials();

		vector<size_t>::type numFacesByMaterial;
		numFacesByMaterial.resize(numMaterials, 0);
		size_t numFaces = mBsp->getNumFaces();
		for(size_t i = 0; i != numFaces; ++i)
		{
			const Bsp::Face& face = mBsp->getFace(i);
			numFacesByMaterial[face.getMaterialIndex()]++;
		}

		size_t indexStart = 0;
		for(size_t j = 0; j != numMaterials; ++j)
		{
			size_t numIndices = numFacesByMaterial[j] * 3;
			const String& materialName = mBsp->getMaterial(j)->getName();
				
			SubMesh* subMesh = mMesh->createSubMesh();
			subMesh->useSharedVertices = true;
			subMesh->setMaterialName(materialName);
			
			IndexData* indexData = subMesh->indexData;
			indexData->indexStart = indexStart;
			indexStart += numIndices;
			indexData->indexCount = 0;
			indexData->indexBuffer = buffer;
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::createEntity(SceneManager* _sceneManager)
	{
		if(!mEntity && !mMesh.isNull())
		{
			NameValuePairList params;
			params["mesh"] = mMesh->getName();
			params["resourceGroup"] = mMesh->getGroup();
			mEntity = static_cast<BspEntity*>(
				_sceneManager->createMovableObject(BspEntityFactory::FACTORY_TYPE_NAME, 
				&params));
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::destroyEntity()
	{
		if(mEntity)
		{
			mEntity->_getManager()->destroyMovableObject(mEntity);
			mEntity = nullptr;
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::destroyListsOfSceneNodes()
	{
		if(!mListsOfSceneNodes.empty())
		{
			destroyListsOfSceneNodes(mBsp->getRootNode());
			mListsOfSceneNodes.clear();
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::destroyListsOfSceneNodes(Bsp::NodeBase* _nodeBase)
	{
		if(_nodeBase->isLeaf())
		{
			Bsp::Leaf* leaf = static_cast<Bsp::Leaf*>(_nodeBase);
			ListOfSceneNodes* sceneNodes = getSceneNodesIntersectBspLeaf(leaf);
			for(IteratorOfSceneNode itSN = sceneNodes->begin(); 
				itSN != sceneNodes->end(); ++itSN)
			{
				FastSceneNode* fsn = static_cast<FastSceneNode*>( *itSN );
				for(PositionInBspPart::iterator it2 = fsn->mPositionInBspPart.begin();
					it2 != fsn->mPositionInBspPart.end(); ++it2)
				{
					PositionInBspLeaf& posInBspLeaf = *it2;
					if(posInBspLeaf.bspLeaf == leaf)
					{
						fsn->mPositionInBspPart.erase(it2);
						break;
					}
				}

				if(!fsn->mPositionInNonBspPart.nonBspPart)
				{
					fsn->_addToNonBspPart();
				}
			}
			sceneNodes->clear();
			leaf->setParam(0);
		}
		else
		{
			Bsp::Node* node = static_cast<Bsp::Node*>(_nodeBase);
			destroyListsOfSceneNodes(node->getFront());
			destroyListsOfSceneNodes(node->getBack());
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::createListsOfSceneNodes()
	{
		if(mListsOfSceneNodes.empty() && !mBsp.isNull())
		{
			mListsOfSceneNodes.reserve(mBsp->getNumLeaves());
			createListsOfSceneNodes(mBsp->getRootNode());
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::createListsOfSceneNodes(Bsp::NodeBase* _nodeBase)
	{
		if(_nodeBase->isLeaf())
		{
			Bsp::Leaf* leaf = static_cast<Bsp::Leaf*>(_nodeBase);
			mListsOfSceneNodes.push_back(ListOfSceneNodes());
			ListOfSceneNodes* sceneNodes = &mListsOfSceneNodes.back();
			leaf->setParam( reinterpret_cast<size_t>( sceneNodes ) );
		}
		else
		{
			Bsp::Node* node = static_cast<Bsp::Node*>(_nodeBase);
			createListsOfSceneNodes(node->getFront());
			createListsOfSceneNodes(node->getBack());
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::repositionSceneNodesFromNonBspParts(SceneManager* _sceneManager)
	{
		if(!mSceneNodesRepositioned)
		{
			mSceneNodesRepositioned = true;
			repositionSceneNodesFromNonBspParts(_sceneManager->getRootSceneNode());
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::repositionSceneNodesFromNonBspParts(SceneNode* _sceneNode)
	{
		FastSceneNode* fsn = static_cast<FastSceneNode*>( _sceneNode );
		if(fsn->mPositionInNonBspPart.nonBspPart != nullptr)
		{
			bool allInBsp;
			addSceneNodeIfIntersects(_sceneNode, &allInBsp);
			if(allInBsp)
			{
				fsn->_removeFromNonBspPart();
			}
		}

		SceneNode::ChildNodeIterator it = _sceneNode->getChildIterator();
		while(it.hasMoreElements())
		{
			SceneNode* child = static_cast<SceneNode*>( it.getNext() );
			repositionSceneNodesFromNonBspParts(child);
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::prepare(SceneManager* _sceneManager)
	{
		createListsOfSceneNodes();
		createMesh();
		createEntity(_sceneManager);
		repositionSceneNodesFromNonBspParts(_sceneManager);
	}
	//-----------------------------------------------------------------------------
	void BspPart::findVisibleObjects(const CameraInfo* _cameraInfo, 
		RenderQueue* _renderQueue, size_t _checkVisibilityCount)
	{
		if(!mBsp.isNull())
		{
			if(mBsp->getStateCount() != mBspStateCount)
			{
				reset();
			}

			SceneManagerPtr sceneManager = _cameraInfo->getCamera()->getSceneManager();
			prepare(sceneManager);

			VisibleBspLeavesSearch bspSearch(mBsp, mEntity, _cameraInfo, _renderQueue, _checkVisibilityCount);
			bspSearch.run();
		}
	}
	//-----------------------------------------------------------------------------
	BspPart::SceneNodeLocation BspPart::getSceneNodeLocation(SceneNode* _sceneNode) const
	{
		FastSceneNode* fsn = static_cast<FastSceneNode*> (_sceneNode);
		const Vector3& fsn_center = fsn->getWorldAABBCenter();
		const Vector3& fsn_halfSize = fsn->getWorldAABBHalfSize();
		const Vector3& bsp_center = mBsp->getRootNode()->getBoundingBoxCenter();
		const Vector3& bsp_halfSize = mBsp->getRootNode()->getBoundingBoxHalfSize();
		
		Vector3 diff_center = fsn_center - bsp_center;
		diff_center.x = Math::Abs(diff_center.x);
		diff_center.y = Math::Abs(diff_center.y);
		diff_center.z = Math::Abs(diff_center.z);

		if(diff_center.x < bsp_halfSize.x - fsn_halfSize.x
			&& diff_center.y < bsp_halfSize.y - fsn_halfSize.y
			&& diff_center.z < bsp_halfSize.z - fsn_halfSize.z)
			return InsideBsp;

		if(diff_center.x > bsp_halfSize.x + fsn_halfSize.x
			|| diff_center.y > bsp_halfSize.y + fsn_halfSize.y
			|| diff_center.z > bsp_halfSize.z + fsn_halfSize.z)
			return OutsideBsp;

		return BoundaryBsp;
	}
	//-----------------------------------------------------------------------------
	void BspPart::addSceneNodeIfIntersects(SceneNode* _sceneNode, bool* _allInside)
	{
		SceneNodeLocation location = getSceneNodeLocation(_sceneNode);
		if(location == OutsideBsp)
		{
			*_allInside = false;
		}
		else
		{
			*_allInside = (location == InsideBsp);
			AddSceneNodeToBspLeaves add(_sceneNode);
			add.run(mBsp->getRootNode());
		}
	}
	//-----------------------------------------------------------------------------
	BspPart::AddSceneNodeToBspLeaves::AddSceneNodeToBspLeaves(SceneNode* _sceneNode)
	{
		mSceneNode = _sceneNode;
		FastSceneNode* fsn = static_cast<FastSceneNode*>(_sceneNode);
		mBoxMin = fsn->_getWorldAABB().getMinimum();
		mBoxMax = fsn->_getWorldAABB().getMaximum();
	}
	//-----------------------------------------------------------------------------
	void BspPart::AddSceneNodeToBspLeaves::run(Bsp::NodeBase* _nodeBase)
	{
		if(_nodeBase->isLeaf())
		{
			Bsp::Leaf* leaf = static_cast<Bsp::Leaf*>(_nodeBase);
			ListOfSceneNodes* sceneNodes = getSceneNodesIntersectBspLeaf( leaf );
			sceneNodes->push_front(mSceneNode);
			
			FastSceneNode* fsn = static_cast<FastSceneNode*>(mSceneNode);
			fsn->mPositionInBspPart.push_front(PositionInBspLeaf());
			PositionInBspLeaf& posInBspLeaf = fsn->mPositionInBspPart.front();
			posInBspLeaf.bspLeaf = leaf;
			posInBspLeaf.iterator = sceneNodes->begin();
		}
		else
		{
			Bsp::Node* node = static_cast<Bsp::Node*>(_nodeBase);
			const Plane& sepPlane = node->getSeparationPlane();
			Real fmax = sepPlane.getDistance(mBoxMax);
			Real fmin = sepPlane.getDistance(mBoxMin);
			bool qfront = (fmax > 0);
			bool qback = (fmin < 0);
			
			if(qfront)
				run(node->getFront());

			if(qback)
				run(node->getBack());
		}
	}
	//-----------------------------------------------------------------------------
	void BspPart::removeSceneNodeFromAllBspParts(SceneNode* _sceneNode)
	{
		FastSceneNode* fsn = static_cast<FastSceneNode*>( _sceneNode );
		for(PositionInBspPart::iterator it = fsn->mPositionInBspPart.begin();
			it != fsn->mPositionInBspPart.end(); ++it)
		{
			PositionInBspLeaf& posInBspLeaf = *it;
			Bsp::Leaf* bspLeaf = posInBspLeaf.bspLeaf;
			getSceneNodesIntersectBspLeaf(bspLeaf)->erase(posInBspLeaf.iterator);
		}
		fsn->mPositionInBspPart.clear();
	}
	//-----------------------------------------------------------------------------
	String BspPart::findSector(const Vector3& _point) const
	{
		FindSector fs(_point);
		return fs.run(mBsp->getRootNode());
	}
	//-----------------------------------------------------------------------------
	BspPart::FindSector::FindSector(const Vector3& _point)
	{
		mPoint = _point;
	}
	//-----------------------------------------------------------------------------
	String BspPart::FindSector::run(Bsp::NodeBase* _nodeBase)
	{
		return StringUtil::BLANK;
	/*	if(_nodeBase->isLeaf())
		{
			Bsp::Leaf* leaf = static_cast<Bsp::Leaf*>(_nodeBase);
			String s;
			s = "";
			for(size_t i = 0; i != leaf->getSectors().size(); ++i)
				s += leaf->getSectors()[i]->getName();
			return s;
		}
		else
		{
			Bsp::Node* node = static_cast<Bsp::Node*>(_nodeBase);
			const Plane& sepPlane = node->getSeparationPlane();
			Real f = sepPlane.getDistance(mPoint);
			if(f > 0)
				return run(node->getFront());
			else
				return run(node->getBack());
		}*/
	}

} // namespace GothOgre