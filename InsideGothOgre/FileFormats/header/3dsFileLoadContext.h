#ifndef _3DS_FILE_LOAD_CONTEXT_H
#define _3DS_FILE_LOAD_CONTEXT_H


namespace GothOgre
{

	class _3dsFileLoadContext : public ResourceLoadContext
	{
	public:
		_3dsFileLoadContext();
		~_3dsFileLoadContext();

		void load(const DataStreamExPtr& _dataStream);
		void apply(Resource* _resource);
		void skip(const DataStreamExPtr& _stream);

	protected:
		#pragma pack(push, 1)

		class Chunk
		{
		public:
			Chunk(const DataStreamExPtr& _stream);
			Chunk(Chunk& _parentChunk);
			~Chunk();

			unsigned short getID() const {return mChunkID;}
			size_t getPos() const {return mChunkPos;}
			size_t getSize() const {return mChunkSize;}
			
			size_t getDataSize() const {return mDataSize;}
			void setDataSize(size_t _dataSize);
			
			bool hasMoreSubchunks() const {return (long) mRestSize > 0;}
			Chunk* getParent() const {return mParentChunk;}

		private:
			void readHeader();
			void skipRest();

		private:
			ushort			mChunkID;   // don't change order of
			size_t          mChunkSize; // these two member variables
			size_t          mDataSize;
			size_t          mRestSize;
			size_t          mChunkPos;
			DataStreamExPtr	mDataStream;
			Chunk*          mParentChunk;
		};

		// uv mapping as it is written to 3ds file.
		struct UVMapping
		{
			float u, v;
		};

		// Vertex as it is written to 3ds file.
		struct Vec3
		{
			float x, y, z;

			// I had no purpose to define all the "standard" vector operations.
			// I'd defined only operations which are used by the _3dsFileLoader class.
			Vec3() {}
			Vec3(float _x, float _y, float _z) {x = _x; y = _y; z = _z;}
			Vector3 toVector3() const {return Vector3( Real(x), Real(y), Real(z) );}

			float length() const {return sqrt(x * x + y * y + z * z);}
			Vec3 crossProduct(const Vec3& b) const { return Vec3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);}
			const Vec3& operator /=(float f) {x /= f; y /= f; z /= f; return *this;}
			const Vec3& operator +=(const Vec3& b) {x += b.x; y += b.y; z += b.z; return *this;}
			friend Vec3 operator -(const Vec3& a, const Vec3& b) {return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);}
		};

		// Face as it is written to 3ds file.
		struct Face
		{
			// Indices of vertices in array "Object::vertices" used by the face.
			// The indices are 16-bit, so one mesh can use up to 65535 vertices.
			ushort v[3];

			// Flags describe edge visibility; not used in the loader.
			ushort flags;
		};

		// A material name and list of faces which use the material.
		struct FacesMtl
		{
			// Indices of faces in array "Object::faces"
			// The indices are 16-bit, so one mesh can use up to 65535 faces.
			vector<ushort>::type	faceIndices;

			// The name of material.
			String					materialName;
		};

		#pragma pack(pop)

		struct VertFeature;

		// Vert. info - information about one vertex.
		struct VertInfo
		{
			// The pointer to vertex position.
			// This is a pointer to an element of array "Object::vertices".
			Vec3*			pos;

			// The pointer to vertex texture coordinates.
			// This is a pointer to an element of array "Object::tVerts".
			UVMapping*		uv;

			// One vertex can correspond up to several vert. features.
			// This is a pointer to the first one.
			// A vertex can have several normals at the same time
			// (different normals in different faces),
			// so one vertex can correspond up to several vert. features
			// (one vert. feature stores one normal).
			// This is a pointer to an element of array "Object::vertFeatures".
			VertFeature*	firstVertFeature;
		};

		// Vert. feature.
		struct VertFeature
		{
			// Pointer to the next vert. feature, corresponding to the same vertex.
			// This is a pointer to an element of array "Object::vertFeatures".
			VertFeature*	nextVertFeature;
			
			// Pointer to the parent vertex info.
			// This is a pointer to an element of array "Object::vertInfos".
			VertInfo*		vertInfo;

			// Calculated normal.
			Vec3			normal;

			// The following fields are temporary data
			// used only when calculating normals.
			ulong			numJoined;
			bool			useSmoothGroup;
			ulong			smoothGroup;
			float			cosSmoothAngle;
		};

		// Information to create one submesh - material and indices.
		struct SubMeshInfo
		{
			// Material.
			MtlPtr			mtl;

			// Pointer to the first index in the submesh.
			// This is a pointer to an element of array "Object::indexBuffer".
			ulong*			indices;

			// Number of indices in the submesh.
			// This is a number of sequence of elements of array "Object::indexBuffer",
			// used in the submesh.
			ulong			numIndices;
		};

		// One object (a single 3ds file can contain up to several objects).
		struct Object
		{
			// section 1 - read from file
			String					objectName;
			vector<Vec3>::type		vertices;
			vector<UVMapping>::type	tVerts;
			vector<Face>::type		faces;
			vector<ulong>::type		smoothGroups;
			list<FacesMtl>::type	facesMtls;

			// section 2 - calculated by calculateSubMeshInfos function
			vector<VertInfo>::type		vertInfos;
			vector<VertFeature>::type	vertFeatures;
			vector<SubMeshInfo>::type	subMeshInfos;
			vector<ulong>::type			indexBuffer;

			// section 3 - calculated by calculateBounds function
			AxisAlignedBox			bounds;
		};

		struct MaterialMap
		{
			String       mappingFileName;
			// There are other map properties described in the 3DS file format specification.
		};

		enum MaterialMapType
		{
			TextureMap1 = 0xA200
			// There are other map types described in the 3DS file format specification.
		};

		// Material as it is written to 3ds file.
		struct MaterialInfo
		{
			String			materialName;
			ColourValue		diffuseColor;
			map<MaterialMapType, MaterialMap>::type	maps;
			// There are other material properties described in the 3DS file format specification.
		};

	protected:
		ColourValue readColor(Chunk& _parentChunk);
		float readPercent(Chunk& _parentChunk);
		void read3dsVersion(Chunk& _parentChunk);
		void readMeshVersion(Chunk& _parentChunk);
		void readMaterialMap(Chunk& _parentChunk);
		void readMaterialBlock(Chunk& _parentChunk);
	
		void readVertices(Chunk& _parentChunk);
		void readMappingCoords(Chunk& _parentChunk);
		void readSmoothGroups(Chunk& _parentChunk);
		void readFacesDescription(Chunk& _parentChunk);
		void readFacesMtl(Chunk& _parentChunk);
		void readMesh(Chunk& _parentChunk);
		void readObjectBlock(Chunk& _parentChunk);
		void read3dEditor(Chunk& _parentChunk);
		void readMainChunk(Chunk& _parentChunk);
		void loadFromDataStream(const DataStreamExPtr& _dataStream);

		void createMtls();
		MtlPtr createMtlByMaterialInfo(MaterialInfo* _matInfo);
		void calculateSubMeshInfos();
		void calculateSubMeshInfos(Object* obj);
		bool canJoin(const VertFeature* _vertFeature1, const VertFeature* _vertFeature2);
		void join(VertFeature* _vertFeature1, const VertFeature* _vertFeature2);
		void calculateBounds();
		void calculateBounds(Object* obj);
		void createSharedVertices();
		void createSubMeshes();
		void setBounds();
		void insertIntoRenderSystem(Resource* _destMesh);

		void clear();

	protected:
		typedef list<MaterialInfo>::type MaterialInfos;
		typedef list<Object>::type Objects;
		typedef list<MtlPtr>::type Mtls;

		DataStreamExPtr			 mDataStream;
		MeshEx*					 mMesh;
		ulong					 m3dsVersion;
		ulong					 mMeshVersion;
		MaterialInfos            mMaterialInfos;
		MaterialInfo*			 mCurrentMaterialInfo;
		MaterialMap*			 mCurrentMaterialMap;
		Mtls                     mMtls;
		Objects					 mObjects;
		Object*					 mCurrentObject;
		AxisAlignedBox			 mBounds;
	};

} // namespace GothOgre

#endif // _3DS_FILE_LOAD_CONTEXT_H