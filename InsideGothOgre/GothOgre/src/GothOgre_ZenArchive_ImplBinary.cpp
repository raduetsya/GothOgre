#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenArchive_ImplBinary.h"

namespace GothOgre
{

	ZenArchiveImplBinary::ZenArchiveImplBinary()
	{
	}
	//----------------------------------------------------------------------------------
	ZenArchiveImplBinary::~ZenArchiveImplBinary()
	{

	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::readHeader()
	{
		// read number of objects
		mDataStream->readLine(mCurrentLine, EOL::LF);
		size_t delimpos = mCurrentLine.find(' ');
		mTempStr = mCurrentLine.substr(0, delimpos);
		if(mTempStr != "objects")
			GOTHOGRE_EXCEPT(mName << " - Keyword 'objects' expected");

		StrStream ss( mCurrentLine.substr(delimpos) );
		size_t numObjects;
		ss >> numObjects;
		mObjectReadList->resize(numObjects);

		// skip "END"
		mDataStream->readLine(mCurrentLine, EOL::LF);
		if(mCurrentLine != "END")
			GOTHOGRE_EXCEPT(mName << " - Keyword 'END' expected");
		
		// skip empty line
		mDataStream->readLine(mCurrentLine, EOL::LF);
		if(!mCurrentLine.empty())
			GOTHOGRE_EXCEPT(mName << " - Empty line after 'END' expected");
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::readFooter()
	{
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeHeader()
	{
		mDataStream->writeString("objects ");
		mPosNumObjects = mDataStream->tellp();
		mDataStream->writeLine("0        ", EOL::LF);
		mDataStream->writeLine("END", EOL::LF);
		mDataStream->writeLine(StringUtil::BLANK, EOL::LF);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeFooter()
	{
		size_t pos = mDataStream->tellp();
		mDataStream->seekp(mPosNumObjects);

		// записываем в заголовок число объектов
		StrStream ss;
		ss << mObjectWriteMap->size();
		mDataStream->writeString( ss.str() );
		
		mDataStream->seekp(pos);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeInt(const String& _entryName, int _value)
	{
		mDataStream->writeData(&_value);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeByte(const String& _entryName, uchar _value)
	{
		mDataStream->writeData(&_value);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeWord(const String& _entryName, ushort _value)
	{
		mDataStream->writeData(&_value);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeBool(const String& _entryName, bool _value)
	{
		uchar val1 = (uchar) _value;
		mDataStream->writeData(&val1);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeFloat(const String& _entryName, float _value)
	{
		mDataStream->writeData(&_value);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeString(const String& _entryName, const String& _value)
	{
		mDataStream->writeLine(_value, EOL::NULLCHAR);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeVec3(const String& _entryName, const Vec3& _value)
	{
		mDataStream->writeData(&_value);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeColor(const String& _entryName, const Color& _value)
	{
		mDataStream->writeData(&_value);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeRaw(const String& _entryName, const void* _rawData, size_t _sizeInBytes)
	{
		mDataStream->writeData(_rawData, _sizeInBytes);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeRawFloat(const String& _entryName, const float* _rawData, size_t _sizeInBytes)
	{
		mDataStream->writeData(_rawData, _sizeInBytes);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeEnum(const String& _entryName, int _value)
	{
		GOTHOGRE_ASSERT(0 <= _value && _value <= 255, mName << " - Enum value " 
			<< _entryName << " is out of range [0..255]");
		uchar val1 = (uchar) _value;
		mDataStream->writeData(&val1);
	}
	//----------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchiveImplBinary::writeChunkBegin(const String& _chunkName, const String& _chunkType, ushort _objectVersion, ulong _objectIndex)
	{
		Chunk* chunk = new Chunk;
		chunk->mChunkName = _chunkName;
		chunk->mChunkType = _chunkType;
		chunk->mObjectVersion = _objectVersion;
		chunk->mObjectIndex = _objectIndex;
		chunk->mChunkPos = mDataStream->tellp();
		
		ulong chunkSize = 0;
		mDataStream->writeData(&chunkSize);

		mDataStream->writeData(&_objectVersion);
		mDataStream->writeData(&_objectIndex);
		mDataStream->writeLine(_chunkName, EOL::NULLCHAR);
		mDataStream->writeLine(_chunkType, EOL::NULLCHAR);

		chunk->mDataPos = mDataStream->tellp();
		return chunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::writeChunkEnd(ZenArchive::Chunk* _chunk)
	{
		// вычисляем размер чанка и записываем его в архив
		size_t endPos = mDataStream->tellp();
		ulong chunkSize = endPos - _chunk->getChunkPos();
		mDataStream->seekp(_chunk->getChunkPos());
		mDataStream->writeData(&chunkSize);
		mDataStream->seekp(endPos);
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readInt(const String& _entryName, int& _outValue)
	{
		mDataStream->readData(_outValue);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readByte(const String& _entryName, uchar& _outValue)
	{
		mDataStream->readData(_outValue);
		return true;
	}		
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readWord(const String& _entryName, ushort& _outValue)
	{
		mDataStream->readData(_outValue);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readFloat(const String& _entryName, float& _outValue)
	{
		mDataStream->readData(_outValue);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readBool(const String& _entryName, bool& _outValue)
	{
		uint8 val1;
		mDataStream->readData(val1);
		_outValue = (val1 != 0);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readString(const String& _entryName, String& _outValue)
	{
		mDataStream->readLine(_outValue, EOL::NULLCHAR);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readVec3(const String& _entryName, Vec3& _outValue)
	{
		mDataStream->readData(_outValue);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readColor(const String& _entryName, Color& _outValue)
	{
		mDataStream->readData(_outValue);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinary::readEnum(const String& _entryName, int& _outValue)
	{
		uchar val1;
		mDataStream->readData(val1);
		_outValue = val1;
		return true;
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplBinary::readRaw(const String& _entryName, void* _rawData, size_t _sizeInBytes)
	{
		mDataStream->readData(_rawData, _sizeInBytes);
		return _sizeInBytes;
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplBinary::readRawFloat(const String& _entryName, float* _rawData, size_t _sizeInBytes)
	{
		mDataStream->readData(_rawData, _sizeInBytes);
		return _sizeInBytes;
	}
	//----------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchiveImplBinary::readChunkBegin(const String& _chunkName)
	{
		Chunk* chunk = new Chunk;
		chunk->mChunkPos = mDataStream->tellg();
		mDataStream->readData(chunk->mChunkSize);	
		mDataStream->readData(chunk->mObjectVersion);	
		mDataStream->readData(chunk->mObjectIndex);
		mDataStream->readLine(chunk->mChunkName, EOL::NULLCHAR);
		mDataStream->readLine(chunk->mChunkType, EOL::NULLCHAR);
		chunk->mDataPos = mDataStream->tellg();
		return chunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinary::readChunkEnd(ZenArchive::Chunk* _chunk)
	{
		Chunk* chunk = static_cast<Chunk*>(_chunk);
		mDataStream->seekg(_chunk->getChunkPos() + chunk->mChunkSize);
	}

} // namespace GothOgre

