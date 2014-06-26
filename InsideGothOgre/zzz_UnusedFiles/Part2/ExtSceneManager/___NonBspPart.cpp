#include "Precompiled.h"
#include "NonBspPart.h"
#include "CameraInfo.h"
#include "ExtSceneNode.h"

namespace GothOgre
{
	//-----------------------------------------------------------------------------
	NonBspPart::NonBspPart()
	{
	}
	//-----------------------------------------------------------------------------
	NonBspPart::~NonBspPart()
	{
		for(IteratorOfSceneNode it = mSceneNodes.begin(); it != mSceneNodes.end(); ++it)
		{
			FastSceneNode* fsn = static_cast<FastSceneNode*>( *it );
			if(fsn->mPositionInNonBspPart.nonBspPart == this)
				fsn->mPositionInNonBspPart.nonBspPart = nullptr;
		}
	}
	//-----------------------------------------------------------------------------
	void NonBspPart::addSceneNode(SceneNode* _sceneNode)
	{
		FastSceneNode* fsn = static_cast<FastSceneNode*>( _sceneNode );
		mSceneNodes.push_front(_sceneNode);
		fsn->mPositionInNonBspPart.iterator = mSceneNodes.begin();
		fsn->mPositionInNonBspPart.nonBspPart = this;
	}
	//-----------------------------------------------------------------------------
	void NonBspPart::removeSceneNode(IteratorOfSceneNode it)
	{
		FastSceneNode* fsn = static_cast<FastSceneNode*>( *it );
		mSceneNodes.erase(it);
		fsn->mPositionInNonBspPart.nonBspPart = nullptr;
	}
	//-----------------------------------------------------------------------------
	void NonBspPart::findVisibleObjects(const CameraInfo* _cameraInfo, RenderQueue* _renderQueue, size_t _checkVisibilityCount)
	{
		if(!mSceneNodes.empty())
		{
			CameraInfo::PlaneIndices planeIndices;
			if(_cameraInfo->isVisibleBox(mCenter, mHalfSize, _cameraInfo->getAllPlaneIndices(), planeIndices))
			{
				for(IteratorOfSceneNode it = mSceneNodes.begin(); it != mSceneNodes.end(); ++it)
				{
					FastSceneNode* sceneNode = static_cast<FastSceneNode*>(*it);
					sceneNode->_checkSceneNodeVisibility(_cameraInfo, _renderQueue,
						_checkVisibilityCount, planeIndices);
				}
			}
		}
	}

} // namespace GothOgre