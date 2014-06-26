#ifndef MRM_FILE_LOAD_CONTEXT_H
#define MRM_FILE_LOAD_CONTEXT_H

namespace GothOgre
{
	class MrmFileLoadContext : public ResourceLoadContext
	{
	public:
		MrmFileLoadContext();
		~MrmFileLoadContext();

		void load(const DataStreamExPtr& _stream);
		void apply(Resource* _destResource);
		void skip(const DataStreamExPtr& _stream);

	public:
		struct LodLevel
		{
			Real value;
			Real triangleQuota;

			/** Constructor for convenience */
			LodLevel(Real _value, Real _triangleQuota) : value(_value), triangleQuota(_triangleQuota) {}
		};
		typedef vector<LodLevel>::type LodConfig;

		const LodConfig& getLodConfig() const;
		void setLodConfig(const LodConfig& _lodConfig);

	protected:
		#pragma pack(push, 1)

		class Chunk
		{
		public:
			Chunk(const DataStreamExPtr& _stream);
			~Chunk();

			unsigned short getID() const {return mChunkID;}
			size_t getPos() const {return mChunkPos;}
			size_t getDataPos() const {return mChunkPos + 6;}
			size_t getDataSize() const {return mDataSize;}
			
		private:
			void readHeader();
			void skipRest();

		private:
			ushort			mChunkID;  // don't change order of
			size_t          mDataSize; // these two member variables
			size_t          mChunkPos;
			DataStreamExPtr	mDataStream;
		};

		struct UVMapping
		{
			float u, v;
		};

		struct Vec3
		{
			float x, y, z;
			Vector3 toVector3() const {return Vector3( (Real)x, (Real)y, Real(z) );}
		};

		struct Wedge
		{
			Vec3		normal;
			UVMapping	uvMapping;
			uint16		vertIndex;
			uint16		unused;
		};

		struct Triangle
		{
			uint16	wedgeIndices[3];
		};

		struct Resolution
		{
			uint16	numTriangles;
			uint16	numWedges;
		};

		struct AABB
		{
			Vec3 min;
			Vec3 max;
		};

		struct MeshDecl
		{
			uint8  numMaterials;
			uint32 offsetVertices;
			uint32 numVertices;
			uint32 offsetNormals;
			uint32 numNormals;
		};

		struct SubMeshDecl
		{
			uint32 offsetTriangles;
			uint32 numTriangles;
			uint32 offsetWedges;
			uint32 numWedges;
			uint32 offsetUnknown1;
			uint32 numUnknown1;
			uint32 offsetPlaneIndices;
			uint32 numPlaneIndices;
			uint32 offsetPlanes;
			uint32 numPlanes;
			uint32 offsetLeftByCollapsed;
			uint32 numLeftByCollapsed;
			uint32 offsetResolutions;
			uint32 numResolutions;
			uint32 offsetUnknown2;
			uint32 numUnknown2;
			uint32 offsetUnknown3;
			uint32 numUnknown3;
			uint32 offsetUnknown4;
			uint32 numUnknown4;
		};

		#pragma pack(pop)

		struct SubMeshInfo
		{
			MtlPtr	        mtl;
			Triangle*		triangles;
			size_t			numTriangles;
			Wedge*			wedges;
			size_t			numWedges;
			uint16*			leftByCollapsed;
			size_t			numLeftByCollapsed;
			Resolution*		resolutions;
			size_t			numResolutions;
		};

	protected:
		AxisAlignedBox readAxisAlignedBox();
		void readFileInfo();
		void readMeshData();
		void readMeshDecl();
		void readSubMeshDecls();
		void readAndCreateMaterials();
		void readBounds();
		void loadFromDataStream(const DataStreamExPtr& _dataStream);

		void calculateMeshResolutions();
		size_t findResolution(Real _triangleQuota) const;
		void initLodSubMeshVertices(const Resolution& _resolution, const SubMeshInfo& _info, SubMesh* _subMesh);
		void initLodSubMeshIndices(const Resolution& _resolution, const SubMeshInfo& _info, SubMesh* _subMesh);
		void setBounds(Mesh* _lodMesh);
		void initLodMesh(size_t _resolutionIndex, MeshEx* _lodMesh);
		void initLodMeshes();
		void insertIntoRenderSystem(Resource* _destMesh);

		void clear();

	protected:
		LodConfig					mLodConfig;

		DataStreamExPtr				mDataStream;
		MeshEx*						mMesh;
		
		// Data which is read from file
		ushort						mMrmVersion;
		vector<uint8>::type			mMeshData;
		MeshDecl					mMeshDecl;
		vector<SubMeshDecl>::type	mSubMeshDecls;
		DataStreamPtr				mMemoryStreamWithMaterials;
		AxisAlignedBox				mBounds;
		
		// vector of vertices
		Vec3*						mVertices;
		size_t						mNumVertices;

		// vector of normals
		Vec3*						mNormals;
		size_t						mNumNormals;

		// vector of submesh infos
		vector<SubMeshInfo>::type	mSubMeshInfos;

		// Resolutions for the entire mesh
		vector<Resolution>::type	mMeshResolutions;
	};

} // namespace GothOgre

#endif // MRM_FILE_LOAD_CONTEXT_H