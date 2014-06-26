#include "Precompiled.h"
#include "VisibleBspLeavesSearch.h"
#include "CameraInfo.h"
#include "ExtSceneNode.h"
#include "BspPart.h"

namespace GothOgre
{
	VisibleBspLeavesSearch::VisibleBspLeavesSearch(const BspPtr& _bsp, 
		Entity* _entity, const CameraInfo* _cameraInfo, 
		RenderQueue* _renderQueue, size_t _checkVisibilityCount)
	{
		mCameraInfo = _cameraInfo;
		mRenderQueue = _renderQueue;
		mEntity = _entity;
		mMesh = mEntity->getMesh();
		mBsp = _bsp;
		mCheckVisibilityCount = _checkVisibilityCount;

		mNumMaterials = mBsp->getNumMaterials();
		if(mNumMaterials != 0)
		{
			mIndexDatas = new IndexData*[mNumMaterials];
			for(size_t i = 0; i != mNumMaterials; ++i)
			{
				IndexData* indexData = mMesh->getSubMesh(i)->indexData;
				mIndexDatas[i] = indexData;
			}
			mHardwareIndexBuffer = mIndexDatas[0]->indexBuffer;
		}
		else
		{
			mIndexDatas = nullptr;
			mHardwareIndexBuffer.setNull();
		}

		mNumFaces = mBsp->getNumFaces();
		if(mNumFaces != 0)
		{
			Bsp::Face& face0 = mBsp->getFace(0);
			mFaces = &face0;
			mFaceVisibilities = new bool[mNumFaces];
		}
		else
		{
			mFaces = nullptr;
			mFaceVisibilities = nullptr;
		}

		mNumFaceIndices = mBsp->getNumFaceIndices();
		if(mNumFaceIndices != 0)
		{
			size_t& faceIndex0 = mBsp->getFaceIndex(0);
			mFaceIndices = &faceIndex0;
		}
		else
		{
			mFaceIndices = nullptr;
		}

		mNumVisibleFaces = 0;
	}
	//-----------------------------------------------------------------------------
	VisibleBspLeavesSearch::~VisibleBspLeavesSearch()
	{
		if(mIndexDatas)
			delete[] mIndexDatas;

		if(mFaceVisibilities)
			delete[] mFaceVisibilities;
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::run()
	{
		startSearching();
		enumVisibleBspLeaves();
		endSearching();
		addBspEntityToRenderQueue();
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::startSearching()
	{
		// reset visibility flags
		memset(mFaceVisibilities, 0, mNumFaces * sizeof(bool));

		// lock index buffer
		mIndexBuffer = (uint32*) mHardwareIndexBuffer->lock(HardwareBuffer::HBL_DISCARD);

		for(size_t j = 0; j != mNumMaterials; ++j)
		{
			IndexData* indexData = mIndexDatas[j];
			if(indexData)
				indexData->indexCount = indexData->indexStart;
		}
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::endSearching()
	{
		// unlock index buffers
		mHardwareIndexBuffer->unlock();
		mIndexBuffer = nullptr;

		size_t totalNumIndices = 0;
		for(size_t j = 0; j != mNumMaterials; ++j)
		{
			IndexData* indexData = mIndexDatas[j];
			if(indexData)
			{
				indexData->indexCount -= indexData->indexStart;
				totalNumIndices += indexData->indexCount;
			}
		}
		
		mNumVisibleFaces = totalNumIndices / 3;
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::enumVisibleBspLeaves()
	{
		enumVisibleBspLeaves(mBsp->getRootNode(), mCameraInfo->getAllPlaneIndices());
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::enumVisibleBspLeaves(Bsp::NodeBase* _nodeBase, const size_t* _planeIndices)
	{
		CameraInfo::PlaneIndices planeIndices2;
		if(mCameraInfo->isVisibleBox(_nodeBase->getBoundingBoxCenter(), _nodeBase->getBoundingBoxHalfSize(), 
			_planeIndices, planeIndices2))
		{
			if(_nodeBase->isLeaf())
			{
				Bsp::Leaf* leaf = static_cast<Bsp::Leaf*>(_nodeBase);
				
				/*
				bool found = false;
				for(size_t i = 0; i != mBsp->getNumSectors(); ++i)
				{
					const Bsp::Sector& sector = mBsp->getSector(1);
					const vector<Bsp::Leaf*>::type& leaves = sector.getVisibleLeaves();
					if(std::find(leaves.begin(), leaves.end(), leaf) != leaves.end())
					{
						found = true;
						break;
					}
				}
				if(found)*/
					addFaces(leaf->getFirstFaceIndex(), leaf->getNumFaceIndices());
				findVisibleSceneNodes(leaf, planeIndices2);
			}
			else
			{
				Bsp::Node* node = static_cast<Bsp::Node*>(_nodeBase);
				enumVisibleBspLeaves(node->getBack(), planeIndices2);
				enumVisibleBspLeaves(node->getFront(), planeIndices2);
			}
		}
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::addFaces(size_t _firstFaceIndex, size_t _numFaceIndices)
	{
		if(_numFaceIndices != 0)
		{
			size_t endFaceIndex = _firstFaceIndex + _numFaceIndices;
			for(size_t i = _firstFaceIndex; i != endFaceIndex; ++i)
			{
				size_t faceIndex = mFaceIndices[i];
				if(!mFaceVisibilities[faceIndex])
				{
					const Bsp::Face& face = mFaces[faceIndex];
					mFaceVisibilities[faceIndex] = 1;
					size_t materialIndex = face.getMaterialIndex();
					IndexData* indexData = mIndexDatas[materialIndex];
					uint32* indexBuffer = mIndexBuffer + indexData->indexCount;
					*indexBuffer = face.getVertexIndex(0);
					*++indexBuffer = face.getVertexIndex(1);
					*++indexBuffer = face.getVertexIndex(2);
					indexData->indexCount += 3;
				}
			}
		}
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::addBspEntityToRenderQueue()
	{
		for(size_t i = 0; i != mNumMaterials; ++i)
		{
			SubEntity* subEntity = mEntity->getSubEntity(i);
            if(subEntity->isVisible() && mIndexDatas[i] && mIndexDatas[i]->indexCount != 0)
            {
				mRenderQueue->addRenderable(subEntity);
            }
        }
	}
	//-----------------------------------------------------------------------------
	void VisibleBspLeavesSearch::findVisibleSceneNodes(Bsp::Leaf* _leaf, const size_t* _planeIndices)
	{
		ListOfSceneNodes* sceneNodes = BspPart::getSceneNodesIntersectBspLeaf(_leaf);
		for(IteratorOfSceneNode it = sceneNodes->begin(); it != sceneNodes->end(); ++it)
		{
			FastSceneNode* sceneNode = static_cast<FastSceneNode*>(*it);
			sceneNode->_checkSceneNodeVisibility(mCameraInfo, mRenderQueue,
				mCheckVisibilityCount, _planeIndices);
		}
	}

} // namespace GothOgre
