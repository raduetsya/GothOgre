#ifndef VISIBLE_BSP_LEAVES_SEARCH_H
#define VISIBLE_BSP_LEAVES_SEARCH_H


namespace GothOgre
{
	class CameraInfo;

	class VisibleBspLeavesSearch
	{
	public:
		VisibleBspLeavesSearch(const BspPtr& _bsp, Entity* _entity,
			const CameraInfo* _cameraInfo, RenderQueue* _renderQueue, 
			size_t _checkVisibilityCount);

		~VisibleBspLeavesSearch();

		void run();

	private:
		void startSearching();
		void endSearching();
		void enumVisibleBspLeaves();
		void enumVisibleBspLeaves(Bsp::NodeBase* _nodeBase, const size_t* _planeIndices);
		void processVisibleBspLeaf(Bsp::Leaf* _leaf, const size_t* _planeIndices);
		void findVisibleSceneNodes(Bsp::Leaf* _leaf, const size_t* _planeIndices);
		void addFaces(size_t _firstFaceIndex, size_t _numFaceIndices);
		void addBspEntityToRenderQueue();

	private:
		const CameraInfo*				mCameraInfo;
		RenderQueue*					mRenderQueue;
		Entity*							mEntity;
		MeshPtr							mMesh;
		BspPtr							mBsp;
		size_t							mCheckVisibilityCount;

		size_t							mNumMaterials;
		IndexData**						mIndexDatas;
		HardwareIndexBufferSharedPtr	mHardwareIndexBuffer;
		uint32*							mIndexBuffer;
		size_t							mNumFaceIndices;
		size_t*							mFaceIndices;
		size_t							mNumFaces;
		Bsp::Face*						mFaces;
		bool*							mFaceVisibilities;
		size_t							mNumVisibleFaces;
	};

} // namespace GothOgre

#endif // VISIBLE_BSP_LEAVES_SEARCH_H