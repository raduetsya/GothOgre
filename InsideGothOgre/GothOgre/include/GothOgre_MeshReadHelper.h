#ifndef GOTHOGRE_MESH_READ_HELPER_H
#define GOTHOGRE_MESH_READ_HELPER_H


namespace GothOgre
{
	//----------------------------------------------------------------------
	/** Helper classes to retrieve information about faces and vertices from a mesh.
	The function of the standard Ogre's "Mesh" class is not suitable for this purpose. */
	class GOTHOGRE_EXPORT MeshReadHelper
	{
	public:
		MeshReadHelper();
		MeshReadHelper(const MeshPtr& _msh);
		~MeshReadHelper();

		/** Sets the mesh to retrieve information from. */
		void setMesh(const MeshPtr& _mesh);

		/** Returns the mesh. */
		MeshPtr getMesh() const;

		/** Unlocks all hardware buffers, which have been 
		(internally) locked by this class instance. */
		void unlockAllBuffers();

		/** Returns the total number of the vertices in this mesh. */
		size_t getNumVertices() const;

		/** Returns the coordinates of a vertex by an index, which is in range [0..getNumVertices() - 1]. */
		Vector3 getVertex(size_t _vertexIndex);

		/** Returns true if this mesh contains texture coordinates. */
		bool hasTextureCoords();

		/** Returns the texture coordinates of a vertex by its index. */
		Vector2 getTextureCoords(size_t _vertexIndex);

		/** Returns the number of the submeshes. */
		size_t getNumSubMeshes() const;

		/** Returns the material of a submesh. */
		const String& getMaterialName(size_t _subMeshIndex);

		/** Returns the number of the faces in a submesh. */
		size_t getNumFaces(size_t _subMeshIndex);

		/** A single triangular face. */
		struct Face
		{
			union
			{
				struct
				{
					size_t i0, i1, i2;
				};
				struct
				{
					size_t i[3];
				};
			};
		};

		/** Returns the indices of the vertices of a face
		by the index of the face in a submesh. */
		Face getFace(size_t _subMeshIndex, size_t _faceIndex);

	private:
		// Initialisation (called by the constructor only).
		void init();

		// Calculates mSubMeshInfos & mNumVertices for a new mesh
		void calculateMeshInfo();

		// Resets pointers for fast access (mCurSubMeshInfoV, mCurSubMeshInfoT, 
		// mCurSubMeshInfoF, mCurSubMeshInfoFi) to default values.
		void resetCurPointers();

		// Locks the vertex buffer (if it's necessary),
		// and returns offset of a specified vertex in the current submesh's vertex data.
		size_t prepareVerticesGetOffset(size_t _vertexIndex);

		// Locks the texture coordinates' buffer (if it's necessary),
		// and returns offset of a specified vertex in the current submesh's vertex data.
		size_t prepareTexCoordsGetOffset(size_t _vertexIndex);
		
		// Locks the index buffer (if it's necessary).
		void prepareFaces(size_t _subMeshIndex);

		void lockVertexBuffer();
		void unlockVertexBuffer();
		void lockTexCoordBuffer();
		void unlockTexCoordBuffer();
		void lockIndexBuffer();
		void unlockIndexBuffer();

	private:
		MeshPtr                         mMesh;

		struct SubMeshInfo
		{
			SubMesh*                    subMesh;
			VertexData*                 vertexData;
			size_t                      vertexStart;
			size_t                      vertexCount;
			size_t                      vertexStartInJoinedList;
			IndexData*                  indexData;
			size_t                      indexStart;
			size_t                      indexCount;
		};
		vector<SubMeshInfo>::type       mSubMeshInfos;
		size_t                          mNumVertices;
		SubMeshInfo*                    mCurSubMeshInfoV;
		SubMeshInfo*                    mCurSubMeshInfoT;
		SubMeshInfo*                    mCurSubMeshInfoF;
		size_t                          mCurSubMeshInfoFi;

		IndexData*                      mIndexData;
		HardwareIndexBufferSharedPtr    mIndexBuffer;
		size_t                          mIndexStart;
		size_t                          mIndexCount;
		size_t                          mIndexSize;
		HardwareIndexBuffer::IndexType  mIndexType;
		const void*                     mIndexPtr;

		VertexData*                     mVertexData;
		HardwareVertexBufferSharedPtr   mVertexBuffer;
		size_t                          mVertexSize;
		size_t                          mVertexOffset;
		VertexElementType               mVertexType;
		const void*                     mVertexPtr;

		VertexData*                     mTexCoordData;
		HardwareVertexBufferSharedPtr   mTexCoordBuffer;
		size_t                          mTexCoordStart;
		size_t                          mTexCoordSize;
		size_t                          mTexCoordOffset;
		VertexElementType               mTexCoordType;
		const void*                     mTexCoordPtr;
	};


} // namespace GothOgre

#endif // GOTHOGRE_MESH_READ_HELPER_H