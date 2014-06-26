#include "GothOgre_Precompiled.h"
#include "GothOgre_MemoryPoolAllocator.h"
#include "GothOgre_Bsp.h"
#include "GothOgre_BspManagerEx.h"


namespace GothOgre
{
	// Pool allocator to make operations faster.
	static MemoryPoolAllocator<Bsp::Node> sNodeAlloc(8192);
	static MemoryPoolAllocator<Bsp::Leaf> sLeafAlloc(8192);
	//-------------------------------------------------------------------------
	Bsp::Node* Bsp::createNode()
	{
		Node* p = sNodeAlloc.construct_new();
		return p;
	}
	//-------------------------------------------------------------------------
	Bsp::Leaf* Bsp::createLeaf()
	{
		Leaf* p = sLeafAlloc.construct_new();
		return p;
	}
	//-------------------------------------------------------------------------
	void Bsp::destroyNode(Bsp::Node* _p)
	{
		sNodeAlloc.del(_p);
	}
	//-------------------------------------------------------------------------
	void Bsp::destroyLeaf(Bsp::Leaf* _p)
	{
		sLeafAlloc.del(_p);
	}
	//-------------------------------------------------------------------------
	Bsp::Node* Bsp::Node::createFrontNode()
	{
		Bsp::Node* node = Bsp::createNode();
		mFront = node;
		return node;
	}
	//-------------------------------------------------------------------------
	Bsp::Leaf* Bsp::Node::createFrontLeaf()
	{
		Bsp::Leaf* leaf = Bsp::createLeaf();
		mFront = leaf;
		return leaf;
	}
	//-------------------------------------------------------------------------
	void Bsp::Node::destroyFront()
	{
		if(mFront)
		{
			if(mFront->isLeaf())
			{
				Bsp::Leaf* leaf = static_cast<Leaf*>(mFront);
				Bsp::destroyLeaf(leaf);
			}
			else
			{
				Bsp::Node* node = static_cast<Node*>(mFront);
				Bsp::destroyNode(node);
			}
		}
	}
	//-------------------------------------------------------------------------
	Bsp::Node* Bsp::Node::createBackNode()
	{
		Bsp::Node* node = Bsp::createNode();
		mBack = node;
		return node;
	}
	//-------------------------------------------------------------------------
	Bsp::Leaf* Bsp::Node::createBackLeaf()
	{
		Bsp::Leaf* leaf = Bsp::createLeaf();
		mBack = leaf;
		return leaf;
	}
	//-------------------------------------------------------------------------
	void Bsp::Node::destroyBack()
	{
		if(mBack)
		{
			if(mBack->isLeaf())
			{
				Leaf* leaf = static_cast<Leaf*>(mBack);
				Bsp::destroyLeaf(leaf);
			}
			else
			{
				Node* node = static_cast<Node*>(mBack);
				Bsp::destroyNode(node);
			}
		}
	}
	//-------------------------------------------------------------------------
	Bsp::Bsp(Ogre::ResourceManager* _creator, const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual, ManualResourceLoader* _loader)
		: Resource(_creator, _name, _handle, _group, _isManual, _loader)
	{
		mRootNode = Bsp::createNode();
		mNumNodes = 0;
		mNumLeaves = 0;
		mMaxDepth = 0;
	}
	//-------------------------------------------------------------------------
	Bsp::~Bsp()
	{
		destroyNode(mRootNode);
	}
	//-------------------------------------------------------------------------
	void Bsp::loadImpl()
	{
		/** Internal implementation of the meat of the 'load' action, 
			must not be called because this resource type must be
			loaded only via ManualResourceLoader. */
		GOTHOGRE_EXCEPT("Bsp::loadImpl never should be called!");		
	}
	//-------------------------------------------------------------------------
	void Bsp::unloadImpl()
	{
		destroyNode(mRootNode);
		mRootNode = Bsp::createNode();
	}
	//-------------------------------------------------------------------------
	size_t Bsp::calculateSize() const
	{
		size_t sizeVertices = mVertices.size() * sizeof(Vertex);
		size_t sizeFaces = mFaces.size() * sizeof(Face);
		size_t sizeMaterials = mMaterials.size() * sizeof(MaterialPtr);
		size_t sizeNodes = getNumNodes() * sizeof(Node);
		size_t sizeLeaves = getNumLeaves() * sizeof(Leaf);
		return sizeVertices + sizeFaces + sizeMaterials + sizeNodes + sizeLeaves;
	}
	//-------------------------------------------------------------------------
	size_t Bsp::getNumNodes() const
	{
		return mNumNodes;
	}
	//-------------------------------------------------------------------------
	size_t Bsp::getNumLeaves() const
	{
		return mNumLeaves;
	}
	//-------------------------------------------------------------------------
	size_t Bsp::getMaxDepth() const
	{
		return mMaxDepth;
	}
	//-------------------------------------------------------------------------
	void Bsp::calculateStats()
	{
		mNumLeaves = 0;
		mNumNodes = 0;
		mMaxDepth = 0;
		calculateStats(mRootNode, 0);
	}
	//-------------------------------------------------------------------------
	void Bsp::calculateStats(NodeBase* _nodeBase, size_t _depth)
	{
		if(mMaxDepth < _depth)
			mMaxDepth = _depth;	

		if(_nodeBase->isLeaf())
		{
			++mNumLeaves;
		}
		else
		{
			++mNumNodes;
			++_depth;
			Node* node = static_cast<Node*>(_nodeBase);
			calculateStats(node->getBack(), _depth);
			calculateStats(node->getFront(), _depth);
		}
	}


	//------------------------------------------------------------------------
	// BspLoader
	//------------------------------------------------------------------------
	BspLoader::BspLoader(const String& _name, int _order)
		: ResourceLoader(BspManagerEx::getSingletonPtr(), _name, _order)
	{
	}


	//------------------------------------------------------------------------
	// BspSaver
	//------------------------------------------------------------------------
	BspSaver::BspSaver(const String& _name, int _order)
		: ResourceSaver(BspManagerEx::getSingletonPtr(), _name, _order)
	{
	}

} // namespace GothOgre