#include "Precompiled.h"
#include "3dsFileLoadContext.h"


namespace GothOgre
{

	_3dsFileLoadContext::Chunk::Chunk(const DataStreamExPtr& _dataStream)
	{
		mDataStream = _dataStream;
		mParentChunk = nullptr;
		readHeader();
	}
	//---------------------------------------------------------------------------------
	_3dsFileLoadContext::Chunk::Chunk(Chunk& _parentChunk)
	{
		mParentChunk = &_parentChunk;
		mDataStream = _parentChunk.mDataStream;
		readHeader();
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::Chunk::readHeader()
	{
		mChunkPos = mDataStream->tellg();
		mDataStream->readData(&mChunkID, sizeof(mChunkID) + sizeof(mChunkSize));
		GOTHOGRE_ASSERT(mChunkSize >= 6, "3dsFileLoader: Chunk size must be greater or equal 6. File: '" << mDataStream->getName() << "'");
		mDataSize = 0;
		mRestSize = mChunkSize - 6;
	}
	//---------------------------------------------------------------------------------
	_3dsFileLoadContext::Chunk::~Chunk()
	{
		skipRest();
		if(mParentChunk)
			mParentChunk->mRestSize -= mChunkSize;
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::Chunk::skipRest()
	{
		size_t newPos = mChunkPos + mChunkSize;
		mDataStream->seekg(newPos);
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::Chunk::setDataSize(size_t _dataSize) 
	{
		mRestSize += mDataSize; 
		mDataSize = _dataSize; 
		mRestSize -= mDataSize;
	}
	//---------------------------------------------------------------------------------
	ColourValue _3dsFileLoadContext::readColor(Chunk& _parentChunk)
	{
		ColourValue clr;
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			if(chunk.getID() == 0x0010)
			{
				float r, g, b;
				mDataStream->readData(r);
				mDataStream->readData(g);
				mDataStream->readData(b);
				clr.r = r;
				clr.g = g;
				clr.b = b;
			}
			else if(chunk.getID() == 0x0011)
			{
				uchar r, g, b;
				mDataStream->readData(r);
				mDataStream->readData(g);
				mDataStream->readData(b);
				clr.r = r / 255.0f;
				clr.g = g / 255.0f;
				clr.b = b / 255.0f;
			}
		}
		clr.a = 1;
		return clr;
	}
	//---------------------------------------------------------------------------------
	float _3dsFileLoadContext::readPercent(Chunk& _parentChunk)
	{
		float percent;
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			if(chunk.getID() == 0x0030)
			{
				ushort p;
				mDataStream->readData(p);
				percent = p / 100.0f;
			}
			else if(chunk.getID() == 0x0031)
			{
				float p;
				mDataStream->readData(p);
				percent = p / 100.0f;
			}
		}
		return percent;
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::read3dsVersion(Chunk& _parentChunk)
	{
		mDataStream->readData(m3dsVersion);
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readMeshVersion(Chunk& _parentChunk)
	{
		mDataStream->readData(mMeshVersion);
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readMaterialMap(Chunk& _parentChunk)
	{
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			if(chunk.getID() == 0xA300)
			{
				mCurrentMaterialMap->mappingFileName = mDataStream->readLine(EOL::NULLCHAR);
			}
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readMaterialBlock(Chunk& _parentChunk)
	{
		mMaterialInfos.push_back(MaterialInfo());
		mCurrentMaterialInfo = &mMaterialInfos.back();
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			switch(chunk.getID())
			{
				case 0xA000: mCurrentMaterialInfo->materialName = mDataStream->readLine(EOL::NULLCHAR); break;
				case 0xA020: mCurrentMaterialInfo->diffuseColor = readColor(chunk); break;
				case 0xA200: mCurrentMaterialMap = &mCurrentMaterialInfo->maps[TextureMap1]; readMaterialMap(chunk); break;
			}
		}
		mCurrentMaterialInfo = nullptr;
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readVertices(Chunk& _parentChunk)
	{
		ushort numVertices;
		mDataStream->readData(numVertices);
		mCurrentObject->vertices.resize(numVertices);
		if(numVertices != 0)
			mDataStream->readData(&mCurrentObject->vertices[0], numVertices * sizeof(Vec3));
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readMappingCoords(Chunk& _parentChunk)
	{
		ushort numTVerts;
		mDataStream->readData(numTVerts);
		mCurrentObject->tVerts.resize(numTVerts);
		if(numTVerts != 0)
			mDataStream->readData(&mCurrentObject->tVerts[0], numTVerts * sizeof(UVMapping));
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readFacesMtl(Chunk& _parentChunk)
	{
		String materialName = mDataStream->readLine(EOL::NULLCHAR);
		ushort numEntries;
		mDataStream->readData(numEntries);

		if(!numEntries)
			return;

		// search the material name in list of FacesMtls
		// (maybe the material was read before)
		list<FacesMtl>::type::iterator it;
		for(it = mCurrentObject->facesMtls.begin(); it != mCurrentObject->facesMtls.end(); ++it)
		{
			if(UnicodeUtil::equalsNoCase(it->materialName, materialName))
				break;
		}

		if(it == mCurrentObject->facesMtls.end())
		{
			// the FacesMtl is reading at the first time
			mCurrentObject->facesMtls.push_back(FacesMtl());
			FacesMtl& facesMtl = mCurrentObject->facesMtls.back();
			facesMtl.materialName = materialName;
			vector<ushort>::type& inds = facesMtl.faceIndices;
			inds.resize(numEntries);
			mDataStream->readData(&inds[0], numEntries * sizeof(ushort));
		}
		else
		{
			// the FacesMtl was read before, so we need to join two lists
			FacesMtl& facesMtl = *it;
			vector<ushort>::type& inds = facesMtl.faceIndices;
			size_t oldNumEntries = inds.size();
			inds.resize(oldNumEntries + numEntries);
			mDataStream->readData(&inds[oldNumEntries], numEntries * sizeof(ushort));
			
			// remove duplications
			std::sort(inds.begin(), inds.end());
			inds.erase(std::unique(inds.begin(), inds.end()), inds.end());
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readSmoothGroups(Chunk& _parentChunk)
	{
		size_t numFaces = mCurrentObject->faces.size();
		mCurrentObject->smoothGroups.resize(numFaces);
		if(numFaces != 0)
			mDataStream->readData(&mCurrentObject->smoothGroups[0], numFaces * sizeof(ulong));
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readFacesDescription(Chunk& _parentChunk)
	{
		ushort numFaces;
		mDataStream->readData(numFaces);
		mCurrentObject->faces.resize(numFaces);
		if(numFaces != 0)
			mDataStream->readData(&mCurrentObject->faces[0], numFaces * sizeof(Face));

		_parentChunk.setDataSize(2 + 8 * numFaces);
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			switch(chunk.getID())
			{
				case 0x4130: readFacesMtl(chunk); break;
				case 0x4150: readSmoothGroups(chunk); break;
			}
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readMesh(Chunk& _parentChunk)
	{
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			switch(chunk.getID())
			{
				case 0x4110: readVertices(chunk); break;
				case 0x4140: readMappingCoords(chunk); break;
				case 0x4120: readFacesDescription(chunk); break;
			}
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readObjectBlock(Chunk& _parentChunk)
	{
		mObjects.push_back(Object());
		mCurrentObject = &mObjects.back();
		
		String objectName = mDataStream->readLine(EOL::NULLCHAR, false);
		_parentChunk.setDataSize(objectName.length() + 1);
		StrUtil::trim(objectName);
		mCurrentObject->objectName = objectName;
		
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			switch(chunk.getID())
			{
				case 0x4100: readMesh(chunk); break;
			}
		}
		mCurrentObject = nullptr;
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::read3dEditor(Chunk& _parentChunk)
	{
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			switch(chunk.getID())
			{
				case 0x3D3E: readMeshVersion(chunk); break;
				case 0xAFFF: readMaterialBlock(chunk); break;
				case 0x4000: readObjectBlock(chunk); break;
			}
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::readMainChunk(Chunk& _parentChunk)
	{
		while(_parentChunk.hasMoreSubchunks())
		{
			Chunk chunk(_parentChunk);
			switch(chunk.getID())
			{
				case 0x0002: read3dsVersion(chunk); break;
				case 0x3D3D: read3dEditor(chunk); break;
			}
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::createMtls()
	{
		for(MaterialInfos::iterator it = mMaterialInfos.begin(); 
			it != mMaterialInfos.end(); ++it)
		{
			MtlPtr mtl = createMtlByMaterialInfo(&*it);
			mMtls.push_back(mtl);
		}
	}
	//---------------------------------------------------------------------------------
	MtlPtr _3dsFileLoadContext::createMtlByMaterialInfo(MaterialInfo* _matInfo)
	{
		const String& materialName = _matInfo->materialName;
		MtlPtr mtl = MtlManagerEx::getSingleton().findByName(materialName);
		if(mtl.isNull())
		{
			mtl = MtlManagerEx::getSingleton().createManual(materialName);
			mtl->setColour(_matInfo->diffuseColor);
			mtl->setTexture(_matInfo->maps[TextureMap1].mappingFileName);
		}
		return mtl;
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::calculateSubMeshInfos()
	{
		for(Objects::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
		{
			Object* obj = &*it;
			calculateSubMeshInfos(obj);
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::calculateSubMeshInfos(Object* obj)
	{
		size_t i, j, k;

		// Check size of array "Object::tVerts" and initialise the array 
		// if it has not been initialised yet.
		size_t numVertices = obj->vertices.size();
		if(obj->tVerts.size() != numVertices)
		{
			obj->tVerts.resize(numVertices);
			for(i = 0; i != numVertices; ++i)
			{
				UVMapping* uv = &obj->tVerts[i];
				uv->u = 0;
				uv->v = 0;
			}
		}

		// Initialise array "Object::vertInfos",
		// one vertInfo per one vertex.
		obj->vertInfos.resize(numVertices);
		for(i = 0; i != numVertices; ++i)
		{
			VertInfo* vertInfo = &obj->vertInfos[i];
			vertInfo->pos = &obj->vertices[i];
			vertInfo->uv = &obj->tVerts[i];
			vertInfo->firstVertFeature = nullptr;
		}

		// Determine whether smooth groups can be used for calculating normals.
		// If all smooth groups are zero or not exist
		// then smooth groups cannot be used and smooth angle must be used.
		bool useSmoothGroup = false;
		size_t numFaces = obj->faces.size();
		if(obj->smoothGroups.size() == numFaces)
		{
			for(i = 0; i != numFaces; ++i)
			{
				if(obj->smoothGroups[i] != 0)
					useSmoothGroup = true;
			}
		}

		// std::vector::reserve is used here with max possible size of vectors 
		// to prevent the following algorithm from memory reallocation.
		obj->vertFeatures.clear();
		obj->vertFeatures.reserve(numFaces * 3);

		obj->indexBuffer.clear();
		obj->indexBuffer.reserve(numFaces * 3);

		obj->subMeshInfos.clear();
		obj->subMeshInfos.reserve(obj->facesMtls.size());

		// For each material...
		i = 0;
		for(list<FacesMtl>::type::const_iterator it = obj->facesMtls.begin();
			it != obj->facesMtls.end(); ++it, ++i)
		{
			// Get list of faces and material.
			const FacesMtl* facesMtl = &*it;
			MtlPtr mtl = MtlManagerEx::getSingleton().findByName(facesMtl->materialName);
			if(!mtl.isNull() && !facesMtl->faceIndices.empty())
			{
				// Create one submesh per the material.
				obj->subMeshInfos.push_back(SubMeshInfo());
				SubMeshInfo* subMeshInfo = &obj->subMeshInfos.back();
				subMeshInfo->mtl = mtl;
				float cosSmoothAngle = (float) Math::Cos(mtl->getSmoothAngle());
				
				// The initial number of indices used by the submesh is zero
				// (this count will be increased later).
				subMeshInfo->numIndices = 0;

				// Store start of indices use by the submesh.
				ulong startIndex = obj->indexBuffer.size();

				// For each face of the facesMtl list ...
				for(j = 0; j != facesMtl->faceIndices.size(); ++j)
				{
					// Calculate normal to the face.
					ushort faceIndex = facesMtl->faceIndices[j];
					const Face* face = &obj->faces[faceIndex];
					ulong v[3] = {face->v[0], face->v[1], face->v[2]};

					Vec3 ab = obj->vertices[v[1]] - obj->vertices[v[0]];
					Vec3 bc = obj->vertices[v[2]] - obj->vertices[v[1]];
					Vec3 normal = ab.crossProduct(bc);
					float normal_len = normal.length();

					// If length of the face's normal is about zero
					// then the face is degenerate and must not be processed.
					if(normal_len > 1e-8f)
					{
						// Normalize the normal.
						normal /= normal_len;

						// Get the smooth group for fast access.
						ulong smoothGroup = useSmoothGroup ? obj->smoothGroups[faceIndex] : 0;

						// For each vertex of the face.
						for(k = 0; k != 3; ++k)
						{
							VertInfo* vertInfo = &obj->vertInfos[v[k]];

							// Create a new vert. feature for the vertex and fill its data.
							obj->vertFeatures.push_back(VertFeature());
							VertFeature* newVertFeature = &obj->vertFeatures.back();
							newVertFeature->vertInfo = vertInfo;
							newVertFeature->normal = normal;
							newVertFeature->numJoined = 1;
							newVertFeature->useSmoothGroup = useSmoothGroup;
							newVertFeature->smoothGroup = smoothGroup;
							newVertFeature->cosSmoothAngle = cosSmoothAngle;

							// Find an old vert. feature which can be joined with
							// the new vert. feature.
							VertFeature* vertFeature = vertInfo->firstVertFeature;
							ulong newIndex = -1;
							while(vertFeature)
							{
								if(canJoin(vertFeature, newVertFeature))
								{
									// Old vert. feature which can be joined is found, 
									// so we can join them and use the old vert. feature
									// instead of the new vert. feature.
									join(vertFeature, newVertFeature);
									obj->vertFeatures.pop_back();
									newIndex = ulong(vertFeature - &obj->vertFeatures[0]);
									break;
								}
								vertFeature = vertFeature->nextVertFeature;
							}
							if(newIndex == -1)
							{
								// Old vert. feature is not found,
								// so we must use the new vert. feature.
								newIndex = ulong(newVertFeature - &obj->vertFeatures[0]);
								newVertFeature->nextVertFeature = vertInfo->firstVertFeature;
								vertInfo->firstVertFeature = newVertFeature;
							}

							// New index for the submesh has been calculated, store it.
							obj->indexBuffer.push_back(newIndex);

							// Increase the number of indices used by the submesh.
							subMeshInfo->numIndices++;
						}
					}
				}

				// Post-process the submesh
				if(subMeshInfo->numIndices != 0)
				{
					// Store a pointer to start of indices used by the submesh.
					subMeshInfo->indices = &obj->indexBuffer[startIndex];
				}
				else
				{
					// The submesh has no indices, so the submesh is degenerate,
					// remove it from the list of submeshes.
					obj->subMeshInfos.pop_back();
				}
			}
		}

		// Normalize normals again for vert. features 
		// which were joined from several vert. features
		size_t numVertFeatures = obj->vertFeatures.size();
		for(i = 0; i != numVertFeatures; ++i)
		{
			VertFeature* vertFeature = &obj->vertFeatures[i];
			if(vertFeature->numJoined != 1)
			{
				float len = vertFeature->normal.length();
				vertFeature->normal /= len;
			}
		}
	}
	//---------------------------------------------------------------------------------
	bool _3dsFileLoadContext::canJoin(const VertFeature* vertFeature1, const VertFeature* vertFeature2)
	{
		// This function determines whether the specified vert. features can be joined,
		// i.e. their two normals can be replaces with one common normal.
		const Vec3& n1 = vertFeature1->normal;
		const Vec3& n2 = vertFeature2->normal;
		float cosAngle = n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;

		if(cosAngle > 0.99f) // cos(0) = 1, same normals -> always join
			return true;
		if(cosAngle < -0.99f) // cos(pi) = -1, opposite normals -> never join
			return false;

		if(vertFeature1->useSmoothGroup)
		{
			// smooth by smooth group
			return ((vertFeature1->smoothGroup & vertFeature1->smoothGroup) != 0);
		}
		else
		{
			// smooth by smooth angle
			return (vertFeature1->cosSmoothAngle == vertFeature2->cosSmoothAngle)
				&& (cosAngle > vertFeature1->cosSmoothAngle);
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::join(VertFeature* vertFeature1, const VertFeature* vertFeature2)
	{
		// Calculate common normal.
		vertFeature1->normal += vertFeature2->normal;

		// Calculate common smooth group.
		vertFeature1->smoothGroup |= vertFeature2->smoothGroup;
		vertFeature1->numJoined++;
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::calculateBounds()
	{
		// Calculate axis aligned box for all the objects
		mBounds.setNull();
		for(Objects::iterator it = mObjects.begin(); 
			it != mObjects.end(); ++it)
		{
			Object* obj = &*it;
			calculateBounds(obj);
			mBounds.merge(obj->bounds);
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::calculateBounds(Object* obj)
	{
		// Calculate axis aligned box for the specified objects
		obj->bounds.setNull();
		size_t numVertices = obj->vertices.size();
		for(size_t i = 0; i != numVertices; ++i)
			obj->bounds.merge(obj->vertices[i].toVector3());
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::createSharedVertices()
	{
		// Calculate number of vert. features in all the objects.
		size_t totalVertFeatures = 0;
		for(Objects::iterator it = mObjects.begin(); 
			it != mObjects.end(); ++it)
		{
			totalVertFeatures += it->vertFeatures.size();
		}

		// Prepare a buffer with positions, normals, and texture coordinates.
		HardwareVertexBufferSharedPtr buffer =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			32 /* 12(pos) + 12(normal) + 8(uv) */, totalVertFeatures, 
			HardwareBuffer::HBU_STATIC, false);

		float* bufferPtr = (float*)buffer->lock(HardwareBuffer::HBL_DISCARD);
		for(Objects::iterator it = mObjects.begin(); 
			it != mObjects.end(); ++it)
		{
			Object* obj = &*it;
			size_t numVertFeatures = obj->vertFeatures.size();
			for(size_t i = 0; i != numVertFeatures; ++i)
			{
				VertFeature* vertFeature = &obj->vertFeatures[i];
				VertInfo* vertInfo = vertFeature->vertInfo;
				Vec3* pos = vertInfo->pos;
				UVMapping* uv = vertInfo->uv;
				Vec3* normal = &(vertFeature->normal);
				*bufferPtr++ = pos->x;
				*bufferPtr++ = pos->y;
				*bufferPtr++ = pos->z;
				*bufferPtr++ = normal->x;
				*bufferPtr++ = normal->y;
				*bufferPtr++ = normal->z;
				*bufferPtr++ = uv->u;
				*bufferPtr++ = -uv->v;
			}
		}
		buffer->unlock();

		// Initialise shared vertex data.
		mMesh->sharedVertexData = new VertexData();
		mMesh->sharedVertexData->vertexStart = 0;
		mMesh->sharedVertexData->vertexCount = totalVertFeatures;

		mMesh->sharedVertexData->vertexDeclaration->addElement(0, 0, 
			VET_FLOAT3, VES_POSITION, 0);
		mMesh->sharedVertexData->vertexDeclaration->addElement(1, 12, 
			VET_FLOAT3, VES_NORMAL, 0);
		mMesh->sharedVertexData->vertexDeclaration->addElement(2, 24, 
			VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

		mMesh->sharedVertexData->vertexBufferBinding->setBinding(0, buffer);
		mMesh->sharedVertexData->vertexBufferBinding->setBinding(1, buffer);
		mMesh->sharedVertexData->vertexBufferBinding->setBinding(2, buffer);
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::createSubMeshes()
	{
		SceneManagerEx* sceneMgr = SceneManagerExEnumerator::getSingleton().getActiveSceneManager();

		// Create submeshes with information calculated 
		// by function _3dsFileLoadContext::calculateSubMeshInfos.
		ulong startVertFeatureIndex = 0;
		for(Objects::iterator it = mObjects.begin(); 
			it != mObjects.end(); ++it)
		{
			Object* obj = &*it;
			for(size_t j = 0; j != obj->subMeshInfos.size(); ++j)
			{
				SubMeshInfo* subMeshInfo = &obj->subMeshInfos[j];
				size_t numIndices = subMeshInfo->numIndices;

				HardwareIndexBufferSharedPtr buffer = 
					HardwareBufferManager::getSingleton().createIndexBuffer(
					HardwareIndexBuffer::IT_32BIT, numIndices, 
					HardwareBuffer::HBU_STATIC, false);

				ulong* bufferPtr = (ulong*)
					buffer->lock(HardwareBuffer::HBL_DISCARD);

				for(size_t k = 0; k != numIndices; ++k)
				{
					*bufferPtr++ = subMeshInfo->indices[k] + startVertFeatureIndex;
				}

				buffer->unlock();

				SubMeshEx* subMesh = mMesh->createSubMesh();
				subMesh->setMtl( subMeshInfo->mtl );
				subMesh->setMatGenParams( MatGenParams() );
				
				subMesh->useSharedVertices = true;

				IndexData* indexData = subMesh->indexData;
				indexData->indexStart = 0;
				indexData->indexCount = numIndices;
				indexData->indexBuffer = buffer;
			}
			startVertFeatureIndex += obj->vertFeatures.size();
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::setBounds()
	{
		mMesh->_setBounds(mBounds, false);
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::load(const DataStreamExPtr& _dataStream)
	{
		clear();
		mDataStream = _dataStream;
		EncodingSettings::getSingleton().setupStreamEncoding(mDataStream);

		Chunk chunk(mDataStream);
		if(chunk.getID() != 0x4D4D)
		{
			GOTHOGRE_EXCEPT(mDataStream->getName() << " - File is not a 3d studio mesh (*.3ds).");
		}
		readMainChunk(chunk);
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::skip(const DataStreamExPtr& _dataStream)
	{
		clear();
		mDataStream = _dataStream;
		{
			// Local scope for variable 'chunk'
			Chunk chunk(mDataStream);
			if(chunk.getID() != 0x4D4D)
			{
				GOTHOGRE_EXCEPT(mDataStream->getName() << " - File is not a 3d studio mesh (*.3ds).");
			}
		}
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::apply(Resource* _destMesh)
	{
		mMesh = static_cast<MeshEx*>(_destMesh);
		createMtls();
		calculateSubMeshInfos();
		calculateBounds();
		createSharedVertices();
		createSubMeshes();
		setBounds();
		clear();
	}
	//---------------------------------------------------------------------------------
	void _3dsFileLoadContext::clear()
	{
		mMesh = nullptr;
		m3dsVersion = 0;
		mMeshVersion = 0;
		mCurrentMaterialInfo = nullptr;
		mCurrentMaterialMap = nullptr;
		mCurrentObject = nullptr;
		mBounds.setNull();
		mMaterialInfos.clear();
		mMtls.clear();
		mObjects.clear();
	}
	//---------------------------------------------------------------------------------
	_3dsFileLoadContext::_3dsFileLoadContext()
	{
		clear();
	}
	//---------------------------------------------------------------------------------
	_3dsFileLoadContext::~_3dsFileLoadContext()
	{
	}

} // namespace GothOgre