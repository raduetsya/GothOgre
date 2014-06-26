#include "GothOgre_Precompiled.h"
#include "GothOgre_BoundingSphereTree.h"
#include "GothOgre_StdIO.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// Implementation: constants
	//-------------------------------------------------------------------------
	const BoundingSphereTreeCommonDefs::ElementHandle 
		BoundingSphereTreeCommonDefs::NULL_HANDLE = nullptr;

	
	
	//-------------------------------------------------------------------------
	// Implementation: pool allocation
	//-------------------------------------------------------------------------
	MemoryPoolAllocator<BaseBoundingSphereTree::Node> 
		BaseBoundingSphereTree::ms_NodeAlloc(2048);
	
	MemoryPoolAllocator<BaseBoundingSphereTree::BaseNode*> 
		BaseBoundingSphereTree::Children::ms_Allocator(2048);
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::Node* BaseBoundingSphereTree::createNode(size_t _level)
	{
		++mNumNodes;
		++mNumNodesByLevel[_level];
		if(mMaxNodesByLevel[_level] < mNumNodesByLevel[_level])
			mMaxNodesByLevel[_level] = mNumNodesByLevel[_level];

		Node* p = ms_NodeAlloc.construct_new();
		p->level = _level;
		return p;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::destroyNode(BaseBoundingSphereTree::Node* _p)
	{
		size_t level = _p->level;
		--mNumNodesByLevel[level];
		--mNumNodes;
		ms_NodeAlloc.del(_p);
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::Leaf* BaseBoundingSphereTree::createLeaf()
	{
		++mNumLeaves;
		++mNumLeavesByLevel[0];

		Leaf* p = createLeafImpl();
		p->level = 0;
		return p;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::destroyLeaf(BaseBoundingSphereTree::Leaf* _p)
	{
		size_t level = _p->level;
		--mNumLeavesByLevel[level];
		--mNumLeaves;
		destroyLeafImpl(_p);
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::changeLeafLevel(BaseBoundingSphereTree::Leaf* _p, size_t _newLevel)
	{
		size_t oldLevel = _p->level;
		if(oldLevel != _newLevel)
		{
			_p->level = _newLevel;
			--mNumLeavesByLevel[oldLevel];
			++mNumLeavesByLevel[_newLevel];
			if(mMaxLeavesByLevel[_newLevel] < mNumLeavesByLevel[_newLevel])
				mMaxLeavesByLevel[_newLevel] = mNumLeavesByLevel[_newLevel];
		}
	}



	//-------------------------------------------------------------------------
	// Implementation: main operations
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::BaseBoundingSphereTree()
	{
		init();
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::init()
	{
		mNumNodes = 0;
		mNumLeaves = 0;
		mRoot = nullptr;
		_setMaxRadiusList(RealList());
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::~BaseBoundingSphereTree()
	{
		clear();
		destroyNode(mRoot);
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::clear()
	{
		destroyTree(mRoot);
	}
	//-------------------------------------------------------------------------
	size_t BaseBoundingSphereTree::size() const
	{
		return mNumLeaves;
	}
	//-------------------------------------------------------------------------
	bool BaseBoundingSphereTree::empty() const
	{
		return mNumLeaves == 0;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::destroyTree(Node* _node)
	{
		for(Children::iterator it = _node->children.begin(); 
			it != _node->children.end(); ++it)
		{
			BaseNode* basechild = *it;
			if(basechild->isLeaf)
			{
				Leaf* leaf = (Leaf*) basechild;
				destroyLeaf(leaf);
			}
			else
			{
				Node* node = (Node*) basechild;
				destroyTree(node);
				destroyNode(node);
			}
		}
		_node->children.clear();
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::setMaxRadiusList(const RealList& _maxRadiusList)
	{
		if(_maxRadiusList == mMaxRadiusList)
			return;

		_setMaxRadiusList(_maxRadiusList);
	}
	//-------------------------------------------------------------------------
	const BaseBoundingSphereTree::RealList& BaseBoundingSphereTree::getMaxRadiusList() const
	{
		return mMaxRadiusList;
	}
	//-------------------------------------------------------------------------
	size_t BaseBoundingSphereTree::getDepth() const
	{
		return mNumLevels - 2;
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::ElementHandle 
		BaseBoundingSphereTree::add(const Vector3& _center, Real _radius)
	{
		// create a new leaf
		Leaf* leaf = createLeaf();
		leaf->center = _center;
		leaf->radius = _radius;

		// insert it to the sphere tree
		insertLeafToTree(leaf, mRoot);
		return leaf;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::insertLeafToTree(Leaf* _leaf, Node* _node)
	{
		// Non-recursive part of the algorithm
		Real needRadius = _node->center.distance(_leaf->center) + _leaf->radius;
		_node->radius = std::max(_node->radius, needRadius);
		if(_node->radius > mMaxRadiusList2[_node->level])
			GOTHOGRE_EXCEPT("Bounding sphere tree is inconsistent!");
		
		insertLeafToTree_r(_leaf, _node);
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::insertLeafToTree_r(Leaf* _leaf, Node* _node)
	{
		// Recursive part of the algorithm

		//
		size_t level = _node->level; 
		size_t childLevel = level + 1;
		if(childLevel >= mMaxRadiusList2.size() || _leaf->radius > mMaxRadiusList2[childLevel])
		{
			_leaf->parent = _node;
			_node->children.push_front(_leaf);
			_leaf->iteratorInParent = _node->children.begin();
			changeLeafLevel(_leaf, childLevel);
			return;
		}
		Real childMaxRadius = mMaxRadiusList2[childLevel];

		// Searching a child node to insert the leaf
		Node* nearChild = nullptr;
		Real minNeedRadius = 0;

		for(Children::iterator it = _node->children.begin(); it != _node->children.end(); ++it)
		{
			BaseNode* basechild = *it;
			if(!basechild->isLeaf)
			{
				Node* child = (Node*) basechild;
				Real needRadius = child->center.distance(_leaf->center) + _leaf->radius;
				Real delta = needRadius - child->radius;
				if(delta <= 0)
				{
					nearChild = child;
					break;
				}
				else if(needRadius <= childMaxRadius)
				{
					if(!nearChild || minNeedRadius > needRadius)
					{
						nearChild = child;
						minNeedRadius = needRadius;
					}
				}
			}
		}

		if(nearChild)
		{
			// Found a child node, then increase its radius
			nearChild->radius = std::max(nearChild->radius, minNeedRadius);
		}
		else
		{
			// An appropriate child node not found,
			// so we should create a new child node
			nearChild = createNode(childLevel);
			nearChild->center = _leaf->center;
			nearChild->radius = _leaf->radius;
			nearChild->parent = _node;
			_node->children.push_front(nearChild);
			nearChild->iteratorInParent = _node->children.begin();
		}

		// Go to subtree
		insertLeafToTree_r( _leaf, nearChild);
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::removeLeafFromTree(Leaf* _leaf)
	{
		_leaf->parent->children.erase(_leaf->iteratorInParent);
		_leaf->parent = nullptr;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::reinsert(Leaf* _leaf)
	{
		// find a subtree to insert
		Node* oldParent = _leaf->parent;
		Node* newParent = oldParent;
		while(newParent != mRoot)
		{
			Real needRadius = newParent->center.distance(_leaf->center) + _leaf->radius;
			Real delta = needRadius - newParent->radius;
			if(delta <= 0)
				break;
			newParent = newParent->parent;
		}
		
		// insert the leaf to the found subtree
		if(newParent != oldParent)
		{
			removeLeafFromTree(_leaf);
			insertLeafToTree(_leaf, newParent);
			destroyUnusedNodes(oldParent);
		}
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::destroyUnusedNodes(Node* _node)
	{
		Node* node = _node;
		while(node != mRoot && node->children.empty())
		{
			Node* parent = node->parent;
			parent->children.erase(node->iteratorInParent);
			destroyNode(node);
			node = parent;
		}
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::remove(ElementHandle _handle)
	{
		Leaf* leaf = (Leaf*)_handle;
		Node* parent = leaf->parent;
		removeLeafFromTree(leaf);
		destroyLeaf(leaf);
		destroyUnusedNodes(parent);
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::setCenter(ElementHandle _handle, const Vector3& _newcenter)
	{
		Leaf* leaf = (Leaf*)_handle;
		if(leaf->center != _newcenter)
		{
			leaf->center = _newcenter;
			reinsert(leaf);
		}
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::setRadius(ElementHandle _handle, Real _newradius)
	{
		Leaf* leaf = (Leaf*)_handle;
		if(leaf->radius != _newradius)
		{
			leaf->radius = _newradius;
			reinsert(leaf);
		}
	}
	//-------------------------------------------------------------------------
	const Vector3& BaseBoundingSphereTree::getCenter(ElementHandle _handle)
	{
		Leaf* leaf = (Leaf*)_handle;
		return leaf->center;
	}
	//-------------------------------------------------------------------------
	Real BaseBoundingSphereTree::getRadius(ElementHandle _handle) const
	{
		Leaf* leaf = (Leaf*)_handle;
		return leaf->radius;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::_setMaxRadiusList(const RealList& _maxRadiusList)
	{
		mMaxRadiusList = _maxRadiusList;

		// Calculate suitable version of the list of max radii
		RealList tmplist = mMaxRadiusList;
		std::sort(tmplist.begin(), tmplist.end(), std::greater<Real>());
		tmplist.erase(std::unique(tmplist.begin(), tmplist.end()), tmplist.end());
		tmplist.erase(std::remove_if(tmplist.begin(), tmplist.end(), std::bind2nd(std::less_equal<Real>(), 0)), tmplist.end());
		tmplist.erase(std::remove_if(tmplist.begin(), tmplist.end(), std::bind2nd(std::equal_to<Real>(), Math::POS_INFINITY)), tmplist.end());
		tmplist.insert(tmplist.begin(), 1, Math::POS_INFINITY);

		// Store the suitable version in the member variable
		mMaxRadiusList2 = tmplist;
		mNumLevels = mMaxRadiusList2.size() + 1;

		// Clear statistics
		mMaxNodesByLevel.assign(mNumLevels, 0);
		mMaxLeavesByLevel.assign(mNumLevels, 0);
		mRiNumChecksByLevel.assign(mNumLevels, 0);
		mRiCount = 0;
		mSiNumChecksByLevel.assign(mNumLevels, 0);
		mSiCount = 0;
		mFiNumChecksByLevel.assign(mNumLevels, 0);
		mFiCount = 0;

		// We need to rebuild tree if it has been changed from the initial state
		rebuildTree();
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::rebuildTree()
	{
		if(mNumNodesByLevel.size() < mNumLevels)
			mNumNodesByLevel.resize(mNumLevels, 0);
		if(mNumLeavesByLevel.size() < mNumLevels)
			mNumLeavesByLevel.resize(mNumLevels, 0);

		Node* newRoot = createRootNode();
		if(mRoot)
		{
			moveLeavesFromFirstToSecond(mRoot, newRoot);
			clear();
			destroyNode(mRoot);
		}
		mRoot = newRoot;

		mNumNodesByLevel.resize(mNumLevels);
		mNumLeavesByLevel.resize(mNumLevels);
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::moveLeavesFromFirstToSecond(Node* _node, Node* _newRoot)
	{
		for(Children::iterator it = _node->children.begin(); it != _node->children.end(); ++it)
		{
			BaseNode* basechild = *it;
			if(basechild->isLeaf)
			{
				Leaf* leaf = (Leaf*) basechild;
				removeLeafFromTree(leaf);
				insertLeafToTree(leaf, _newRoot);
			}
			else
			{
				Node* child = (Node*) basechild;
				moveLeavesFromFirstToSecond(child, _newRoot);
			}
		}
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::Node* BaseBoundingSphereTree::createRootNode()
	{
		Node* node = createNode(0);
		node->parent = nullptr;
		node->center = Vector3::ZERO;
		node->radius = 0;
		node->level = 0;
		return node;
	}






	//-------------------------------------------------------------------------
	// Implementation: Find operation
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findAll(
		BoundingSphereTreeListener* _listener, void* _param)
	{
		FindAll fa;
		fa.listener = _listener;
		fa.param = _param;

		findAll_r(fa, mRoot);
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findAll_r(
		FindAll& _fa, BaseNode* _basenode)
	{
		if(_basenode->isLeaf)
		{
			Leaf* leaf = (Leaf*)_basenode;
			_fa.listener->enumerateAll(leaf, _fa.param);
		}
		else
		{
			Node* node = (Node*)_basenode;
			for(Children::iterator it = node->children.begin(); it != node->children.end(); ++it)
			{
				BaseNode* child = *it;
				findAll_r(_fa, child);
			}
		}
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::ElementHandleVector BaseBoundingSphereTree::findAll()
	{
		ElementHandleVector vec;
		findAll(this, &vec);
		return vec;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::enumerateAll(ElementHandle _handle, void* _param)
	{
		ElementHandleVector& vec = *(ElementHandleVector*)_param;
		vec.push_back(_handle);
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::RayInfo::RayInfo(const Ray& _ray)
		: ray(_ray), maxDistance(Math::POS_INFINITY) {}

	BaseBoundingSphereTree::RayInfo::RayInfo(const Ray& _ray, Real _maxDistance)
		: ray(_ray), maxDistance(_maxDistance) {}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findRayIntersects(const RayInfo& _rayInfo, 
		BoundingSphereTreeListener* _listener, void* _param)
	{
		FindRayIntersects fri;
		fri.listener = _listener;
		fri.param = _param;
		fri.numChecksByLevel.assign(mNumLevels, 0);
		fri.rayOrigin = _rayInfo.ray.getOrigin();
		fri.rayDirection = _rayInfo.ray.getDirection();
		fri.maxDistance = _rayInfo.maxDistance;

		findRayIntersects_r(fri, mRoot);

		++mRiCount;
		for(size_t i = 0; i != mNumLevels; ++i)
			mRiNumChecksByLevel[i] += fri.numChecksByLevel[i];
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findRayIntersects_r(
		FindRayIntersects& _fri, BaseNode* _basenode)
	{
		++_fri.numChecksByLevel[_basenode->level];

		// Geometric calculations, see picture ...jpg
		const Vector3& OA = _fri.rayOrigin;
		const Vector3& OB = _basenode->center;
		Real r = _basenode->radius;
		Real sqr_r = r * r;

		Vector3 AB = OB - OA;
		Real sqr_AB = AB.squaredLength();

		Real distance; 
		if(sqr_AB <= sqr_r) 
		{
			// The origin of the ray is inside the sphere
			distance = 0; 
		}
		else
		{
			Real abs_AB = sqrt(sqr_AB);
			Real abs_AF = abs_AB - r;
			if(abs_AF > _fri.maxDistance)
				return;
			
			Real abs_AC = AB.dotProduct(_fri.rayDirection);
			if(abs_AC < 0)
				return;

			Vector3 AC = abs_AC * _fri.rayDirection;
			Vector3 OC = OA + AC;
			Vector3 BC = AC - AB;

			Real sqr_BC = BC.squaredLength();
			if(sqr_BC > sqr_r)
				return;

			Real abs_CD = sqrt(sqr_r - sqr_BC);
			distance = abs_AC - abs_CD;
			if(distance > _fri.maxDistance)
				return;
		}

		if(_basenode->isLeaf)
		{
			Leaf* leaf = (Leaf*)_basenode;
			_fri.listener->enumerateRayIntersects(leaf, distance, _fri.param);
		}
		else
		{
			Node* node = (Node*)_basenode;
			for(Children::iterator it = node->children.begin(); it != node->children.end(); ++it)
			{
				BaseNode* child = *it;
				findRayIntersects_r(_fri, child);
			}
		}		
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::ElementHandleVector BaseBoundingSphereTree::findRayIntersects(const RayInfo& _rayInfo)
	{
		ElementHandleVector vec;
		findRayIntersects(_rayInfo, this, &vec);
		return vec;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::enumerateRayIntersects(ElementHandle _handle, Real _distance, void* _param)
	{
		ElementHandleVector& vec = *(ElementHandleVector*)_param;
		vec.push_back(_handle);
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::SphereInfo::SphereInfo(const Sphere& _sphere)
		: sphere(_sphere) {}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findSphereIntersects(const SphereInfo& _sphereInfo, 
		BoundingSphereTreeListener* _listener, void* _param)
	{
		FindSphereIntersects fsi;
		fsi.listener = _listener;
		fsi.param = _param;
		fsi.numChecksByLevel.assign(mNumLevels, 0);
		fsi.sphereCenter = _sphereInfo.sphere.getCenter();
		fsi.sphereRadius = _sphereInfo.sphere.getRadius();

		findSphereIntersects_r(fsi, mRoot);

		++mSiCount;
		for(size_t i = 0; i != mNumLevels; ++i)
			mSiNumChecksByLevel[i] += fsi.numChecksByLevel[i];
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findSphereIntersects_r(
		FindSphereIntersects& _fsi, BaseNode* _basenode)
	{
		++_fsi.numChecksByLevel[_basenode->level];

		const Vector3& A = _fsi.sphereCenter;
		const Vector3& B = _basenode->center;
		Real r = _basenode->radius;
		Real R = _fsi.sphereRadius;

		Real sqr_AB = A.squaredDistance(B);
		Real rPlusR = r + R; 
		Real sqr_rPlusR = rPlusR * rPlusR;
		if(sqr_AB > sqr_rPlusR)
			return;

		if(_basenode->isLeaf)
		{
			Real abs_AB = sqrt(sqr_AB);
			Real distance = abs_AB - r;
			if(distance < 0)
				distance = 0;

			Leaf* leaf = (Leaf*)_basenode;
			_fsi.listener->enumerateSphereIntersects(leaf, distance, _fsi.param);
		}
		else
		{
			Node* node = (Node*)_basenode;
			for(Children::iterator it = node->children.begin(); it != node->children.end(); ++it)
			{
				BaseNode* child = *it;
				findSphereIntersects_r(_fsi, child);
			}
		}	
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::ElementHandleVector BaseBoundingSphereTree::findSphereIntersects(const SphereInfo& _sphereInfo)
	{
		ElementHandleVector vec;
		findSphereIntersects(_sphereInfo, this, &vec);
		return vec;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::enumerateSphereIntersects(ElementHandle _handle, Real _distance, void* _param)
	{
		ElementHandleVector& vec = *(ElementHandleVector*)_param;
		vec.push_back(_handle);
	}


	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::FrustumInfo::FrustumInfo(const Frustum* _frustum)
		: frustum(_frustum), farClipDistance(Math::POS_INFINITY) {}
	
	BaseBoundingSphereTree::FrustumInfo::FrustumInfo(const Frustum* _frustum, Real _farClipDistance)
		: frustum(_frustum), farClipDistance(_farClipDistance) {}

	BaseBoundingSphereTree::FrustumInfo::FrustumInfo(const Frustum* _frustum, const RealPairList& _distanceMinRadiusPairList)
		: frustum(_frustum), farClipDistance(Math::POS_INFINITY), distanceMinRadiusPairList(_distanceMinRadiusPairList) {}

	BaseBoundingSphereTree::FrustumInfo::FrustumInfo(const Frustum* _frustum, Real _farClipDistance, const RealPairList& _distanceMinRadiusPairList)
		: frustum(_frustum), farClipDistance(_farClipDistance), distanceMinRadiusPairList(_distanceMinRadiusPairList) {}

	BaseBoundingSphereTree::FrustumInfo::FrustumInfo(const Frustum* _frustum, const RealPairList& _distanceMinRadiusPairList, Real _farClipDistance)
		: frustum(_frustum), farClipDistance(_farClipDistance), distanceMinRadiusPairList(_distanceMinRadiusPairList) {}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findFrustumIntersects(const FrustumInfo& _frustumInfo, BoundingSphereTreeListener* _listener, void* _param)
	{
		FindFrustumIntersects ffi;
		ffi.listener = _listener;
		ffi.param = _param;
		ffi.numChecksByLevel.assign(mNumLevels, 0);

		const Frustum* frustum = _frustumInfo.frustum;
		const Vector3& frustumPosition = frustum->getPositionForViewUpdate();
		Vector3 frustumDirection = frustum->getOrientationForViewUpdate() * Vector3::NEGATIVE_UNIT_Z;
		ffi.directionPerpPlane = Plane(frustumDirection, frustumPosition);

		ffi.clippingPlanes[0] = frustum->getFrustumPlane(FRUSTUM_PLANE_NEAR);
		ffi.clippingPlanes[1] = frustum->getFrustumPlane(FRUSTUM_PLANE_LEFT);
		ffi.clippingPlanes[2] = frustum->getFrustumPlane(FRUSTUM_PLANE_RIGHT);
		ffi.clippingPlanes[3] = frustum->getFrustumPlane(FRUSTUM_PLANE_TOP);
		ffi.clippingPlanes[4] = frustum->getFrustumPlane(FRUSTUM_PLANE_BOTTOM);

		ffi.farClipDistance = _frustumInfo.farClipDistance;
		if(ffi.farClipDistance <= 0 || ffi.farClipDistance == Math::POS_INFINITY)
			ffi.farClipDistance = frustum->getFarClipDistance();
		if(ffi.farClipDistance <= 0)
			ffi.farClipDistance = Math::POS_INFINITY;

		ffi.linearFuncByDistance.clear();
		if(_frustumInfo.distanceMinRadiusPairList.size() >= 2)
		{
			RealPairList tmplist = _frustumInfo.distanceMinRadiusPairList;
			std::sort(tmplist.begin(), tmplist.end(), std::less<std::pair<Real, Real>>());

			for(size_t i = 0; i != tmplist.size(); ++i)
			{
				Real x0 = tmplist[i].first;
				Real y0 = tmplist[i].second;
				if(i != tmplist.size() - 1)
				{
					Real x1 = tmplist[i+1].first;
					Real y1 = tmplist[i+1].second;
					if(x0 != x1)
					{
						Real a = (y1 - y0) / (x1 - x0);
						Real b = y0 - a * x0;
						LinearFunc lf;
						lf.a = a;
						lf.b = b;
						ffi.linearFuncByDistance.insert(
							ffi.linearFuncByDistance.begin(), 
							std::make_pair(x0, lf));
					}
				}
				else if(ffi.linearFuncByDistance.empty())
				{
					LinearFunc lf;
					lf.a = y0/x0;
					lf.b = 0;
					ffi.linearFuncByDistance.push_back(std::make_pair(x0, lf));
				}
			}
		}

		findFrustumIntersects_r(ffi, mRoot);

		++mFiCount;
		for(size_t i = 0; i != mNumLevels; ++i)
			mFiNumChecksByLevel[i] += ffi.numChecksByLevel[i];
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::findFrustumIntersects_r(FindFrustumIntersects& _ffi, BaseNode* _basenode)
	{
		++_ffi.numChecksByLevel[_basenode->level];

		const Vector3& B = _basenode->center;
		Real r = _basenode->radius;

		for(size_t i = 0; i != 5; ++i)
			if(_ffi.clippingPlanes[i].getDistance(B) + r < 0)
				return;

		Real pseudodistance = _ffi.directionPerpPlane.getDistance(B);
		Real distance = pseudodistance - r;
		if(distance > _ffi.farClipDistance)
			return;

		if(distance < 0)
			distance = 0;

		for(size_t j = 0; j != _ffi.linearFuncByDistance.size(); ++j)
		{
			Real d = _ffi.linearFuncByDistance[j].first;
			if(d <= distance)
			{
				const LinearFunc& lf = _ffi.linearFuncByDistance[j].second;
				if(r < lf.a * distance + lf.b)
					return;
				break;
			}
		}

		if(_basenode->isLeaf)
		{
			Leaf* leaf = (Leaf*)_basenode;
			_ffi.listener->enumerateFrustumIntersects(leaf, distance, _ffi.param);
		}
		else
		{
			Node* node = (Node*)_basenode;
			for(Children::iterator it = node->children.begin(); it != node->children.end(); ++it)
			{
				BaseNode* child = *it;
				findFrustumIntersects_r(_ffi, child);
			}
		}	
	}
	//-------------------------------------------------------------------------
	BaseBoundingSphereTree::ElementHandleVector BaseBoundingSphereTree::findFrustumIntersects(const FrustumInfo& _frustumInfo)
	{
		ElementHandleVector vec;
		findFrustumIntersects(_frustumInfo, this, &vec);
		return vec;
	}
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::enumerateFrustumIntersects(ElementHandle _handle, Real _distance, void* _param)
	{
		ElementHandleVector& vec = *(ElementHandleVector*)_param;
		vec.push_back(_handle);
	}




	//-------------------------------------------------------------------------
	// Statistics
	//-------------------------------------------------------------------------
	void BaseBoundingSphereTree::logStatistics(const String& _name)
	{
		// General statistics
		GOTHOGRE_INFO(_name << ": num_levels: " << mNumLevels 
			<< ", max_radii: " << mMaxRadiusList2);

		GOTHOGRE_INFO(_name << ": max. number of Nodes and Leaves: ");
		StrStream ss;
		ss << _name << ": ";
		size_t sum_n = 0, sum_l = 0;
		for(size_t i = 0; i != mNumLevels; ++i)
		{
			size_t n = mMaxNodesByLevel[i];
			size_t l = mMaxLeavesByLevel[i];
			sum_n += n;
			sum_l += l;
			ss << "level " << i << ": " << n << "N " << l << "L, ";
		}
		ss << "total: " << sum_n << "N " << sum_l << "L.";
		GOTHOGRE_INFO(ss.str());
		
		// FindRayIntersects statistics
		if(mRiCount != 0)
		{
			GOTHOGRE_INFO(_name << ": findRayIntersects (" << mRiCount << "): ");
			StrStream ss;
			ss << _name << ": ";
			ss << "avg. number of checks: ";
			Real total = 0;
			for(size_t i = 0; i != mNumLevels; ++i)
			{
				Real val = (Real) mRiNumChecksByLevel[i] / (Real) mRiCount;
				ss << "level " << i << ": " << val << ", ";
				total += val;
			}
			ss << "total: " << total << ".";
			GOTHOGRE_INFO(ss.str());
		}

		// FindSphereIntersects statistics
		if(mSiCount != 0)
		{
			GOTHOGRE_INFO(_name << ": findSphereIntersects (" << mSiCount << "): ");
			StrStream ss;
			ss << _name << ": ";
			ss << "avg. number of checks: ";
			Real total = 0;
			for(size_t i = 0; i != mNumLevels; ++i)
			{
				Real val = (Real) mSiNumChecksByLevel[i] / (Real) mSiCount;
				ss << "level " << i << ": " << val << ", ";
				total += val;
			}
			ss << "total: " << total << ".";
			GOTHOGRE_INFO(ss.str());
		}

		// FindFrustumIntersects statistics
		if(mFiCount != 0)
		{
			GOTHOGRE_INFO(_name << ": findFrustumIntersects (" << mFiCount << "): ");
			StrStream ss;
			ss << _name << ": ";
			ss << "avg. number of checks: ";
			Real total = 0;
			for(size_t i = 0; i != mNumLevels; ++i)
			{
				Real val = (Real) mFiNumChecksByLevel[i] / (Real) mFiCount;
				ss << "level " << i << ": " << val << ", ";
				total += val;
			}
			ss << "total: " << total << ".";
			GOTHOGRE_INFO(ss.str());
		}
	}

} // namespace GothOgre