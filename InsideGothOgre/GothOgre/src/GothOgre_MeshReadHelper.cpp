#include "GothOgre_Precompiled.h"
#include "GothOgre_MeshReadHelper.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	// MeshReadHelper
	//-----------------------------------------------------------------------
	MeshReadHelper::MeshReadHelper()
	{
		init();
	}
	//-----------------------------------------------------------------------
	MeshReadHelper::MeshReadHelper(const MeshPtr& _mesh)
	{
		init();
		setMesh(_mesh);
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::init()
	{
		mIndexData = nullptr;
		mIndexPtr = nullptr;
		mVertexData = nullptr;
		mVertexPtr = nullptr;
		mTexCoordData = nullptr;
		mTexCoordPtr = nullptr;
	}
	//-----------------------------------------------------------------------
	MeshReadHelper::~MeshReadHelper()
	{
		unlockAllBuffers();
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::setMesh(const MeshPtr& _mesh)
	{
		if(mMesh != _mesh)
		{
			mMesh = _mesh;
			calculateMeshInfo();
			resetCurPointers();
		}
	}
	//-----------------------------------------------------------------------
	MeshPtr MeshReadHelper::getMesh() const
	{
		return mMesh;
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::unlockAllBuffers()
	{
		unlockIndexBuffer();
		unlockVertexBuffer();
		unlockTexCoordBuffer();
		resetCurPointers();
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::calculateMeshInfo()
	{
		mNumVertices = 0;
		mSubMeshInfos.resize(0);

		if(!mMesh.isNull())
		{
			size_t numSubMeshes = mMesh->getNumSubMeshes();
			mSubMeshInfos.reserve(numSubMeshes + 1);

			if(mMesh->sharedVertexData)
				mNumVertices += mMesh->sharedVertexData->vertexCount;

			for(unsigned short j = 0; j != numSubMeshes; ++j)
			{
				SubMesh* subMesh = mMesh->getSubMesh(j);
				mSubMeshInfos.push_back(SubMeshInfo());
				SubMeshInfo& smi = mSubMeshInfos.back();
				smi.subMesh = subMesh;

				if(subMesh->useSharedVertices)
				{
					smi.vertexData = mMesh->sharedVertexData;
					smi.vertexStartInJoinedList = 0;
				}
				else
				{
					smi.vertexData = subMesh->vertexData;
					smi.vertexStartInJoinedList = mNumVertices;
					mNumVertices += subMesh->vertexData->vertexCount;
				}
				smi.vertexStart = smi.vertexData->vertexStart;
				smi.vertexCount = smi.vertexData->vertexCount;
				smi.indexData = subMesh->indexData;
				smi.indexStart = smi.indexData->indexStart;
				smi.indexCount = smi.indexData->indexCount;
			}
		}

		// 
		mSubMeshInfos.push_back(SubMeshInfo());
		SubMeshInfo& smi = mSubMeshInfos.back();
		smi.subMesh = nullptr;
		smi.vertexData = nullptr;
		smi.vertexStart = 0;
		smi.vertexCount = 0;
		smi.vertexStartInJoinedList = mNumVertices;
		smi.indexData = nullptr;
		smi.indexStart = 0;
		smi.indexCount = 0;
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::resetCurPointers()
	{
		if(mSubMeshInfos.empty())
		{
			mCurSubMeshInfoV = nullptr;
			mCurSubMeshInfoT = nullptr;
			mCurSubMeshInfoF = nullptr;
			mCurSubMeshInfoFi = -1;
		}
		else
		{
			mCurSubMeshInfoV = &mSubMeshInfos.back();
			mCurSubMeshInfoT = &mSubMeshInfos.back();
			mCurSubMeshInfoF = &mSubMeshInfos.back();
			mCurSubMeshInfoFi = mSubMeshInfos.size() - 1;
		}
	}
	//-----------------------------------------------------------------------
	size_t MeshReadHelper::getNumVertices() const
	{
		return mNumVertices;
	}
	//-----------------------------------------------------------------------
	Vector3 MeshReadHelper::getVertex(size_t _vertexIndex)
	{
		size_t ofs = _vertexIndex - mCurSubMeshInfoV->vertexStartInJoinedList;
		if(ofs >= mCurSubMeshInfoV->vertexCount)
		{
			ofs = prepareVerticesGetOffset(_vertexIndex);
		}
		size_t indexInVD = ofs + mCurSubMeshInfoV->vertexStart;
		const uint8* ptr = (const uint8*) mVertexPtr + mVertexSize * indexInVD + mVertexOffset;
		return *(Vector3*) ptr;
	}
	//-----------------------------------------------------------------------
	size_t MeshReadHelper::prepareVerticesGetOffset(size_t _vertexIndex)
	{
		mCurSubMeshInfoV = nullptr;
		
		size_t ofs = _vertexIndex - mCurSubMeshInfoT->vertexStartInJoinedList;
		if(ofs < mCurSubMeshInfoT->vertexCount)
		{
			mCurSubMeshInfoV = mCurSubMeshInfoT;
		}
		else
		{
			ofs = _vertexIndex - mCurSubMeshInfoF->vertexStartInJoinedList;
			if(ofs < mCurSubMeshInfoF->vertexCount)
			{
				mCurSubMeshInfoV = mCurSubMeshInfoF;
			}
			else
			{
				for(size_t j = 0; j != mSubMeshInfos.size(); ++j)
				{
					SubMeshInfo* curSubMeshInfo = &mSubMeshInfos[j];
					ofs = _vertexIndex - curSubMeshInfo->vertexStartInJoinedList;
					if(ofs < curSubMeshInfo->vertexCount)
					{
						mCurSubMeshInfoV = curSubMeshInfo;
						break;
					}
				}
			}
		}

		if(!mCurSubMeshInfoV)
			GOTHOGRE_EXCEPT("Invalid index of vertex: " << _vertexIndex);

		lockVertexBuffer();
		return ofs;
	}
	//-----------------------------------------------------------------------
	bool MeshReadHelper::hasTextureCoords()
	{
		if(mCurSubMeshInfoT == &mSubMeshInfos.back())
		{
			prepareTexCoordsGetOffset(0);
		}
		return mTexCoordPtr != nullptr;
	}
	//-----------------------------------------------------------------------
	Vector2 MeshReadHelper::getTextureCoords(size_t _vertexIndex)
	{
		size_t ofs = _vertexIndex - mCurSubMeshInfoT->vertexStartInJoinedList;
		if(ofs >= mCurSubMeshInfoT->vertexCount)
		{
			ofs = prepareTexCoordsGetOffset(_vertexIndex);
		}
		size_t indexInVD = ofs + mCurSubMeshInfoT->vertexStart;
		const uint8* ptr = (const uint8*) mTexCoordPtr + mTexCoordSize * indexInVD + mTexCoordOffset;
		return *(Vector2*) ptr;
	}
	//-----------------------------------------------------------------------
	size_t MeshReadHelper::prepareTexCoordsGetOffset(size_t _vertexIndex)
	{
		mCurSubMeshInfoT = nullptr;
		
		size_t ofs = _vertexIndex - mCurSubMeshInfoV->vertexStartInJoinedList;
		if(ofs < mCurSubMeshInfoV->vertexCount)
		{
			mCurSubMeshInfoT = mCurSubMeshInfoV;
		}
		else
		{
			ofs = _vertexIndex - mCurSubMeshInfoF->vertexStartInJoinedList;
			if(ofs < mCurSubMeshInfoF->vertexCount)
			{
				mCurSubMeshInfoT = mCurSubMeshInfoF;
			}
			else
			{
				for(size_t j = 0; j != mSubMeshInfos.size(); ++j)
				{
					SubMeshInfo* curSubMeshInfo = &mSubMeshInfos[j];
					ofs = _vertexIndex - curSubMeshInfo->vertexStartInJoinedList;
					if(ofs < curSubMeshInfo->vertexCount)
					{
						mCurSubMeshInfoT = curSubMeshInfo;
						break;
					}
				}
			}
		}

		if(!mCurSubMeshInfoT)
			GOTHOGRE_EXCEPT("Invalid index of vertex: " << _vertexIndex);

		lockTexCoordBuffer();
		return ofs;
	}
	//-----------------------------------------------------------------------
	size_t MeshReadHelper::getNumSubMeshes() const
	{
		return mSubMeshInfos.size() - 1;
	}
	//-----------------------------------------------------------------------
	size_t MeshReadHelper::getNumFaces(size_t _subMeshIndex)
	{
		if(_subMeshIndex != mCurSubMeshInfoFi)
		{
			prepareFaces(_subMeshIndex);
		}
		return mCurSubMeshInfoF->indexCount / 3;
	}
	//-----------------------------------------------------------------------
	const String& MeshReadHelper::getMaterialName(size_t _subMeshIndex)
	{
		if(_subMeshIndex != mCurSubMeshInfoFi)
		{
			prepareFaces(_subMeshIndex);
		}
		return mCurSubMeshInfoF->subMesh->getMaterialName();
	}
	//-----------------------------------------------------------------------
	MeshReadHelper::Face MeshReadHelper::getFace(size_t _subMeshIndex, size_t _faceIndex)
	{
		if(_subMeshIndex != mCurSubMeshInfoFi)
		{
			prepareFaces(_subMeshIndex);
		}

		Face face;
		size_t indexInID = _faceIndex * 3 + mCurSubMeshInfoF->indexStart;
		switch(mIndexType)
		{
			case HardwareIndexBuffer::IT_16BIT:
			{
				face.i[0] = ((uint16*) mIndexPtr)[indexInID + 0];
				face.i[1] = ((uint16*) mIndexPtr)[indexInID + 1];
				face.i[2] = ((uint16*) mIndexPtr)[indexInID + 2];
				break;
			}
			case HardwareIndexBuffer::IT_32BIT:
			{
				face.i[0] = ((uint32*) mIndexPtr)[indexInID + 0];
				face.i[1] = ((uint32*) mIndexPtr)[indexInID + 1];
				face.i[2] = ((uint32*) mIndexPtr)[indexInID + 2];
				break;
			}
		}

		size_t ofs = mCurSubMeshInfoF->vertexStartInJoinedList - mCurSubMeshInfoF->vertexStart;
		face.i[0] += ofs;
		face.i[1] += ofs;
		face.i[2] += ofs;
		return face;
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::prepareFaces(size_t _subMeshIndex)
	{
		mCurSubMeshInfoFi = _subMeshIndex;
		mCurSubMeshInfoF = &mSubMeshInfos[_subMeshIndex];
		lockIndexBuffer();
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void MeshReadHelper::lockIndexBuffer()
	{
		IndexData* indexData = mCurSubMeshInfoF->indexData;
		if(indexData == mIndexData)
			return; // continue using the same IndexData

		if(!indexData)
		{
			unlockIndexBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mIndexData = indexData;
		mIndexStart = indexData->indexStart;
		mIndexCount = indexData->indexCount;
	
		HardwareIndexBufferSharedPtr indexBuffer = mIndexData->indexBuffer;
		if(indexBuffer == mIndexBuffer)
			return; // continue using the same hardware buffer

		unlockIndexBuffer();
		
		mIndexData = indexData;
		mIndexBuffer = indexBuffer;

		if(mIndexBuffer.isNull())
			return; // error, exiting with mIndexPtr == nullptr

		mIndexPtr = (uint8*) mIndexBuffer->lock( HardwareBuffer::HBL_READ_ONLY );
		mIndexSize = mIndexBuffer->getIndexSize();
		mIndexType = mIndexBuffer->getType();		
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::unlockIndexBuffer()
	{
		if(mIndexPtr)
		{
			mIndexBuffer->unlock();
			mIndexPtr = nullptr;
		}
		mIndexBuffer.setNull();
		mIndexData = nullptr;
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::lockVertexBuffer()
	{
		VertexData* vertexData = mCurSubMeshInfoV->vertexData;
		if(vertexData == mVertexData)
			return; // 

		if(!vertexData)
		{
			unlockVertexBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mVertexData = vertexData;

		const VertexElement* positionElem = mVertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);
		if(!positionElem)
		{
			unlockVertexBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mVertexOffset = positionElem->getOffset();
		mVertexType   = positionElem->getType();

		HardwareVertexBufferSharedPtr vertexBuffer = mVertexData->vertexBufferBinding->getBuffer(positionElem->getSource());
		if(vertexBuffer == mVertexBuffer)
			return; // continue using the same buffer

		unlockVertexBuffer();

		mVertexData = vertexData;
		mVertexBuffer = vertexBuffer;

		if(mVertexBuffer.isNull())
			return; // error, exiting with mIndexPtr == nullptr

		mVertexSize = mVertexBuffer->getVertexSize();

		assert(mVertexPtr == nullptr);
		if(mVertexBuffer == mTexCoordBuffer)
		{
			mVertexPtr = mTexCoordPtr;
		}
		else
		{
			mVertexPtr = mVertexBuffer->lock( HardwareBuffer::HBL_READ_ONLY );
		}
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::unlockVertexBuffer()
	{
		if(mVertexPtr)
		{
			if(mVertexBuffer != mTexCoordBuffer)
				mVertexBuffer->unlock();
			mVertexPtr = nullptr;
		}
		mVertexBuffer.setNull();
		mVertexData = nullptr;
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::lockTexCoordBuffer()
	{
		VertexData* texCoordData = mCurSubMeshInfoT->vertexData;
		if(texCoordData == mTexCoordData)
			return; // 

		if(!texCoordData)
		{
			unlockTexCoordBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mTexCoordData = texCoordData;

		const VertexElement* texCoordElem = mTexCoordData->vertexDeclaration->findElementBySemantic(VES_TEXTURE_COORDINATES);
		if(!texCoordElem)
		{
			unlockTexCoordBuffer();
			return; // error, exiting with mIndexPtr == nullptr
		}

		mTexCoordOffset = texCoordElem->getOffset();
		mTexCoordType   = texCoordElem->getType();

		HardwareVertexBufferSharedPtr texCoordBuffer = mTexCoordData->vertexBufferBinding->getBuffer(texCoordElem->getSource());
		if(texCoordBuffer == mTexCoordBuffer)
			return; // continue using the same buffer

		unlockTexCoordBuffer();

		mTexCoordData = texCoordData;
		mTexCoordBuffer = texCoordBuffer;

		if(mTexCoordBuffer.isNull())
			return; // error, exiting with mIndexPtr == nullptr

		mTexCoordSize = mTexCoordBuffer->getVertexSize();

		assert(mTexCoordPtr == nullptr);
		if(mTexCoordBuffer == mVertexBuffer)
		{
			mTexCoordPtr = mVertexPtr;
		}
		else
		{
			mTexCoordPtr = mTexCoordBuffer->lock( HardwareBuffer::HBL_READ_ONLY );
		}
	}
	//-----------------------------------------------------------------------
	void MeshReadHelper::unlockTexCoordBuffer()
	{
		if(mTexCoordPtr)
		{
			if(mTexCoordBuffer != mVertexBuffer)
				mTexCoordBuffer->unlock();
			mTexCoordPtr = nullptr;
		}
		mTexCoordBuffer.setNull();
		mTexCoordData = nullptr;
	}

} // namespace GothOgre