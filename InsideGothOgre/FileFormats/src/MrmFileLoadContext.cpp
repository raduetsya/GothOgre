#include "Precompiled.h"
#include "MrmFileLoadContext.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------------
	MrmFileLoadContext::Chunk::Chunk(const DataStreamExPtr& _dataStream)
	{
		mDataStream = _dataStream;
		readHeader();
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::Chunk::readHeader()
	{
		mChunkPos = mDataStream->tellg();
		mDataStream->readData(&mChunkID, sizeof(mChunkID) + sizeof(mDataSize));
	}
	//---------------------------------------------------------------------------------
	MrmFileLoadContext::Chunk::~Chunk()
	{
		skipRest();
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::Chunk::skipRest()
	{
		size_t newPos = mChunkPos + mDataSize + 6;
		mDataStream->seekg(newPos);
	}
	//---------------------------------------------------------------------------------
	AxisAlignedBox MrmFileLoadContext::readAxisAlignedBox()
	{
		AABB aabb;
		mDataStream->readData(aabb);
		return AxisAlignedBox(aabb.min.x, aabb.min.y, aabb.min.z, 
									aabb.max.x, aabb.max.y, aabb.max.z);
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::readFileInfo()
	{
		mDataStream->readData(mMrmVersion);
		if(mMrmVersion != 0x0305 && mMrmVersion != 0x0905)
		{
			// 0x0305 - Gothic I
			// 0x0905 - Gothic II
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - Mrm version " << mMrmVersion << " is not supported.");
		}
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::readMeshData()
	{
		uint32 meshSize;
		mDataStream->readData(meshSize);
		mMeshData.resize(meshSize);
		mDataStream->readData(&mMeshData[0], meshSize);
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::readMeshDecl()
	{
		mDataStream->readData(mMeshDecl);
		mVertices = (Vec3*) ((uint8*) &mMeshData[0] + mMeshDecl.offsetVertices);
		mNumVertices = mMeshDecl.numVertices;
		mNormals = (Vec3*) ((uint8*) &mMeshData[0] + mMeshDecl.offsetNormals);
		mNumNormals = mMeshDecl.numNormals;
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::readSubMeshDecls()
	{
		mSubMeshDecls.resize(mMeshDecl.numMaterials);
		mDataStream->readData(&mSubMeshDecls[0], mMeshDecl.numMaterials * sizeof(SubMeshDecl));
		mSubMeshInfos.resize(mSubMeshDecls.size());
		for(size_t i = 0; i != mSubMeshDecls.size(); ++i)
		{
			SubMeshDecl& decl = mSubMeshDecls[i];
			SubMeshInfo& info = mSubMeshInfos[i];
			info.mtl.setNull();
			info.triangles = (Triangle*) ((uint8*) &mMeshData[0] + decl.offsetTriangles);
			info.numTriangles = decl.numTriangles;
			info.wedges = (Wedge*) ((uint8*) &mMeshData[0] + decl.offsetWedges);
			info.numWedges = decl.numWedges;
			info.leftByCollapsed = (uint16*) ((uint8*) &mMeshData[0] + decl.offsetLeftByCollapsed);
			info.numLeftByCollapsed = decl.numLeftByCollapsed;
			info.resolutions = (Resolution*) ((uint8*) &mMeshData[0] + decl.offsetResolutions);
			info.numResolutions = decl.numResolutions;
		}
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::readAndCreateMaterials()
	{
		// materials are read as zen archive.
		ZenArchive ar;
		ar.readHeader(mDataStream, ZenArchiveFlags::NO_LOG);

		SceneManagerEx* sceneMgr = SceneManagerExEnumerator::getSingleton().getActiveSceneManager();

		for(size_t i = 0; i != mSubMeshInfos.size(); ++i)
		{
			String mtlName = ar.readString(StringUtil::BLANK);
			MtlPtr mtl = MtlManagerEx::getSingleton().findByName(mtlName);
			if(mtl.isNull())
			{
				ZenObjectPtr zenmtl = ar.readObject();
				mtl = MtlManagerEx::getSingleton().findByName(mtlName);
			}
			else
			{
				ar.readChunkEnd(ar.readChunkBegin());
			}
			mSubMeshInfos[i].mtl = mtl;
		}
		bool unknown = ar.readBool(StringUtil::BLANK);
		ar.readFooter();
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::readBounds()
	{
		size_t t = mDataStream->tellg();
		mBounds = readAxisAlignedBox();
		// objectOrientedBox? - skipped
		// lod params? - skipped
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::calculateMeshResolutions()
	{
		if(mSubMeshInfos.empty())
		{
			mMeshResolutions.clear();
			return;
		}
		mMeshResolutions.resize(mSubMeshInfos[0].numResolutions);
		for(size_t j = 0; j != mMeshResolutions.size(); ++j)
		{
			mMeshResolutions[j].numTriangles = 0;
			mMeshResolutions[j].numWedges = 0;
		}
		for(size_t i = 0; i != mSubMeshInfos.size(); ++i)
			for(size_t j = 0; j != mMeshResolutions.size(); ++j)
			{
				mMeshResolutions[j].numTriangles += mSubMeshInfos[i].resolutions[j].numTriangles;
				mMeshResolutions[j].numWedges += mSubMeshInfos[i].resolutions[j].numWedges;
			}
	}
	//---------------------------------------------------------------------------------
	size_t MrmFileLoadContext::findResolution(Real _triangleQuota) const
	{
		if(mMeshResolutions.empty())
			return -1;

		size_t totalNumTriangles = mMeshResolutions[mMeshResolutions.size() - 1].numTriangles;
		Real _desiredNumTriangles = _triangleQuota * totalNumTriangles;
		size_t index = -1;
		Real minDelta = 0;
		for(size_t j = 0; j != mMeshResolutions.size(); ++j)
		{
			if(mMeshResolutions[j].numTriangles != 0)
			{
				Real delta = Math::Abs(mMeshResolutions[j].numTriangles - _desiredNumTriangles);
				if(index == -1 || delta < minDelta)
				{
					index = j;
					minDelta = delta;
				}
			}
		}
		return index;
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::initLodSubMeshVertices(const Resolution& _resolution, const SubMeshInfo& _info, SubMesh* _subMesh)
	{
		// Create buffer.
		HardwareVertexBufferSharedPtr buffer =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			32 /* 12(pos) + 12(normal) + 8(uv) */, 
			_resolution.numWedges, HardwareBuffer::HBU_STATIC, false);

		float* bufferPtr = (float*)buffer->lock(HardwareBuffer::HBL_DISCARD);
	
		// Write positions and normals and texture coordinates to the buffer.
		for(size_t j = 0; j != _resolution.numWedges; ++j)
		{
			Wedge& wedge = _info.wedges[j];
			Vec3& pos = mVertices[wedge.vertIndex];
			float* p = &bufferPtr[j * 8];
			p[0] = pos.x;
			p[1] = pos.z; // y <-> z
			p[2] = pos.y;
			p[3] = wedge.normal.x;
			p[4] = wedge.normal.z; // y <-> z
			p[5] = wedge.normal.y;
			p[6] = wedge.uvMapping.u;
			p[7] = wedge.uvMapping.v;
		}

		buffer->unlock();

		// Initialise vertex data
		_subMesh->useSharedVertices = false;
		_subMesh->vertexData = new VertexData();
		_subMesh->vertexData->vertexStart = 0;
		_subMesh->vertexData->vertexCount = _resolution.numWedges;

		_subMesh->vertexData->vertexDeclaration->addElement(0, 0, 
			VET_FLOAT3, VES_POSITION, 0);
		_subMesh->vertexData->vertexDeclaration->addElement(1, 12, 
			VET_FLOAT3, VES_NORMAL, 0);
		_subMesh->vertexData->vertexDeclaration->addElement(2, 24, 
			VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

		_subMesh->vertexData->vertexBufferBinding->setBinding(0, buffer);
		_subMesh->vertexData->vertexBufferBinding->setBinding(1, buffer);
		_subMesh->vertexData->vertexBufferBinding->setBinding(2, buffer);
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::initLodSubMeshIndices(const Resolution& _resolution, const SubMeshInfo& _info, SubMesh* _subMesh)
	{
		size_t numIndices = _resolution.numTriangles * 3;
		size_t numWedges = _resolution.numWedges;

		HardwareIndexBufferSharedPtr buffer = 
			HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT, numIndices, 
			HardwareBuffer::HBU_STATIC, false);

		uint16* bufferPtr = (uint16*) buffer->lock(HardwareBuffer::HBL_DISCARD);
		uint16* srcPtr = (uint16*) _info.triangles;
		for(size_t i = 0; i != numIndices; ++i)
		{
			size_t wedgeIndex = *srcPtr++;
			while(wedgeIndex >= numWedges)
				wedgeIndex = _info.leftByCollapsed[wedgeIndex];
			*bufferPtr++ = wedgeIndex;
		}

		buffer->unlock();
		IndexData* indexData = _subMesh->indexData;
		indexData->indexStart = 0;
		indexData->indexCount = numIndices;
		indexData->indexBuffer = buffer;
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::setBounds(Mesh* _lodMesh)
	{
		// y <-> z
		AxisAlignedBox bounds2 (mBounds.getMinimum().x, mBounds.getMinimum().z, mBounds.getMinimum().y, 
			mBounds.getMaximum().x, mBounds.getMaximum().z, mBounds.getMaximum().y );

		_lodMesh->_setBounds(bounds2, false);
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::initLodMesh(size_t _resolutionIndex, MeshEx* _lodMesh)
	{
		for(size_t i = 0; i != mSubMeshInfos.size(); ++i)
		{
			const SubMeshInfo& info = mSubMeshInfos[i];
			const Resolution& resolution = info.resolutions[_resolutionIndex];
			if(resolution.numTriangles != 0)
			{
				SubMeshEx* subMesh = _lodMesh->createSubMesh();
				subMesh->setMtl( info.mtl );
				subMesh->setMatGenParams( MatGenParams() );
				initLodSubMeshVertices(resolution, info, subMesh);
				initLodSubMeshIndices(resolution, info, subMesh);
			}
		}
		setBounds(_lodMesh);
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::initLodMeshes()
	{
		if(mMeshResolutions.empty())
			return;

		// Init LOD #0: All triangles are in use - highest details.
		initLodMesh(mMeshResolutions.size() - 1, mMesh);

		// Init LOD #1, LOD #2, ...
		vector<size_t>::type resolutionIndices;
		for(size_t i = 0; i != mLodConfig.size(); ++i)
		{
			if(mLodConfig[i].triangleQuota != 1.0f) // if 1 it's LOD #0 and it's already processed
			{
				size_t resolutionIndex = findResolution(mLodConfig[i].triangleQuota);
				if(resolutionIndex != -1 
					&& std::find(resolutionIndices.begin(), resolutionIndices.end(), resolutionIndex) == resolutionIndices.end())
				{
					resolutionIndices.push_back(resolutionIndex);
					StrStream ss;
					ss << mMesh->getName() + "LOD\x01";
					ss << resolutionIndex;
					const String& lodName = ss.str();
					MeshExPtr lodMesh = MeshManagerEx::getSingleton().createManual( lodName );
					initLodMesh(resolutionIndex, lodMesh.get());
					mMesh->createManualLodLevel(mLodConfig[i].value, lodMesh->getName(), lodMesh->getGroup());
				}
			}
		}

		// We must link MeshLodUsage::manualMesh to each LOD mesh
		// to increase reference counter for each LOD mesh
		// and prevent LOD meshes from deleting (as unused)
		// before they will been rendered.
		for(ushort index = 0; index != mMesh->getNumLodLevels(); ++index)
		{
			const MeshLodUsage& usage = mMesh->getLodLevel(index);
		}
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::load(const DataStreamExPtr& _dataStream)
	{
		clear();
		mDataStream = _dataStream;
		EncodingSettings::getSingleton().setupStreamEncoding(mDataStream);

		ulong numChunks = 0;
		bool finish = false;
		while(!finish)
		{
			Chunk chunk(mDataStream);
			++numChunks;

			if(numChunks == 1 && chunk.getID() != 0xB100)
				GOTHOGRE_EXCEPT(mDataStream->getName() << " - File is not a multi-resolution mesh (*.mrm).");

			switch(chunk.getID())
			{
				case 0xB100: 
				{
					readFileInfo();
					readMeshData();
					readMeshDecl();
					readSubMeshDecls();
					readAndCreateMaterials();
					readBounds();
					break;
				}
				case 0xB1FF: 
				{
					finish = true; 
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::skip(const DataStreamExPtr& _dataStream)
	{
		clear();
		mDataStream = _dataStream;
		ulong numChunks = 0;
		while(true)
		{
			Chunk chunk(mDataStream);
			++numChunks;

			if(numChunks == 1 && chunk.getID() != 0xB100)
				GOTHOGRE_EXCEPT(mDataStream->getName() << " - File is not a multi-resolution mesh (*.mrm).");

			if(chunk.getID() == 0xB1FF)
				break; 
		}
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::apply(Resource* _destMesh)
	{
		mMesh = static_cast<MeshEx*>(_destMesh);
		// materials were created already (by function readAndCreateMaterials)
		calculateMeshResolutions();
		initLodMeshes();
		clear();
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::clear()
	{
		mDataStream.setNull();
		mMesh = nullptr;
		
		mMrmVersion = 0;
		mMeshData.clear();
		mMeshDecl.numMaterials = 0;
		mMeshDecl.numVertices = 0;
		mMeshDecl.numNormals = 0;
		mSubMeshDecls.clear();
		mMemoryStreamWithMaterials.setNull();
		mBounds.setNull();

		mVertices = nullptr;
		mNumVertices = 0;
		mNormals = nullptr;
		mNumNormals = 0;
		mSubMeshInfos.clear();
		mMeshResolutions.clear();
	}
	//---------------------------------------------------------------------------------
	void MrmFileLoadContext::setLodConfig(const LodConfig& _lodLevels)
	{
		mLodConfig = _lodLevels;
	}
	//---------------------------------------------------------------------------------
	const MrmFileLoadContext::LodConfig& MrmFileLoadContext::getLodConfig() const
	{
		return mLodConfig;
	}
	//---------------------------------------------------------------------------------
	MrmFileLoadContext::MrmFileLoadContext()
	{
		mLodConfig.push_back(LodLevel(0, 1.00f));
	
	/*	mLodConfig.push_back(LodLevel(1, 0.90f));
		mLodConfig.push_back(LodLevel(2, 0.80f));
		mLodConfig.push_back(LodLevel(3, 0.70f));
		mLodConfig.push_back(LodLevel(4, 0.60f));
		mLodConfig.push_back(LodLevel(5, 0.50f));
		mLodConfig.push_back(LodLevel(6, 0.40f));
		mLodConfig.push_back(LodLevel(7, 0.30f));
	*/
		clear();
	}
	//---------------------------------------------------------------------------------
	MrmFileLoadContext::~MrmFileLoadContext()
	{
	}


} // namespace GothOgre