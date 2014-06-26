#include "Precompiled.h"
#include "MeshAndBspLoadContext.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readMeshAndBspHeader()
	{
		// Known versions of MeshAndBsp:
		// 0x02090000 - Gothic 1,
		// 0x04090000 - Gothic 2.
		mDataStream->readData(mMeshAndBspVersion);
		if(mMeshAndBspVersion != 0x02090000 && mMeshAndBspVersion != 0x04090000)
		{
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - Version of MeshAndBsp " << mMeshAndBspVersion << " is not supported.");
		}
		mDataStream->readData(mMeshAndBspSize);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readBspInfo(Chunk& _chunk)
	{
		// Known BSP versions:
		// 0x0002 - Gothic 1,
		// 0x0003 - Gothic 2.
		mDataStream->readData(mBspVersion); 
		if(mBspVersion != 0x0002 && mBspVersion != 0x0003)
		{
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - Version of Bsp " << mBspVersion << " is not supported.");
		}
		mDataStream->readData(mEnvironment);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readPolygonIndices(Chunk& _chunk)
	{
		// Read indices of polygons sorted specially to use in BSP-nodes
		uint32 numPolygonIndices;
		mDataStream->readData(numPolygonIndices);
		mPolygonIndices.resize(numPolygonIndices);
		mDataStream->readData(&mPolygonIndices[0], numPolygonIndices * sizeof(uint32));
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readBspNodeBase(BspNodeBase* _p)
	{
		mDataStream->readData(&_p->aabb, 
			sizeof(_p->aabb) + sizeof(_p->firstPolygonIndex)
			+ sizeof(_p->numPolygonIndices));

		_p->aabb.max.x = std::max(_p->aabb.max.x, _p->aabb.min.x);
		_p->aabb.max.y = std::max(_p->aabb.max.y, _p->aabb.min.y);
		_p->aabb.max.z = std::max(_p->aabb.max.z, _p->aabb.min.z);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readBspLeaf(BspLeaf* _bspLeaf)
	{
		readBspNodeBase(_bspLeaf);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readBspNode(BspNode* _bspNode)
	{
		readBspNodeBase(_bspNode);

		// Read flags
		BspNodeFlags flags;
		mDataStream->readData(flags);

		// Read separation plane used in BSP technology
		mDataStream->readData(_bspNode->separationPlane);

		_bspNode->lod = 0;
		if(mBspVersion == 0x0002)
		{
			mDataStream->readData(_bspNode->lod);
		}
		
		// Recursively read front BSP nodes
		if(flags.frontExist)
		{
			if(flags.frontIsLeaf)
			{
				mBspLeaves.push_back(BspLeaf());
				BspLeaf* newBspLeaf = &mBspLeaves.back();
				_bspNode->front = newBspLeaf;
				readBspLeaf(newBspLeaf);
			}
			else
			{
				mBspNodes.push_back(BspNode());
				BspNode* newBspNode = &mBspNodes.back();
				_bspNode->front = newBspNode;
				readBspNode(newBspNode);
			}
		}

		// Recursively read back BSP nodes
		if(flags.backExist)
		{
			if(flags.backIsLeaf)
			{
				mBspLeaves.push_back(BspLeaf());
				BspLeaf* newBspLeaf = &mBspLeaves.back();
				_bspNode->back = newBspLeaf;
				readBspLeaf(newBspLeaf);
			}
			else
			{
				mBspNodes.push_back(BspNode());
				BspNode* newBspNode = &mBspNodes.back();
				_bspNode->back = newBspNode;
				readBspNode(newBspNode);
			}
		}
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readBspNodes(Chunk& _chunk)
	{
		// Read all BSP nodes
		uint32 numBspNodesAndLeaves, numBspNodes, numBspLeaves;
		mDataStream->readData(numBspNodesAndLeaves);
		mDataStream->readData(numBspLeaves);
		numBspNodes = numBspNodesAndLeaves - numBspLeaves;

		mBspNodes.reserve(numBspNodes);
		mBspLeaves.reserve(numBspLeaves);

		mBspNodes.push_back(BspNode());
		mRootBspNode = &mBspNodes.back();
		readBspNode(mRootBspNode);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readLights(Chunk& _chunk)
	{
		// ???
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::readSectors(Chunk& _chunk)
	{
		uint32 numSectors;
		mDataStream->readData(numSectors);
		mSectors.resize(numSectors);

		for(uint32 i = 0; i != numSectors; ++i)
		{
			Sector& sector = mSectors[i];
			sector.name = mDataStream->readLine(EOL::LF);
			uint32 numBspLeaves, numPolygons;
			mDataStream->readData(numBspLeaves);
			mDataStream->readData(numPolygons);

			sector.bspLeafIndices.resize(numBspLeaves);
			sector.polygonIndices.resize(numPolygons);
			if(numBspLeaves != 0)
				mDataStream->read(&sector.bspLeafIndices[0], numBspLeaves * sizeof(uint32));
			
			if(numPolygons != 0)
				mDataStream->read(&sector.polygonIndices[0], numPolygons * sizeof(uint32));
		}
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::countPolygonUsesInLod0(BspNodeBase* _nodeBase)
	{
		if(_nodeBase)
		{
			if(_nodeBase->isLeaf)
			{
				BspLeaf* leaf = static_cast<BspLeaf*>(_nodeBase);
				size_t ibegin = leaf->firstPolygonIndex;
				size_t iend = ibegin + leaf->numPolygonIndices;
				for(size_t i = ibegin; i != iend; ++i)
				{
					size_t polyidx = mPolygonIndices[i];
					mPolygonUsesInLod0[polyidx]++;
				}
			}
			else
			{
				BspNode* node = static_cast<BspNode*>(_nodeBase);
				countPolygonUsesInLod0(node->front);
				countPolygonUsesInLod0(node->back);
			}
		}
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::countPolygonUsesInLod0()
	{
		// Prepare to count uses: fill the array with zero
		mPolygonUsesInLod0.resize(mPolygons.size(), 0);

		// Count uses of polygons in LOD 0
		countPolygonUsesInLod0(mRootBspNode);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::removePolygonsNotUsedInLod0()
	{
		size_t i = 0; 
		while(i != mPolygons.size())
		{
			if(mPolygonUsesInLod0[i] == 0)
			{
				// fast remove i-th polygon, without saving order
				std::swap(mPolygons[i], mPolygons.back());
				mPolygons.pop_back();
			}
			++i;
		}
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::logStatistics()
	{
		String prefix = mDataStream->getName() + " - Bsp: ";
		GOTHOGRE_INFO(prefix << "vertices: " << mVertFeatures.size());
		GOTHOGRE_INFO(prefix << "polygons: " << mPolygons.size());
		GOTHOGRE_INFO(prefix << "leaves: "   << mBspLeaves.size());
		GOTHOGRE_INFO(prefix << "nodes: "    << mBspNodes.size());
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::load(const DataStreamExPtr& _dataStream)
	{
		clear();
		mDataStream = _dataStream;

		Progress& progress = Progress::getSingleton();
		progress.setRange(0, 100);

		// Read version and size of entire MeshAndBsp.
		readMeshAndBspHeader();

		// Read mesh
		MshFileLoadContext::load(_dataStream);
		progress.setPosition(5);

		// Read BSP
		bool finish = false;
		while(!finish)
		{
			Chunk chunk(mDataStream);
			switch(chunk.getID())
			{
				case 0xC000: readBspInfo(chunk); break;
				case 0xC010: readPolygonIndices(chunk); break;
				case 0xC040: readBspNodes(chunk); break;
				case 0xC045: readLights(chunk); break;
				case 0xC050: readSectors(chunk); break;
				case 0xC0FF: finish = true; break;
			}
		}
		progress.setPosition(10);

		// Remove excess polygons
		countPolygonUsesInLod0();
		removePolygonsNotUsedInLod0();

		// Log statistics
		logStatistics();
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::apply(Resource* _destBsp)
	{
		// Create Bsp resource
		MshFileLoadContext::apply(_destBsp);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::skip(const DataStreamExPtr& _dataStream)
	{
		clear();
		mDataStream = _dataStream;
		readMeshAndBspHeader();
		mDataStream->skipg(mMeshAndBspSize);
	}
	//---------------------------------------------------------------------------------
	void MeshAndBspLoadContext::clear()
	{
		MshFileLoadContext::clear();

		mPolygonUsesInLod0.resize(0);
		mBspNodes.resize(0);
		mBspLeaves.resize(0);
		mSectors.resize(0);
		mPolygonIndices.resize(0);
		mRootBspNode = nullptr;

		if(mChildOperation != nullptr)
		{
			Progress& pb = Progress::getSingleton();
			pb.endChildOperation(mChildOperation);
			mChildOperation = nullptr;
			mChildOperation2 = nullptr;
		}
	}	
	//---------------------------------------------------------------------------------
	MeshAndBspLoadContext::MeshAndBspLoadContext()
	{
		mChildOperation = nullptr;
		mChildOperation2 = nullptr;
		clear();
	}
	//---------------------------------------------------------------------------------
	MeshAndBspLoadContext::~MeshAndBspLoadContext()
	{
	}

} // namespace GothOgre