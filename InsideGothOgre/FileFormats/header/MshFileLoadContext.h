#ifndef MSH_FILE_LOAD_CONTEXT_H
#define MSH_FILE_LOAD_CONTEXT_H

namespace GothOgre
{
	class MshFileLoadContext : public ResourceLoadContext
	{
	public:
		MshFileLoadContext();
		~MshFileLoadContext();

		void load(const DataStreamExPtr& _stream);
		void apply(Resource* _resource);
		void skip(const DataStreamExPtr& _stream);

	protected:
		#pragma pack(push, 1)

		class Chunk
		{
		public:
			Chunk(const DataStreamExPtr& _stream);
			~Chunk();

			unsigned short getID() const {return mChunkID;}
			size_t getPos() const {return mChunkPos;}
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

		struct Date
		{
			int		year;
			short	month, day, hour, minute, second;
			short	unknown;
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

		struct AABB
		{
			Vec3 min;
			Vec3 max;
		};

		struct Plane
		{
			float	d;
			Vec3	normal;
		};

		struct Color
		{
			uchar b, g, r, a;
		};

		struct VertFeature
		{
			UVMapping	uvMapping;
			Color		color;
			Vec3		normal;
		};

		enum PortalType : uint8
		{
			None,
			Small,
			Big,
			PN
		};

		struct PolyFlags
		{
			union 
			{
				struct
				{
					PortalType	type:2;			//0x03
					uint8		occluder:1;		//4
					uint8		indoor:1;			//8
					uint8		lodGenerate:1;		//10
					uint8		portalWithoutMat:1;//20
					uint8		ghost:1;			//40
				};
				uint8 allbits;
			};
		};

		struct PolyVert
		{
			ulong	vertexIndex;
			ulong	vertFeatureIndex;
		};

		struct Polygon
		{
			ushort		matIndex;
			ushort		lightIndex;
			Plane		plane;
			PolyFlags	flags;
			char		unknown2[2]; // ?
			uchar		numPolyVerts;
			PolyVert	polyVerts[10];
		};

		struct PolyVert_G1 // old format (Gothic 1)
		{
			ushort	vertexIndex;
			ulong	vertFeatureIndex;
		};

		struct Polygon_G1 // old format (Gothic 1)
		{
			ushort		matIndex;
			ushort		lightIndex;
			Plane		plane;
			PolyFlags	flags;
			char		unknown2[3]; // ?
			uchar		numPolyVerts;
			PolyVert_G1	polyVerts[10];
		};

		#pragma pack(pop)

		struct SubMeshInfo
		{
			MtlPtr          mtl;
			MatGenParams    matGenParams;
			size_t          firstIndex;
			size_t          numIndices;
		};

	protected:
		time_t readDate();
		AxisAlignedBox readAxisAlignedBox();
		void readFileInfo(Chunk& _parentChunk);
		void readBounds(Chunk& _parentChunk);
		void readMaterials(Chunk& _parentChunk);
		void readLightMaps(Chunk& _parentChunk);
		void readLightMapIndices(Chunk& _parentChunk);
		void readVertices(Chunk& _parentChunk);
		void readVertFeatures(Chunk& _parentChunk);
		void readPolygons(Chunk& _parentChunk);
		void loadFromDataStream(const DataStreamPtr& _dataStream);

		void createMaterials();
		void calculateSubMeshInfos();
		void calculateBounds();
		void createSharedVertices();
		void createSharedIndices();
		void createSubMeshes();
		void setBounds();
		void clear();

	protected:
		DataStreamExPtr				mDataStream;
		MeshEx*						mMesh;
		ushort						mMshVersion; 
		time_t						mCreationDate;
		String						mSourceFilename;
		AxisAlignedBox				mBounds;

		// temporary data stream in memory with material data
		DataStreamExPtr				mMemoryStreamWithMaterials;

		// vector of vertices
		vector<Vec3>::type			mVertices;

		// vector of vert. features
		vector<VertFeature>::type	mVertFeatures;

		// vector of pointers to polygon buffer
		vector<Polygon*>::type		mPolygons;

		// polygon buffer
		vector<char>::type			mPolygonBuffer;

		// temporary buffer which is used only for converting polygons 
		// from old Gothic 1 format
		vector<char>::type			mPoly1Buffer;

		// vector of submesh infos
		vector<SubMeshInfo>::type	mSubMeshInfos;

		// total number of indices (for all submeshes)
		size_t                      mTotalNumIndices;

		// shared vertex data
		HardwareVertexBufferSharedPtr mSharedVertexBuffer;

		// shared index buffer
		HardwareIndexBufferSharedPtr  mSharedIndexBuffer;
	};
}

#endif // MSH_FILE_LOAD_CONTEXT_H