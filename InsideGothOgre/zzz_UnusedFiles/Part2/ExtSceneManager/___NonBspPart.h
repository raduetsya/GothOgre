#ifndef NON_BSP_PART_H
#define NON_BSP_PART_H

#include "FastContainers.h"

namespace GothOgre
{
	class CameraInfo;

	// Partition - это большой параллелепипед, содержащий множество SceneNod'ов.
	// Partition'ы применяются для SceneNod'ов, ограничивающие параллелепипеды
	// которых выступают за границы любого используемого Bsp.
	class NonBspPart
	{
	public:
		NonBspPart();
		~NonBspPart();

		void setCenter(const Vector3& _center) {mCenter = _center;}
		const Vector3& getCenter() const {return mCenter;}

		void setHalfSize(const Vector3& _halfSize) {mHalfSize = _halfSize;}
		const Vector3& getHalfSize() const {return mHalfSize;}

		void addSceneNode(SceneNode* _sceneNode);
		void removeSceneNode(IteratorOfSceneNode _it);

		bool containsSceneNodes() const {return !mSceneNodes.empty();}

		void findVisibleObjects(const CameraInfo* _cameraInfo, 
			RenderQueue* _renderQueue, size_t _checkVisibilityCount);

	private:
		ListOfSceneNodes	mSceneNodes;
		Vector3				mCenter;
		Vector3				mHalfSize;
	};
};


#endif // NON_BSP_PART_H