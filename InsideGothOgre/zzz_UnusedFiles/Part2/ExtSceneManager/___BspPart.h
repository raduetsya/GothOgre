#ifndef BSP_PART_H
#define BSP_PART_H

#include "FastContainers.h"

namespace GothOgre
{
	class CameraInfo;

	class BspPart
	{
	public:
		BspPart();
		~BspPart();

		//
		void setBsp(const BspPtr& _bsp);

		//
		BspPtr getBsp() const {return mBsp;}

		//
		void findVisibleObjects(const CameraInfo* _cameraInfo, 
			RenderQueue* _renderQueue, size_t _checkVisibilityCount);

		//
		static ListOfSceneNodes* getSceneNodesIntersectBspLeaf(Bsp::Leaf* _leaf)
		{
			return reinterpret_cast<ListOfSceneNodes*>( _leaf->getParam() );
		}

		//
		void addSceneNodeIfIntersects(SceneNode* _sceneNode, bool* _hasOutsidePart);

		//
		static void removeSceneNodeFromAllBspParts(SceneNode* _sceneNode);
		
		//
		String findSector(const Vector3& _point) const;

	private:
		class AddSceneNodeToBspLeaves
		{
		public:
			AddSceneNodeToBspLeaves(SceneNode* _sceneNode);
			void run(Bsp::NodeBase* _nodeBase);

		private:
			SceneNode*	mSceneNode;
			Vector3		mBoxMin;
			Vector3		mBoxMax;
		};

		enum SceneNodeLocation
		{
			InsideBsp,
			OutsideBsp,
			BoundaryBsp
		};

	private:
		class FindSector
		{
		public:
			FindSector(const Vector3& _point);
			String run(Bsp::NodeBase* _nodeBase);

		private:
			Vector3 mPoint;
		};

	private:
		void reset();
		void prepare(SceneManager* _sceneManager);
		void createMesh();
		void destroyMesh();
		void createSharedVertices();
		void createSubMeshes();
		void createEntity(SceneManager* _sceneManager);
		void destroyEntity();
		void createListsOfSceneNodes();
		void createListsOfSceneNodes(Bsp::NodeBase* _nodeBase);
		void destroyListsOfSceneNodes();
		void destroyListsOfSceneNodes(Bsp::NodeBase* _nodeBase);
		void repositionSceneNodesFromNonBspParts(SceneManager* _sceneManager);
		void repositionSceneNodesFromNonBspParts(SceneNode* _sceneNode);
		SceneNodeLocation getSceneNodeLocation(SceneNode* _sceneNode) const;

	private:
		BspPtr		mBsp;
		size_t		mBspStateCount;
		MeshPtr		mMesh;
		BspEntity*	mEntity;
		bool		mSceneNodesRepositioned;
		vector<ListOfSceneNodes>::type mListsOfSceneNodes;
	};

} // namespace GothOgre

#endif // BSP_PART_H