#include "Precompiled.h"
#include "MshFileLoadContext.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------------
	MshFileLoadContext::Chunk::Chunk(const DataStreamExPtr& _dataStream)
	{
		mDataStream = _dataStream;
		readHeader();
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::Chunk::readHeader()
	{
		mChunkPos = mDataStream->tellg();
		mDataStream->readData(&mChunkID, sizeof(mChunkID) + sizeof(mDataSize));
	}
	//---------------------------------------------------------------------------------
	MshFileLoadContext::Chunk::~Chunk()
	{
		skipRest();
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::Chunk::skipRest()
	{
		size_t newPos = mChunkPos + mDataSize + 6;
		mDataStream->seekg(newPos);
	}
	//---------------------------------------------------------------------------------
	time_t MshFileLoadContext::readDate()
	{
		Date date;
		mDataStream->readData(date);
		tm zt;
		zt.tm_sec = date.second;
		zt.tm_min = date.minute;
		zt.tm_hour = date.hour;
		zt.tm_mday = date.day;
		zt.tm_mon = date.month - 1;
		zt.tm_year = date.year - 1900;
		return mktime(&zt);
	}
	//---------------------------------------------------------------------------------
	AxisAlignedBox MshFileLoadContext::readAxisAlignedBox()
	{
		AABB aabb;
		mDataStream->readData(aabb);
		return AxisAlignedBox(aabb.min.x, aabb.min.y, aabb.min.z, 
									aabb.max.x, aabb.max.y, aabb.max.z);
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readFileInfo(Chunk& _parentChunk)
	{
		mDataStream->readData(mMshVersion);
		if(mMshVersion != 0x0009 && mMshVersion != 0x0109)
		{
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - Msh version " << mMshVersion << " is not supported.");
		}
		mCreationDate = readDate();
		mSourceFilename = mDataStream->readLine(EOL::LF);
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readBounds(Chunk& _parentChunk)
	{
		mBounds = readAxisAlignedBox();
		// objectOrientedBox? - skipped
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readMaterials(Chunk& _parentChunk)
	{
		size_t sz = _parentChunk.getDataSize();
		void* p = OGRE_ALLOC_T(uchar, sz, MEMCATEGORY_GENERAL);
		mDataStream->readData(p, sz);
		mMemoryStreamWithMaterials = DataStreamExPtr( new MemoryDataStream(p, sz, true) );
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readLightMaps(Chunk& _parentChunk)
	{
		// light maps?
		// skipped
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readLightMapIndices(Chunk& _parentChunk)
	{
		// light map's indices?
		// skipped
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readVertices(Chunk& _parentChunk)
	{
		ulong numVertices;
		mDataStream->readData(numVertices);
		mVertices.resize(numVertices);
		mDataStream->readData(&mVertices[0], numVertices * sizeof(Vec3));
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readVertFeatures(Chunk& _parentChunk)
	{
		ulong numVertFeatures;
		mDataStream->readData(numVertFeatures);
		mVertFeatures.resize(numVertFeatures); 
		mDataStream->readData(&mVertFeatures[0], numVertFeatures * sizeof(VertFeature));
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::readPolygons(Chunk& _parentChunk)
	{
		// Read number of polygons.
		ulong numPolygons;
		mDataStream->readData(numPolygons);
		mPolygons.reserve(numPolygons);

		// Different formats of polygons are used in Gothic 1 and Gothic 2.
		if(mMshVersion == 0x0109) // Gothic 2
		{
			size_t polyBufSize = _parentChunk.getDataSize() - sizeof(ulong);
			mPolygonBuffer.resize(polyBufSize);
			mDataStream->readData(&mPolygonBuffer[0], polyBufSize);

			void* p = &mPolygonBuffer[0];
			for(ulong i = 0; i != numPolygons; ++i)
			{
				Polygon* poly = (Polygon*) p;
				p = (void*) ( (size_t) p + 24 + 8 * poly->numPolyVerts );
				mPolygons.push_back(poly);
			}
		}
		else if(mMshVersion == 0x0009) // Gothic 1
		{
			size_t tempsz = _parentChunk.getDataSize() - sizeof(ulong);
			mPoly1Buffer.resize(tempsz);
			mDataStream->readData(&mPoly1Buffer[0], tempsz);

			ulong allVerts = (tempsz - 25 * numPolygons) / 6;
			size_t polyBufSize = 24 * numPolygons + 8 * allVerts;
			mPolygonBuffer.resize(polyBufSize);

			void* p1 = &mPoly1Buffer[0];
			void* p = &mPolygonBuffer[0];
			for(ulong i = 0; i != numPolygons; ++i)
			{
				Polygon_G1* poly1 = (Polygon_G1*) p1;
				Polygon* poly = (Polygon*) p;
				poly->matIndex = poly1->matIndex;
				poly->plane = poly1->plane;
				poly->numPolyVerts = poly1->numPolyVerts;
				for(uchar j = 0; j != poly->numPolyVerts; ++j)
				{
					poly->polyVerts[j].vertexIndex = poly1->polyVerts[j].vertexIndex;
					poly->polyVerts[j].vertFeatureIndex = poly1->polyVerts[j].vertFeatureIndex;
				}
				if(true)
				{
					mPolygons.push_back(poly);
					p  = (void*) ((size_t) p + 24 + 8 * poly->numPolyVerts);
				}
				p1 = (void*) ((size_t) p1 + 25 + 6 * poly->numPolyVerts);
			}
		}
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::createMaterials()
	{
		// materials is read as zen archive.
		ZenArchive ar;
		ar.readHeader(mMemoryStreamWithMaterials, ZenArchiveFlags::NO_LOG);
		
		SceneManagerEx* sceneMgr = SceneManagerExEnumerator::getSingleton().getActiveSceneManager();
		ulong numMtls = ar.readInt(StringUtil::BLANK);
		mSubMeshInfos.resize(numMtls);

		for(ulong i = 0; i != numMtls; ++i)
		{
			String mtlName = ar.readString(StringUtil::BLANK);
			String sectorName = StringUtil::BLANK;
			if(MtlNameUtil::isSectorFloorName(mtlName))
			{
				std::pair<String, String> strPair = MtlNameUtil::splitSectorFloorName(mtlName);
				sectorName = strPair.first;
				mtlName = strPair.second;
			}

			MtlPtr& mtl = mSubMeshInfos[i].mtl;
			mtl = MtlManagerEx::getSingleton().findByName(mtlName);
			if(mtl.isNull())
			{
				ZenObjectPtr zenmtl = ar.readObject();
				mtl = MtlManagerEx::getSingleton().findByName(mtlName);
			}
			else
			{
				ar.skipObject();
			}
			
			MatGenParams& matGenParams = mSubMeshInfos[i].matGenParams;
			matGenParams.setSectorName(sectorName);
		}
		ar.readFooter();
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::calculateSubMeshInfos()
	{
		// Initialise number of indices for each submesh (set it to zero).
		size_t i, j;
		for(i = 0; i != mSubMeshInfos.size(); ++i)
		{
			mSubMeshInfos[i].numIndices = 0;
		}

		// Count number of indices for each submesh.
		for(j = 0; j != mPolygons.size(); ++j)
		{
			Polygon* poly = mPolygons[j];
			i = poly->matIndex;
			if(i < mSubMeshInfos.size())
			{
				ulong npv = poly->numPolyVerts;
				ulong numFaces = npv - 2;
				mSubMeshInfos[i].numIndices += numFaces * 3;
			}
		}

		// Set starts of indices per each submesh.
		size_t curPos = 0;
		for(i = 0; i != mSubMeshInfos.size(); ++i)
		{
			mSubMeshInfos[i].firstIndex = curPos;
			curPos += mSubMeshInfos[i].numIndices;
		}
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::calculateBounds()
	{
		if(mBounds.getMinimum() == mBounds.getMaximum())
		{
			mBounds.setNull();
			for(size_t i = 0; i != mVertices.size(); ++i)
				mBounds.merge( mVertices[i].toVector3() );
		}
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::createSharedVertices()
	{
		// Create buffer.
		mSharedVertexBuffer = HardwareBufferManager::getSingleton().createVertexBuffer(
			32 /* 12(pos) + 12(normal) + 8(uv) */, 
			mVertFeatures.size(), HardwareBuffer::HBU_STATIC, false);

		float* bufferPtr = (float*)mSharedVertexBuffer->lock(HardwareBuffer::HBL_DISCARD);
		
		// Write normals and texture coordinates to the buffer.
		for(ulong i = 0; i != mVertFeatures.size(); ++i)
		{
			float* p = &bufferPtr[i * 8];
			p[0] = Math::NEG_INFINITY; // pos.x - will be initialised later
			p[1] = Math::NEG_INFINITY; // pos.y - will be initialised later
			p[2] = Math::NEG_INFINITY; // pos.z - will be initialised later
			p[3] = mVertFeatures[i].normal.x;
			p[4] = mVertFeatures[i].normal.z; // y <-> z
			p[5] = mVertFeatures[i].normal.y;
			p[6] = mVertFeatures[i].uvMapping.u;
			p[7] = mVertFeatures[i].uvMapping.v;
		}

		// Write vertex positions to the buffer.
		for(ulong j = 0; j != mPolygons.size(); ++j)
		{
			Polygon* poly = mPolygons[j];
			for(uchar k = 0; k != poly->numPolyVerts; ++k)
			{
				PolyVert& polyVert = poly->polyVerts[k];
				ulong vertexIndex = polyVert.vertexIndex;
				ulong vertFeatureIndex = polyVert.vertFeatureIndex;
				float* p = &bufferPtr[vertFeatureIndex * 8];
				if(p[0] == Math::NEG_INFINITY)
				{
					p[0] = mVertices[vertexIndex].x;
					p[1] = mVertices[vertexIndex].z; // y <-> z
					p[2] = mVertices[vertexIndex].y;
				}
				else
				{
					GOTHOGRE_ASSERT(p[0] == mVertices[vertexIndex].x
						&& p[1] == mVertices[vertexIndex].z
						&& p[2] == mVertices[vertexIndex].y, "MshFileLoadContext: Two feature vertices correspond the same vertex!");
				}
			}
		}

		mSharedVertexBuffer->unlock();
	
		// Initialise shared vertex data
		VertexData* vertexData = new VertexData();
		mMesh->sharedVertexData = vertexData;
		vertexData->vertexStart = 0;
		vertexData->vertexCount = mVertFeatures.size();

		vertexData->vertexDeclaration->addElement(0, 0, 
			VET_FLOAT3, VES_POSITION, 0);
		vertexData->vertexDeclaration->addElement(1, 12, 
			VET_FLOAT3, VES_NORMAL, 0);
		vertexData->vertexDeclaration->addElement(2, 24, 
			VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

		vertexData->vertexBufferBinding->setBinding(0, mSharedVertexBuffer);
		vertexData->vertexBufferBinding->setBinding(1, mSharedVertexBuffer);
		vertexData->vertexBufferBinding->setBinding(2, mSharedVertexBuffer);
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::createSharedIndices()
	{
		// Calculate total number of indices.
		mTotalNumIndices = 0;
		for(size_t i = 0; i != mSubMeshInfos.size(); ++i)
		{
			mTotalNumIndices += mSubMeshInfos[i].numIndices;
			mSubMeshInfos[i].numIndices = 0; // for the following cycle
		}

		mSharedIndexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_32BIT, mTotalNumIndices, 
			HardwareBuffer::HBU_STATIC, false);

		uint32* bufferPtr = (uint32*) mSharedIndexBuffer->lock(HardwareBuffer::HBL_DISCARD);

		// Fill the index buffer with indices.
		for(size_t j = 0; j != mPolygons.size(); ++j)
		{
			Polygon* poly = mPolygons[j];
			size_t i = poly->matIndex;
			if(i < mSubMeshInfos.size())
			{
				uint32* indices = &bufferPtr[mSubMeshInfos[i].firstIndex];
				size_t& numIndices = mSubMeshInfos[i].numIndices;
				ulong npv = poly->numPolyVerts;
				for(uchar k = 1; k != npv - 1; ++k)
				{
					indices[numIndices++] = poly->polyVerts[0].vertFeatureIndex;
					indices[numIndices++] = poly->polyVerts[k].vertFeatureIndex;
					indices[numIndices++] = poly->polyVerts[k+1].vertFeatureIndex;
				}
			}
		}

		mSharedIndexBuffer->unlock();
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::createSubMeshes()
	{
		for(ulong i = 0; i != mSubMeshInfos.size(); ++i)
		{
			ulong numIndices = mSubMeshInfos[i].numIndices;
			if(numIndices != 0)
			{
				SubMeshEx* subMesh = mMesh->createSubMesh();
				subMesh->setMtl( mSubMeshInfos[i].mtl );
				subMesh->setMatGenParams( mSubMeshInfos[i].matGenParams );

				subMesh->useSharedVertices = true;

				IndexData* indexData = subMesh->indexData;
				indexData->indexBuffer = mSharedIndexBuffer;
				indexData->indexStart = mSubMeshInfos[i].firstIndex;
				indexData->indexCount = mSubMeshInfos[i].numIndices;
			}
		}
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::setBounds()
	{
		// y <-> z
		AxisAlignedBox bounds2 (mBounds.getMinimum().x, mBounds.getMinimum().z, mBounds.getMinimum().y, 
			mBounds.getMaximum().x, mBounds.getMaximum().z, mBounds.getMaximum().y );

		mMesh->_setBounds(bounds2, false);
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::load(const DataStreamExPtr& _dataStream)
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

			if(numChunks == 1 && chunk.getID() != 0xB000)
				GOTHOGRE_EXCEPT(mDataStream->getName() << " - File has not a compiled mesh (*.msh).");

			switch(chunk.getID())
			{
				case 0xB000: readFileInfo(chunk); break;
				case 0xB010: readBounds(chunk); break;
				case 0xB020: readMaterials(chunk); break;
				case 0xB025: readLightMaps(chunk); break;
				case 0xB026: readLightMapIndices(chunk); break;
				case 0xB030: readVertices(chunk); break;
				case 0xB040: readVertFeatures(chunk); break;
				case 0xB050: readPolygons(chunk); break;
				case 0xB060: finish = true; break;
			}
		}
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::apply(Resource* _destMesh)
	{
		mMesh = static_cast<MeshEx*>(_destMesh);
		createMaterials();
		calculateSubMeshInfos();
		calculateBounds();
		createSharedVertices();
		createSharedIndices();
		createSubMeshes();
		setBounds();
		clear();
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::skip(const DataStreamExPtr& _dataStream)
	{
		clear();
		mDataStream = _dataStream;
		ulong numChunks = 0;
		while(true)
		{
			Chunk chunk(mDataStream);
			++numChunks;

			if(numChunks == 1 && chunk.getID() != 0xB000)
				GOTHOGRE_EXCEPT(mDataStream->getName() << " - File has not a compiled mesh (*.msh).");

			if(chunk.getID() == 0xB060)
				break;
		}
	}
	//---------------------------------------------------------------------------------
	void MshFileLoadContext::clear()
	{
		mDataStream.setNull();
		mMesh = nullptr;
		mMshVersion = 0;
		mCreationDate = 0;
		mSourceFilename.clear();
		mBounds.setNull();
		mMemoryStreamWithMaterials.setNull();

		mVertices.resize(0);
		mVertFeatures.resize(0);
		mPolygons.resize(0);
		mPolygonBuffer.resize(0);
		mPoly1Buffer.resize(0);
		mSubMeshInfos.resize(0);
		mSharedVertexBuffer.setNull();
		mSharedIndexBuffer.setNull();
	}
	//---------------------------------------------------------------------------------
	MshFileLoadContext::MshFileLoadContext()
	{
		clear();
	}
	//---------------------------------------------------------------------------------
	MshFileLoadContext::~MshFileLoadContext()
	{
	}

}