#ifndef FAST_SCENE_NODE_H
#define FAST_SCENE_NODE_H

#include "FastContainers.h"

namespace GothOgre
{
	class CameraInfo;

	class FastSceneNode : public SceneNode
	{
	public:
        /** Constructor, only to be called by the creator SceneManager.
        @remarks
            Creates a node with a generated name. */
		FastSceneNode(SceneManager* _creator);
        
		/** Constructor, only to be called by the creator SceneManager.
        @remarks
            Creates a node with a specified name. */
		FastSceneNode(SceneManager* _creator, const String& _name);

		virtual ~FastSceneNode();

		/** Tells the SceneNode to update the world bound info it stores. */
		virtual void _updateBounds();

		const Vector3& getWorldAABBCenter() const {return mWorldAABBCenter;}
		const Vector3& getWorldAABBHalfSize() const {return mWorldAABBHalfSize;}

		void _addToParts();
		void _addToBspParts(bool* _allInside);
		void _addToNonBspPart();

		void _removeFromParts();
		void _removeFromBspParts();
		void _removeFromNonBspPart();

		void _checkSceneNodeVisibility(const CameraInfo* _cameraInfo, 
			RenderQueue* _renderQueue, size_t _checkVisibilityCount,
			const size_t* _planeIndices);

	private:
		friend class BspPart;
		friend class NonBspPart;

	private:
		// Список узлов BSP-дерева, в которые был добавлен этот SceneNode
		PositionInBspPart		mPositionInBspPart;
		PositionInNonBspPart	mPositionInNonBspPart;
		size_t					mCheckVisibilityCount;

		Vector3					mWorldAABBCenter;
		Vector3					mWorldAABBHalfSize;

	public:
		static unsigned __int64 sUpdateBoundsTime;
	};

} // namespace GothOgre

#endif // FAST_SCENE_NODE_H