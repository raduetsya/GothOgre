#ifndef ENTITY_CREATOR_FOR_LARGE_MESH_H
#define ENTITY_CREATOR_FOR_LARGE_MESH_H


namespace GothOgre
{
	class SceneManagerExImpl;


	//-------------------------------------------------------------------------
	/** This class can separate a single large mesh (for example, levelmesh) 
	into lots smaller meshes, and create entities for these smaller meshes.
	It's useful to speed up rendering and raycasting.
	(A single large mesh is processed very slowly. */
	class EntityCreatorForLargeMesh
	{
	public:
		EntityCreatorForLargeMesh(SceneManagerExImpl* _sceneManager);
		~EntityCreatorForLargeMesh();

		typedef SceneManagerEx::EntityList EntityList;
		
		/** Creates a list of entities for a large mesh.
		The function uses splitting of the specified mesh into several meshes.
		The function can be used for static meshes only. */
		void createEntityList(EntityList& _entList, const MeshExPtr& _mesh, uint32 _mask);
		
		/** Returns a list of subsubentities. */
		SubSubEntityList& getSubSubEntityList(SubEntity* _subEnt);

		/** Destroys a list of entities, created by 
		the "createEntityListForLarge" function. */
		void destroyEntityList(EntityList& _entList);

	private:
		class AABoxLimited
		{
		public:
			AABoxLimited() {}

			void init(const Vector3& _min, const Vector3& _max, Real _sizeLimit);
			bool merge(const Vector3& _min, const Vector3& _max);

			const Vector3& getMinimum() const {return mMinimum;}
			const Vector3& getMaximum() const {return mMaximum;}
			Vector3 getSize() const {return mMaximum - mMinimum;}
			AxisAlignedBox getAxisAlignedBox() const {return AxisAlignedBox(mMinimum, mMaximum);}

		private:
			Vector3  mMinimum;
			Vector3  mMaximum;
			Real     mSizeLimit;
			Vector3  mPossibleMinimum;
			Vector3  mPossibleMaximum;
		};

	private:
		struct SubCluster
		{
			AABoxLimited	bounds;
			size_t          indexStart;
			size_t          indexCount;
			size_t          currentIndex;
		};

		typedef list<SubCluster>::type    SubClusters;

		struct Cluster
		{
			AABoxLimited	bounds;
			SubClusters     subclusters;
			size_t          indexStart;
			size_t          indexCount;
			IndexData*      destIndexData;
			MeshExPtr       destMesh;
			Entity*         destEntity;
		};

		typedef list<Cluster>::type   Clusters;

		struct Face
		{
			size_t        indexStart;
			size_t        vertexIndices[3];
			SubCluster*   subcluster;
		};

		typedef vector<Face>::type    Faces;
		typedef map<VertexData*, VertexData*>::type  VertexDataByVertexData;

	private:
		void separateSubMesh();
		void copyFaces();
		void createClusters();
		void updateClustersByFace(Face& _face);
		void updateClustersByFace2(Face& _face, const Vector3* _coords);
		void calculateFaceIndices();
		void writeNewFaceOrderToIndexBuffer();
		
		void lockBuffers();
		void unlockBuffers();
		void lockPositionBuffer();
		void unlockPositionBuffer();
		void lockIndexBuffer();
		void unlockIndexBuffer();

		void createDestVertexData();
		void createDestIndexDatas();
		void createDestMeshes();
		void createDestEntities();

		void clear();

		void updateStatistics();
		void logStatistics();
		void clearStatistics();

	private:
		SceneManagerExImpl*             mSceneManager;
		MeshExPtr                       mMesh;
		uint32                          mQueryMask;
		EntityList*                     mResultEntityList;
		SubMeshEx*                      mSubMesh;

		Real                            mMaxClusterSize;
		Real                            mMaxSubClusterSize;
		Clusters	  					mClusters;
		Faces                           mFaces;

		VertexData*                     mVertexData;
		HardwareVertexBufferSharedPtr   mPositionBuffer;
		size_t                          mPositionStart;
		size_t                          mPositionSize;
		size_t                          mPositionOffset;
		VertexElementType               mPositionType;
		const uint8*                    mPositionPtr;

		IndexData*                      mIndexData;
		HardwareIndexBufferSharedPtr    mIndexBuffer;
		size_t                          mIndexStart;
		size_t                          mIndexCount;
		size_t                          mIndexSize;
		HardwareIndexBuffer::IndexType  mIndexType;
		uint8*                          mIndexPtr;

		VertexData*                     mDestVertexData;
		VertexDataByVertexData          mClonedVertexDataByOriginal;

		size_t                          mTotalNumFaces;
		size_t                          mTotalNumClusters;
		size_t                          mTotalNumSubClusters;
		Vector3                         mTotalClusterSize;
		Vector3                         mTotalSubClusterSize;
	};
};

#endif // ENTITY_CREATOR_FOR_LARGE_MESH_H
