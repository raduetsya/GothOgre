#ifndef GOTHOGRE_BOUNDING_SPHERE_TREE_H
#define GOTHOGRE_BOUNDING_SPHERE_TREE_H

#include "GothOgre_MemoryPoolAllocator.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	/** Common definition inherited by BoundingSphereTree and its listener. */
	class GOTHOGRE_EXPORT BoundingSphereTreeCommonDefs
	{
	public:
		/** Element handle points to a specific element in a bounding sphere tree. */
		typedef void* ElementHandle;

		/** A specific value of element handle, which doesn't point to a element. */
		static const ElementHandle NULL_HANDLE;
	};


	//-------------------------------------------------------------------------
	/** Listener for the later using by class BoundingSphereTree. */
	class BoundingSphereTreeListener : public BoundingSphereTreeCommonDefs
	{
	public:
		/** Called by function BoundingSphereTree::findAll. */
		virtual void enumerateAll(ElementHandle _handle, void* _param) {}

		/** Called by function BoundingSphereTree::findRayIntersects. */
		virtual void enumerateRayIntersects(ElementHandle _handle, Real _distance, void* _param) {}

		/** Called by function BoundingSphereTree::findSphereIntersects. */
		virtual void enumerateSphereIntersects(ElementHandle _handle, Real _distance, void* _param) {}

		/** Called by function BoundingSphereTree::findFrustumIntersects. */
		virtual void enumerateFrustumIntersects(ElementHandle _handle, Real _distance, void* _param) {}
	};


	//-------------------------------------------------------------------------
	/** An internal prototype for the class BoundingSphereTree. 
	It's not recommended to use it directly; 
	use the template BoundingSphereTree instead. */
	class GOTHOGRE_EXPORT BaseBoundingSphereTree : public BoundingSphereTreeListener
	{
	protected:
		struct BaseNode;
			struct Node;
			struct Leaf;

		// List of children of a node
		class Children : public std::list<BaseNode*, MemoryPoolAllocator<BaseNode*>>
		{
		public:
			Children()
				: std::list<BaseNode*, MemoryPoolAllocator<BaseNode*>>(ms_Allocator) {}

			static MemoryPoolAllocator<BaseNode*> ms_Allocator;
		};

		// Common prototype for for class "Node" and class "Leaf"..
		struct BaseNode
		{
			Node*               parent;
			Children::iterator  iteratorInParent;
			Vector3             center;
			Real                radius;
			size_t              level;
			bool                isLeaf;
		};

		// Node - contains other nodes or leaves
		struct Node : public BaseNode
		{
			Node() {isLeaf = false;}
			Children  children;
		};

		// Leaf - contains data
		struct Leaf : public BaseNode
		{
			Leaf() {isLeaf = true;}
		};

		Node*                    mRoot;
		vector<Real>::type       mMaxRadiusList;
		vector<Real>::type       mMaxRadiusList2;
		size_t                   mNumLevels;
		size_t                   mNumNodes;
		size_t                   mNumLeaves;
		vector<size_t>::type     mNumNodesByLevel;
		vector<size_t>::type     mNumLeavesByLevel;

		// Statistics
		vector<size_t>::type     mMaxNodesByLevel;
		vector<size_t>::type     mMaxLeavesByLevel;
		vector<long long>::type  mRiNumChecksByLevel;
		size_t                   mRiCount;
		vector<long long>::type  mSiNumChecksByLevel;
		size_t                   mSiCount;
		vector<long long>::type  mFiNumChecksByLevel;
		size_t                   mFiCount;

	protected:
		// Initialisation (called from the constructor)
		void init();

		// Creates and initialise the root node
		Node* createRootNode();

		// Destroys the sphere tree starting from a specified node.
		void destroyTree(Node* _node);

		// Inserts a leaf to the sphere tree, finding a position for the leaf
		// starting from a specified node.
		void insertLeafToTree(Leaf* _leaf, Node* _node);

		// Recursive function, called by the 'insertLeafToTree' function
		void insertLeafToTree_r(Leaf* _leaf, Node* _node);

		// Finds a new position in the sphere tree for a leaf
		// after changing its position or its radius.
		void reinsert(Leaf* _leaf);

		// Removes a leaf from the sphere tree, 
		// the function doesn't destroy the leaf
		void removeLeafFromTree(Leaf* _leaf);

		// Destroys unused nodes (nodes without leaves),
		// checking the specifing node, then its parent, 
		// then the parent of its parent, and so on.
		void destroyUnusedNodes(Node* _node);

		// Internal function to set list of maximum radiuses
		void _setMaxRadiusList(const vector<Real>::type& _lst);

		// Rebuilds the entire tree, the function can be slowly.
		void rebuildTree();

		// Called by the 'rebuildTree' function.
		void moveLeavesFromFirstToSecond(Node* _firstNode, Node* _secondRoot);

	protected:
		struct FindAll
		{
			BoundingSphereTreeListener* listener;
			void*                       param;
			vector<size_t>::type        numChecksByLevel;
		};

		struct FindRayIntersects : public FindAll
		{
			Vector3       rayOrigin;
			Vector3       rayDirection;
			Real          maxDistance;
		};

		struct FindSphereIntersects : public FindAll
		{
			Vector3       sphereCenter;
			Real          sphereRadius;
		};

		struct LinearFunc
		{
			Real a, b; // y = a * x + b
		};

		typedef vector<std::pair<Real, LinearFunc>>::type LinearFuncByDistance;

		struct FindFrustumIntersects : public FindAll
		{
			Plane                 directionPerpPlane;
			Plane                 clippingPlanes[5]; // near, left, right, top, bottom
			Real                  farClipDistance;
			LinearFuncByDistance  linearFuncByDistance;
		};

		// Recursive versions of search routines.
		void findAll_r(FindAll& _fa, BaseNode* _basenode);
		void findRayIntersects_r(FindRayIntersects& _fri, BaseNode* _basenode);
		void findSphereIntersects_r(FindSphereIntersects& _fsi, BaseNode* _basenode);
		void findFrustumIntersects_r(FindFrustumIntersects& _ffi, BaseNode* _basenode);

		// Self-listener
		void enumerateAll(ElementHandle _handle, void* _param);
		void enumerateRayIntersects(ElementHandle _handle, Real _distance, void* _param);
		void enumerateSphereIntersects(ElementHandle _handle, Real _distance, void* _param);
		void enumerateFrustumIntersects(ElementHandle _handle, Real _distance, void* _param);

	protected:
		/** Create a node at a specified level */
		Node* createNode(size_t _level);

		/** Destroy a node */
		void destroyNode(Node* _node);

		/** Pool allocator for nodes. */
		static MemoryPoolAllocator<Node> ms_NodeAlloc;

		/** Create a leaf at level 0 */
		Leaf* createLeaf();

		/** Destroy a leaf */
		void destroyLeaf(Leaf* _leaf);

		/** Change a leaf's level */
		void changeLeafLevel(Leaf* _leaf, size_t _newLevel);

		/** Virtual function to generate leaves. */
		virtual Leaf* createLeafImpl() = 0;

		/** Virtual function to destroy leaves. */
		virtual void destroyLeafImpl(Leaf* _leaf) = 0;

	public:
		BaseBoundingSphereTree();
		virtual ~BaseBoundingSphereTree();

		/** Vector of floating point numbers. */
		typedef vector<Real>::type RealList;

		/** Vector of pairs of floating point numbers. */
		typedef vector<std::pair<Real, Real>>::type RealPairList;

		/** Sets list of maximum radii of nodes.
		When you insert data to an object of this class,
		nodes can grow until maximum radius has been reached. 
		Calling of this function causes rebuilding of the entire tree,
		and can be slowly if the tree contains lots of elements. */
		void setMaxRadiusList(const RealList& _maxRadiusList);
		
		/** Returns list of maximum radii. */
		const RealList& getMaxRadiusList() const;

		/** Returns the depth of the bounding sphere tree. */
		size_t getDepth() const;

		/** Inserts an element into the bounding sphere tree. */
		ElementHandle add(const Vector3& _center, Real _radius);
		
		/** Removes an element from the bounding sphere tree. */
		void remove(ElementHandle _handle);

		/** Removes all the elements from the bounding sphere tree. */
		void clear();

		/** Returns the number of the elements in the bounding sphere tree. */
		size_t size() const;

		/** Returns true if the bounding sphere tree doesn't contains elements. */
		bool empty() const;

		/** Returns the center of an element. */
		const Vector3& getCenter(ElementHandle _handle);

		/** Sets the center of an element. */
		void setCenter(ElementHandle _handle, const Vector3& _newcenter);

		/** Returns the radius of an element. */
		Real getRadius(ElementHandle _handle) const;

		/** Sets the radius of an element. */
		void setRadius(ElementHandle _handle, Real _newradius);


		/** Vector of elements' handles. */
		typedef vector<ElementHandle>::type ElementHandleVector;

		/** Calls function "enumerateAll" for all the elements. */
		void findAll(BoundingSphereTreeListener* _listener, void* _param = nullptr);

		/** Calls function "enumerateAll" for all the elements. */
		ElementHandleVector findAll();


		/** Information about a ray for the "findRayIntersections" function. */
		struct GOTHOGRE_EXPORT RayInfo
		{
			RayInfo(const Ray& _ray);
			RayInfo(const Ray& _ray, Real _maxDistance);

			// Ray used to raycast
			Ray   ray;

			// Maximum distance along the ray;
			// the callback is not called for objects which are farther 
			// than that distance from the ray's origin.
			// The default value is Math::POS_INFINITY.
			Real  maxDistance;
		};

		/** Calls function "enumerateRayIntersects" for all the elements
		which intersect a specified ray. */
		void findRayIntersects(const RayInfo& _rayInfo, BoundingSphereTreeListener* _listener, void* _param = nullptr);

		/** Calls function "enumerateRayIntersects" for all the elements
		which intersect a specified ray. */
		ElementHandleVector findRayIntersects(const RayInfo& _rayInfo);


		/** Information about a sphere for the "findRayIntersections" function. */
		struct GOTHOGRE_EXPORT SphereInfo
		{
			SphereInfo(const Sphere& _sphere);

			Sphere  sphere;
		};

		/** Calls function "enumerateSphereIntersects" for all the elements
		which intersect a specified sphere. */
		void findSphereIntersects(const SphereInfo& _sphere, BoundingSphereTreeListener* _listener, void* _param = nullptr);

		/** Calls function "enumerateSphereIntersects" for all the elements
		which intersect a specified sphere. */
		ElementHandleVector findSphereIntersects(const SphereInfo& _sphereInfo);


		/** Information about a frustum for the "findRayIntersections" function. */
		struct GOTHOGRE_EXPORT FrustumInfo
		{
			FrustumInfo(const Frustum* _frustum);
			FrustumInfo(const Frustum* _frustum, Real _farClipDistance);
			FrustumInfo(const Frustum* _frustum, const RealPairList& _distanceMinRadiusPairList);
			FrustumInfo(const Frustum* _frustum, Real _farClipDistance, const RealPairList& _distanceMinRadiusPairList);
			FrustumInfo(const Frustum* _frustum, const RealPairList& _distanceMinRadiusPairList, Real _farClipDistance);

			/** Pointer to a frustum. */
			const Frustum*  frustum;

			/** Maximum distance from the frustum's position.
			The callback is not called for objects which are farther than the distance
			from the frustum's position. 
			The default value is equal to the frustum's far clipping distance. */
			Real            farClipDistance;

			/** List of (x,y) coordinates, describing some function,
			where x means distance from the frustum's position, 
			and y means minimum radius for an objects at this distance 
			to be visible. The function is linearly interpolated
			between points specified in this list. 
			The default value is empty list, 
			which means "don't cull objects by radius". */
			RealPairList    distanceMinRadiusPairList;
		};

		/** Calls function "enumerateSphereIntersects" for all the elements
		which intersect a specified frustum. */
		void findFrustumIntersects(const FrustumInfo& _frustumInfo, BoundingSphereTreeListener* _listener, void* _param = nullptr);

		/** Calls function "enumerateSphereIntersects" for all the elements
		which intersect a specified frustum. */
		ElementHandleVector findFrustumIntersects(const FrustumInfo& _frustumInfo);


		/** Prints statistics into log file. */
		void logStatistics(const String& _name);
	};








	//-------------------------------------------------------------------------
	/** Dynamic tree of bounding sphere volumes.
	It's used for many purposes: for example, a fast search of 
	all visible objects in the viewing frustum. */
	template<typename T>
	class BoundingSphereTree : public BaseBoundingSphereTree
	{
	protected:
		struct NewLeaf : public Leaf
		{
			T data;
		};

		// Pool allocator to make operations faster.
		static MemoryPoolAllocator<NewLeaf> ms_NewLeafAlloc;

		// Implement virtual functions
		Leaf* createLeafImpl()
		{
			NewLeaf* p = ms_NewLeafAlloc.construct_new();
			return p;
		}

		void destroyLeafImpl(Leaf* _leaf)
		{
			ms_NewLeafAlloc.del( (NewLeaf*)_leaf );
		}

	public:
		BoundingSphereTree() {}
		~BoundingSphereTree() {clear();}

		/** Inserts an element into the bounding sphere tree. 
		Mask can be used later in search operations. */
		ElementHandle add(const Vector3& _center, Real _radius, T _data)
		{
			ElementHandle handle = BaseBoundingSphereTree::add(_center, _radius);
			setData(handle, _data);
			return handle;
		}

		/** Removes an element from the bounding sphere tree. */
		T remove(ElementHandle _handle)
		{
			T data = getData(_handle);
			BaseBoundingSphereTree::remove(_handle);
			return data;
		}

		/** Sets data linked to a specified element. */
		void setData(ElementHandle _handle, T _data)
		{
			NewLeaf* leaf = (NewLeaf*)_handle;
			leaf->data = _data;
		}

		/** Returns data linked to a specified element. */
		T getData(ElementHandle _handle) const
		{
			NewLeaf* leaf = (NewLeaf*)_handle;
			return leaf->data;
		}
	};
	//-----------------------------------------------------------------------
	template<typename T>
	MemoryPoolAllocator<typename BoundingSphereTree<T>::NewLeaf> 
		BoundingSphereTree<T>::ms_NewLeafAlloc(2048);


} // namespace GothOgre

#endif // GOTHOGRE_BOUNDING_SPHERE_TREE_H