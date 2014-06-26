#ifndef ZEN_BSP_VISUAL_CREATOR_H
#define ZEN_BSP_VISUAL_CREATOR_H


namespace GothOgre
{
	//---------------------------------------------------------------------------
	/** Special class to create scene nodes to display BSP-tree.*/
	class ZenBspVisualCreator
	{
	public:
		ZenBspVisualCreator(SceneNode* _mainSceneNode, BspPtr _bsp);
		~ZenBspVisualCreator();

		void execute();

	private:
		void createClusters();
		void createClustersForLeaves(Bsp::NodeBase* _nodebase);
		void updateClustersByLeaf(Bsp::Leaf* _leaf);
		void updateClustersByFace(size_t _faceIndex);
		void calculateNewFaceIndices();

		void createMeshes();
		void createSharedVertexData();
		void createSharedIndexBuffer();
		void createMeshesForClusters();
		
		void createEntities();
		void createSceneNodes();
		void logStatistics();

	private:
		struct SubCluster
		{
			AxisAlignedBox	bounds;
			size_t			firstNewFaceIndex;
			size_t			numNewFaceIndices;
			size_t          currentNewFaceIndex;
			SubSubEntity*   subSubEntity;
		};

		typedef list<SubCluster>::type    SubClusterList;

		struct Cluster
		{
			AxisAlignedBox	bounds;
			SubClusterList  subClusterList;
			size_t			firstNewFaceIndex;
			size_t			numNewFaceIndices;
			MeshPtr         mesh;
			Entity*         entity;
			SubEntity*      subEntity;
			SceneNode*      sceneNode;
		};

		typedef list<Cluster>::type        ClusterList;
		typedef vector<ClusterList>::type  ClusterListsByMatIndex;
		typedef vector<SubCluster*>::type  SubClustersByFaceIndex;
		typedef vector<size_t>::type       FaceIndices;

	private:
		BspPtr				            mBsp;
		SceneNode*			            mMainSceneNode;

		Real                            mMaxClusterSize;
		Real                            mMaxSubClusterSize;
		SubClustersByFaceIndex          mSubClustersByFaceIndex;
		FaceIndices                     mNewFaceIndices;
		ClusterListsByMatIndex          mClusterListsByMatIndex;

		HardwareVertexBufferSharedPtr	mSharedVertexBuffer;
		VertexData*						mSharedVertexData;
		HardwareIndexBufferSharedPtr	mSharedIndexBuffer;
	};

} // namespace GothOgre

#endif // ZEN_BSP_VISUAL_CREATOR_H