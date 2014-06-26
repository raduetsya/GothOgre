#ifndef GOTHOGRE_BSP_H
#define GOTHOGRE_BSP_H

#include "GothOgre_ResourceSubclassPtr.h"
#include "GothOgre_ResourceLoader.h"
#include "GothOgre_ResourceSaver.h"


namespace GothOgre
{
	/** New resource type: Bsp.
	Contains a BSP-tree, because a compiled zen stores level information 
	as BSP-tree. */
	class GOTHOGRE_EXPORT Bsp : public Resource
	{
	public:
		/** Vertex of the mesh with normal and texture coordinates. */
		class GOTHOGRE_EXPORT Vertex
		{
		public:
			Vertex() {}

			void setPosition(const Vector3& _position) {mPosition = _position;}
			const Vector3& getPosition() const {return mPosition;}

			void setUVMapping(const Vector2& _uvMapping) {mUVMapping = _uvMapping;}
			const Vector2& getUVMapping() const {return mUVMapping;}

			void setNormal(const Vector3& _normal) {mNormal = _normal;}
			const Vector3& getNormal() const {return mNormal;}

		private:
			Vector3 mPosition;
			Vector2 mUVMapping;
			Vector3 mNormal;
		};

	public:
		/** Triangular face of the mesh with a pointer to used material. */
		class GOTHOGRE_EXPORT Face
		{
		public:
			Face() {}

			void setVertexIndex(size_t _indexInTriangle, size_t _indexInMesh) {mVertexIndices[_indexInTriangle] = _indexInMesh;}
			size_t getVertexIndex(size_t _indexInTriangle) const {return mVertexIndices[_indexInTriangle];}

			void setMaterialIndex(size_t _matIndex) {mMaterialIndex = _matIndex;}
			size_t getMaterialIndex() const {return mMaterialIndex;}

		private:
			size_t		mVertexIndices[3];
			size_t		mMaterialIndex;
		};

	public:
		class GOTHOGRE_EXPORT NodeBase
		{
		public:
			NodeBase() {}
			~NodeBase() {}

			bool isLeaf() const {return mIsLeaf;}
			bool isNode() const {return !mIsLeaf;}

			void setBoundingBox(const AxisAlignedBox& _boundingBox) {mBoundingBox = _boundingBox;}
			const AxisAlignedBox& getBoundingBox() const {return mBoundingBox;}

		protected:
			bool			mIsLeaf;

			// ќграничивающий параллелепипед
			AxisAlignedBox	mBoundingBox;
		};

		class Leaf;

		class GOTHOGRE_EXPORT Node : public NodeBase
		{
		public:
			Node() {mIsLeaf = false; mFront = nullptr; mBack = nullptr;}
			~Node() {destroyFront(); destroyBack();}

			void setSeparationPlane(const Plane& _plane) {mSeparationPlane = _plane;}
			const Plane& getSeparationPlane() const {return mSeparationPlane;}

			NodeBase* getFront() const {return mFront;}
			Node* createFrontNode();
			Leaf* createFrontLeaf();
			void destroyFront();
			
			NodeBase* getBack() const {return mBack;}
			Node* createBackNode();
			Leaf* createBackLeaf();
			void destroyBack();

		protected:
			// –азлел€юща€ плоскость, использованна€ дл€
			// разделени€ этой BspNode на Front и Back.
			Plane			mSeparationPlane;

			// ѕерва€ подветвь BSP-дерева: узлы спереди от раздел€ющей плоскости.
			NodeBase*		mFront;

			// ¬тора€ подветвь BSP-дерева: узлы сзади от раздел€ющей плоскости.
			NodeBase*		mBack;
		};

		class GOTHOGRE_EXPORT Leaf : public NodeBase
		{
		public:
			Leaf() {mIsLeaf = true;}
			~Leaf() {}

			void setFirstFaceIndex(size_t _firstFaceIndex) {mFirstFaceIndex = _firstFaceIndex;}
			size_t getFirstFaceIndex() const {return mFirstFaceIndex;}

			void setNumFaceIndices(size_t _numFaceIndices) {mNumFaceIndices = _numFaceIndices;}
			size_t getNumFaceIndices() const {return mNumFaceIndices;}

		protected:
			// ѕервый индекс грани, т.е. 
			// номер первого элемента в массиве Bsp::mFaceIndices,
			// используемого при отображении этой Bsp::Node.
			size_t			mFirstFaceIndex;

			// „исло индексов граней, т.е.
			// число элементов в массиве Bsp::mFaceIndices,
			// используемых при отображении этой Bsp::Node.
			size_t			mNumFaceIndices;
		};

	public:
		Bsp(Ogre::ResourceManager* _creator, const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual = false, ManualResourceLoader* _loader = 0);
		virtual ~Bsp();

		// We need implement all the pure virtual functions from the base class
		virtual void loadImpl();
		virtual void unloadImpl();
		
		/** Calculate the size of a resource; 
		this will only be called after 'load' */
		virtual size_t calculateSize() const;

		// Operations with vertices
		void setNumVertices(size_t _numVertices) {mVertices.resize(_numVertices);}
		size_t getNumVertices() const {return mVertices.size();}
		const Vertex& getVertex(size_t _index) const {return mVertices[_index];}
		Vertex& getVertex(size_t _index) {return mVertices[_index];}
		void setVertex(size_t _index, const Vertex& _vertex) {mVertices[_index];}

		// Operation with materials
		void setNumMaterials(size_t _numMaterials) {mMaterials.resize(_numMaterials);}
		size_t getNumMaterials() const {return mMaterials.size();}
		const MaterialPtr& getMaterial(size_t _index) const {return mMaterials[_index];}
		void setMaterial(size_t _index, const MaterialPtr& _material) {mMaterials[_index] = _material;}

		// Operations with faces
		void setNumFaces(size_t _numFaces) {mFaces.resize(_numFaces);}
		size_t getNumFaces() const {return mFaces.size();}
		const Face& getFace(size_t _index) const {return mFaces[_index];}
		Face& getFace(size_t _index) {return mFaces[_index];}
		void setFace(size_t _index, const Face& _face) {mFaces[_index] = _face;}

		// Operation with face indices
		void setNumFaceIndices(size_t _numFaceIndices) {mFaceIndices.resize(_numFaceIndices);}
		size_t getNumFaceIndices() const {return mFaceIndices.size();}
		size_t getFaceIndex(size_t _index) const {return mFaceIndices[_index];}
		size_t& getFaceIndex(size_t _index) {return mFaceIndices[_index];}
		void setFaceIndex(size_t _index, size_t _faceIndex) {mFaceIndices[_index] = _faceIndex;}

		/** Returns a pointer to the root BSP node. */
		Node* getRootNode() const {return mRootNode;}

		/** Calculate the number of BSP nodes and BSP leafs and the maximum depth. */
		void calculateStats();

		/** Returns the number of BSP nodes. */
		size_t getNumNodes() const;

		/** Returns the number of BSP leafs. */
		size_t getNumLeaves() const;

		/** Returns the maximum depth of BSP tree. */
		size_t getMaxDepth() const;

	private:
		/** Calculate the number of BSP nodes starting from the specified node. */
		void calculateStats(NodeBase* _nodeBase, size_t _depth);

		static Node* createNode();
		static Leaf* createLeaf();
		static void destroyNode(Node* _node);
		static void destroyLeaf(Leaf* _node);

	private:
		vector<MaterialPtr>::type	mMaterials;
		vector<Vertex>::type		mVertices;
		vector<Face>::type			mFaces;
		vector<size_t>::type		mFaceIndices;
		Node*						mRootNode;
		size_t						mNumNodes;
		size_t						mNumLeaves;
		size_t						mMaxDepth;
	};


	//-----------------------------------------------------------------------
	/** Shared pointer to Bsp. */
	typedef ResourceSubclassPtr<Bsp> BspPtr;


	//-----------------------------------------------------------------------
	/** A prototype for future loaders of Bsp resource. */
	class GOTHOGRE_EXPORT BspLoader : public ResourceLoader
	{
	public:
		BspLoader(const String& _name, int _order = Order::NORMAL);
	};


	//-----------------------------------------------------------------------
	/** A prototype for future savers of Bsp resource. */
	class GOTHOGRE_EXPORT BspSaver : public ResourceSaver
	{
	public:
		BspSaver(const String& _name, int _order = Order::NORMAL);
	};


} // namespace GothOgre

#endif // GOTHOGRE_BSP_H