#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenArchive_ImplBinSafe.h"


namespace GothOgre
{

	ZenArchiveImplBinSafe::HashTable::HashTable()
	{
		mSize = 0;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::HashTable::clear()
	{
		mSize = 0;
		for(size_t hashValue = 0; hashValue != HASH_TABLE_SIZE; ++hashValue)
			mTable[hashValue].clear();
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplBinSafe::HashTable::hashFunc(const String& _key)
	{
		size_t value = 0;
		for(size_t i = 0; i != _key.length(); ++i)
		{
			value = value * 33 + _key[i];
		}
		value %= HASH_TABLE_SIZE;
		return value;
	}
	//----------------------------------------------------------------------------------
	ushort ZenArchiveImplBinSafe::HashTable::find(const String& _key) const
	{
		size_t hashValue = hashFunc(_key);
		for(list<Entry>::type::const_iterator it = mTable[hashValue].begin();
			it != mTable[hashValue].end(); ++it)
		{
			const Entry& entry = *it;
			if(entry.mKey == _key)
				return entry.mInsertionIndex;
		}
		return -1;
	}
	//----------------------------------------------------------------------------------
	ushort ZenArchiveImplBinSafe::HashTable::insert(const String& _key)
	{
		// check if the key already exists in the hash table
		size_t hashValue = hashFunc(_key);
		for(list<Entry>::type::iterator it = mTable[hashValue].begin();
			it != mTable[hashValue].end(); ++it)
		{
			Entry& entry = *it;
			if(entry.mKey == _key)
				return entry.mInsertionIndex;
		}
		// if it does not exist, insert a new entry into the hash table
		mTable[hashValue].push_back(Entry());
		Entry& entry = mTable[hashValue].back();
		entry.mKey = _key;
		entry.mInsertionIndex = mSize++;
		return entry.mInsertionIndex;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::HashTable::write(const DataStreamExPtr& _dataStream)
	{
		_dataStream->writeData(&mSize);
		for(size_t hashValue = 0; hashValue != HASH_TABLE_SIZE; ++hashValue)
			for(list<Entry>::type::iterator it = mTable[hashValue].begin();
				it != mTable[hashValue].end(); ++it)
			{
				Entry& entry = *it;
				GOTHOGRE_ASSERT(entry.mKey.length() <= 65535, "ZenArchive: Entry has too long name (> 65535 characters)");
				GOTHOGRE_ASSERT(entry.mInsertionIndex <= 65535, "ZenArchive: Too many entries, cannot save archive in BIN_SAFE mode");
				DataStreamEx::ByteBuffer entryKeyBuffer;
				_dataStream->prepareString(entryKeyBuffer, entry.mKey);
				ushort keyLength = (ushort) entryKeyBuffer.size();
				_dataStream->writeData(&keyLength);
				_dataStream->writeData(&entry.mInsertionIndex);
				_dataStream->writeData(&hashValue);
				_dataStream->writeBuffer(entryKeyBuffer);
			}
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::HashTable::read(const DataStreamExPtr& _dataStream)
	{
		clear();
		ulong size4;
		_dataStream->readData(size4);
		mSize = (ushort) size4;
		for(size_t j = 0; j != mSize; ++j)
		{
			ushort keyLength, insertionIndex;
			size_t hashValue;
			_dataStream->readData(keyLength);
			_dataStream->readData(insertionIndex);
			_dataStream->readData(hashValue);
			mTable[hashValue].push_back(Entry());
			Entry& entry = mTable[hashValue].back();
			entry.mInsertionIndex = insertionIndex;
			_dataStream->readString(entry.mKey, keyLength);
		}
	}
	//----------------------------------------------------------------------------------
	ZenArchiveImplBinSafe::ZenArchiveImplBinSafe()
	{
		mGhostRootChunk = new Chunk;
		mCurrentChunk = mGhostRootChunk;
	}
	//----------------------------------------------------------------------------------
	ZenArchiveImplBinSafe::~ZenArchiveImplBinSafe()
	{
		if(mGhostRootChunk)
			delete mGhostRootChunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::readHeader()
	{
		mDataStream->readData(mBinSafeVersion);
		GOTHOGRE_ASSERT(mBinSafeVersion >= 2, "ZenArchive: '" << mName << "': Unsupported version of BIN_SAFE archive.");

		// read number of objects
		ulong numObjects = 0;
		mDataStream->readData(numObjects);
		mObjectReadList->resize(numObjects);

		// читаем смещение карты относительно начала файла
		ulong hashTableOffset = 0;
		mDataStream->readData(hashTableOffset);

		// сохраняем текущую позицию
		mStartPos = mDataStream->tellg();

		// читаем хэш-таблицу
		mDataStream->seekg(hashTableOffset);
		mHashTable.read(mDataStream);

		// возвращаемся на старое место для чтения содержимого архива
		mDataStream->seekg(mStartPos);

		mNumNotFoundEntries = 0;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::readFooter()
	{
		if(mNumNotFoundEntries != 0)
		{
			GOTHOGRE_WARNING(mName << " - Some entries (" << mNumNotFoundEntries << ") were not found.");
		}
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeHeader()
	{
		mDataStream->writeData(&mBinSafeVersion);
		
		mNumObjectsPos = mDataStream->tellp();
		ulong numObjects = 0;
		mDataStream->writeData(&numObjects);

		mHashTableOffsetPos = mDataStream->tellp();
		ulong hashTableOffset = 0;
		mDataStream->writeData(&hashTableOffset);

		mStartPos = mDataStream->tellp();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeFooter()
	{
		ulong hashTableOffset = mDataStream->tellp();
		mHashTable.write(mDataStream);
		size_t endPos = mDataStream->tellp();

		mDataStream->seekp(mHashTableOffsetPos);
		mDataStream->writeData(&hashTableOffset);

		ulong numObjects = mObjectWriteMap->size();
		mDataStream->seekp(mNumObjectsPos);
		mDataStream->writeData(&numObjects);

		mDataStream->seekp(endPos);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeInt(const String& _entryName, int _value)
	{
		writeEntry(_entryName, TYPE_INT, &_value, sizeof(_value));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeByte(const String& _entryName, uchar _value)
	{
		writeEntry(_entryName, TYPE_BYTE, &_value, sizeof(_value));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeWord(const String& _entryName, ushort _value)
	{
		writeEntry(_entryName, TYPE_WORD, &_value, sizeof(_value));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeBool(const String& _entryName, bool _value)
	{
		ulong val4 = _value;
		writeEntry(_entryName, TYPE_BOOL, &val4, sizeof(val4));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeFloat(const String& _entryName, float _value)
	{
		writeEntry(_entryName, TYPE_FLOAT, &_value, sizeof(_value));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeString(const String& _entryName, const String& _value)
	{
		DataStreamEx::ByteBuffer buf;
		mDataStream->prepareString(buf, _value);
		writeEntry(_entryName, TYPE_STRING, buf.data(), buf.size());
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeVec3(const String& _entryName, const Vec3& _value)
	{
		writeEntry(_entryName, TYPE_VEC3, &_value, sizeof(_value));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeColor(const String& _entryName, const Color& _value)
	{
		writeEntry(_entryName, TYPE_COLOR, &_value, sizeof(_value));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeRaw(const String& _entryName, const void* _rawData, size_t _sizeInBytes)
	{
		writeEntry(_entryName, TYPE_RAW, _rawData, _sizeInBytes);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeRawFloat(const String& _entryName, const float* _rawData, size_t _sizeInBytes)
	{
		writeEntry(_entryName, TYPE_RAW_FLOAT, _rawData, _sizeInBytes);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeEnum(const String& _entryName, int _value)
	{
		writeEntry(_entryName, TYPE_ENUM, &_value, sizeof(_value));
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeEntry(const String& _entryName, BinSafeType _type,
		const void* _data, size_t _size)
	{
		ushort ii = mHashTable.insert(_entryName);
		ulong ii4 = ii;
		writeValue(TYPE_INSERTION_INDEX, &ii4, sizeof(ii4));
		writeValue(_type, _data, _size);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeValue(BinSafeType _type, const void* _data, size_t _size)
	{
		writeTypeAndSize(_type, _size);
		mDataStream->writeData(_data, _size);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeTypeAndSize(BinSafeType _type, size_t _size)
	{
		uchar type1 = (uchar) _type;
		mDataStream->writeData(&type1);
		if(_type == TYPE_STRING || _type == TYPE_RAW || _type == TYPE_RAW_FLOAT)
		{
			GOTHOGRE_ASSERT(_size <= 65535, mName << " - Data size is too big (>65535) and cannot be saved to BIN_SAFE archive!");
			ushort size2 = (ushort) _size;
			mDataStream->writeData(&size2);
		}
	}
	//----------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchiveImplBinSafe::writeChunkBegin(const String& _chunkName, const String& _chunkType, ushort _objectVersion, ulong _objectIndex)
	{
		Chunk* chunk = new Chunk;
		chunk->mChunkName     = _chunkName;
		chunk->mChunkType     = _chunkType;
		chunk->mObjectVersion = _objectVersion;
		chunk->mObjectIndex   = _objectIndex;
		chunk->mChunkPos      = mDataStream->tellp();

		mOutStream.str(StringUtil::BLANK);
		mOutStream << '[';
		mOutStream << _chunkName;
		mOutStream << ' ';
		mOutStream << _chunkType;
		mOutStream << ' ';
		mOutStream << _objectVersion;
		mOutStream << ' ';
		mOutStream << _objectIndex;
		mOutStream << ']';
		String s = mOutStream.str();
		writeValue(TYPE_STRING, s.c_str(), s.length());

		chunk->mDataPos = mDataStream->tellp();
		return chunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::writeChunkEnd(ZenArchive::Chunk* _chunk)
	{
		static const String CHUNK_END = "[]";
		writeValue(TYPE_STRING, CHUNK_END.c_str(), CHUNK_END.length());
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readInt(const String& _entryName, int& _outValue)
	{
		return readEntry(_entryName, TYPE_INT, &_outValue, sizeof(int)) != 0;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readByte(const String& _entryName, uchar& _outValue)
	{
		return readEntry(_entryName, TYPE_BYTE, &_outValue, sizeof(uchar)) != 0;
	}		
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readWord(const String& _entryName, ushort& _outValue)
	{
		return readEntry(_entryName, TYPE_WORD, &_outValue, sizeof(ushort)) != 0;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readFloat(const String& _entryName, float& _outValue)
	{
		return readEntry(_entryName, TYPE_FLOAT, &_outValue, sizeof(_outValue)) != 0;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readBool(const String& _entryName, bool& _outValue)
	{
		uint32 val4;
		if(readEntry(_entryName, TYPE_BOOL, &val4, sizeof(val4)) != 0)
		{
			_outValue = (val4 != 0);
			return true;
		}
		return false;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readString(const String& _entryName, String& _outValue)
	{
		return readEntry(_entryName, TYPE_STRING, &_outValue, sizeof(String)) != 0;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readVec3(const String& _entryName, Vec3& _outValue)
	{
		return readEntry(_entryName, TYPE_VEC3, &_outValue, sizeof(Vec3)) != 0;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readColor(const String& _entryName, Color& _outValue)
	{
		return readEntry(_entryName, TYPE_COLOR, &_outValue, sizeof(Color)) != 0;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::readEnum(const String& _entryName, int& _outValue)
	{
		return readEntry(_entryName, TYPE_ENUM, &_outValue, sizeof(int)) != 0;
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplBinSafe::readRaw(const String& _entryName, void* _rawData, size_t _sizeInBytes)
	{
		return readEntry(_entryName, TYPE_RAW, _rawData, _sizeInBytes);
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplBinSafe::readRawFloat(const String& _entryName, float* _rawData, size_t _sizeInBytes)
	{
		return readEntry(_entryName, TYPE_RAW_FLOAT, _rawData, _sizeInBytes);
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplBinSafe::readEntry(const String& _entryName, BinSafeType _type, void* _data, size_t _size)
	{
		ushort insertionIndex = mHashTable.find(_entryName);
		size_t readSize = 0;
		bool found = false;

		// вначале ищем среди пропущенных параметров
		if(mCurrentChunk)
		{
			map<ushort, SkippedEntry>::type& ses = mCurrentChunk->mSkippedEntries;
			map<ushort, SkippedEntry>::type::iterator it = ses.find(insertionIndex);
			if(it != ses.end())
			{
				SkippedEntry& se = it->second;
				readSize = se.entryData.size();
				checkTypeAndSize(_entryName, _type, _size, se.entryType, readSize);
				if(_type == TYPE_STRING)
				{
					*(String*) _data = se.entryDataIfStr;
				}
				else
				{
					memcpy(_data, &se.entryData[0], readSize);
				}
				ses.erase(it);
				found = true;
			}
		}

		while(!found)
		{
			size_t curPos = mDataStream->tellg();

			BinSafeType readType1;
			size_t		readSize1;
			readTypeAndSize(readType1, readSize1);

			if(readType1 == TYPE_STRING)
			{
				mDataStream->readString(mCurrentLine, readSize1);
	
				// обработка чанков
				if(mCurrentLine.length() >= 2 && mCurrentLine[0] == '[' && mCurrentLine[mCurrentLine.length() - 1] == ']')
				{
					// встретился вложенный чанк [...]
					if(mCurrentLine.length() > 2)
					{
						if(parseChunkBegin())
						{
							// найдено начало какого-то чанка
							SkippedSubchunk& sc = mCurrentChunk->mSkippedSubchunks.insert(std::make_pair(mReadChunk.chunkName, SkippedSubchunk())).first->second;
							sc.chunkName = mReadChunk.chunkName;
							sc.chunkType = mReadChunk.chunkType;
							sc.chunkPos = curPos;
							sc.dataPos = mDataStream->tellg();
							sc.objectIndex = mReadChunk.objectIndex;
							sc.objectVersion = mReadChunk.objectVersion;
							findChunkEnd();							
						}
					}
					// дошли до конца родительского чанка []
					else
					{
						break;
					}
				}
			}
			else if(readType1 == TYPE_INSERTION_INDEX)
			{
				// нашли какой-то параметр, проверяем совпадение имен
				ulong ii4;
				mDataStream->readData(ii4);
				ushort ii = (ushort) ii4;

				BinSafeType readType2;
				size_t readSize2;
				readTypeAndSize(readType2, readSize2);

				if(ii == insertionIndex)
				{
					// имена совпали, требуемый параметр найден
					checkTypeAndSize(_entryName, _type, _size, readType2, readSize2);
					if(_type == TYPE_STRING)
					{
						mDataStream->readString(*(String*) _data, readSize2);
					}
					else
					{
						mDataStream->readData(_data, readSize2);
					}
					readSize = readSize2;
					found = true;
				}
				else
				{
					// имена не совпали - это не тот параметр
					SkippedEntry& se = mCurrentChunk->mSkippedEntries.insert(std::make_pair(ii, SkippedEntry())).first->second;
					se.entryType = readType2;
					if(readType2 == TYPE_STRING)
					{
						mDataStream->readString(se.entryDataIfStr, readSize2);
					}
					else
					{
						se.entryData.resize(readSize2);
						mDataStream->readData(&se.entryData[0], readSize2);
					}
				}
			}
			else
			{
				mDataStream->skipg(readSize1);
			}
		}

		if(!found)
		{
			// параметр не найден
			++mNumNotFoundEntries;
		}
		return readSize;
	}
	//----------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchiveImplBinSafe::readChunkBegin(const String& _chunkName)
	{
		Chunk* chunk = nullptr;

		// вначале ищем среди пропущенных чанков
		if(mCurrentChunk)
		{
			map<String, SkippedSubchunk>::type& scs = mCurrentChunk->mSkippedSubchunks;
			map<String, SkippedSubchunk>::type::iterator it;
			if(_chunkName.empty())
				it = scs.begin();
			else
				it = scs.find(_chunkName);

			if(it != scs.end())
			{
				SkippedSubchunk& sc = it->second;
				chunk = new Chunk;
				chunk->mChunkName     = sc.chunkName;
				chunk->mChunkType     = sc.chunkType;
				chunk->mChunkPos      = sc.chunkPos;
				chunk->mDataPos       = sc.dataPos;
				chunk->mObjectIndex   = sc.objectIndex;
				chunk->mObjectVersion = sc.objectVersion;
				chunk->mStoredPos     = mDataStream->tellg();
				mDataStream->seekg(sc.dataPos);
				scs.erase(it);
			}
		}

		while(!chunk)
		{
			size_t curPos = mDataStream->tellg();

			BinSafeType readType1;
			size_t		readSize1;
			readTypeAndSize(readType1, readSize1);

			if(readType1 == TYPE_STRING)
			{
				mDataStream->readString(mCurrentLine, readSize1);

				// нас интересуют только начала и концы чанков
				if(mCurrentLine.length() >= 2 && mCurrentLine[0] == '[' && mCurrentLine[mCurrentLine.length() - 1] == ']')
				{
					if(mCurrentLine.length() > 2) // [...] beginning of chunk
					{
						if(parseChunkBegin())
						{
							// найдено начало какого-то чанка;
							// проверяем, тот ли это чанк, который нужен
							if(_chunkName.empty() || _chunkName == mReadChunk.chunkName)
							{
								chunk = new Chunk;
								chunk->mChunkName = mReadChunk.chunkName;
								chunk->mChunkType = mReadChunk.chunkType;
								chunk->mChunkPos = curPos;
								chunk->mDataPos = mDataStream->tellg();
								chunk->mObjectIndex = mReadChunk.objectIndex;
								chunk->mObjectVersion = mReadChunk.objectVersion;
								chunk->mStoredPos = -1;
							}
							else
							{
								// это нет тот чанк, пропускаем
								SkippedSubchunk& sc = mCurrentChunk->mSkippedSubchunks.insert(std::make_pair(mReadChunk.chunkName, SkippedSubchunk())).first->second;
								sc.chunkName = mReadChunk.chunkName;
								sc.chunkType = mReadChunk.chunkType;
								sc.chunkPos = curPos;
								sc.dataPos = mDataStream->tellg();
								sc.objectIndex = mReadChunk.objectIndex;
								sc.objectVersion = mReadChunk.objectVersion;
								findChunkEnd();
							}
						}
					}
					else
					{
						// если дошли до конца родительского чанка
						mDataStream->seekg(curPos);
						break;
					}
				}
			}
			else if(readType1 == TYPE_INSERTION_INDEX)
			{
				// нашли какой-то параметр
				ulong ii4;
				mDataStream->readData(ii4);
				ushort ii = (ushort) ii4;

				BinSafeType readType2;
				size_t readSize2;
				readTypeAndSize(readType2, readSize2);

				if(mCurrentChunk)
				{
					SkippedEntry& se = mCurrentChunk->mSkippedEntries.insert(std::make_pair(ii, SkippedEntry())).first->second;
					se.entryType = readType2;
					if(readType2 == TYPE_STRING)
					{
						mDataStream->readString(se.entryDataIfStr, readSize2);
					}
					else
					{
						se.entryData.resize(readSize2);
						mDataStream->readData(&se.entryData[0], readSize2);
					}
				}
			}
			else
			{
				mDataStream->skipg(readSize1);
			}
		}

		if(chunk)
		{
			// found chunk, make it current
			mCurrentChunk = chunk;
		}
		return chunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::readChunkEnd(ZenArchive::Chunk* _chunk)
	{
		Chunk* chunk = static_cast<Chunk*>(_chunk);

		if(chunk->mStoredPos != -1)
			mDataStream->seekg(chunk->mStoredPos);
		else
			findChunkEnd();

		mCurrentChunk = static_cast<Chunk*>(chunk->getParent());
		if(!mCurrentChunk)
			mCurrentChunk = mGhostRootChunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::findChunkEnd()
	{
		int bracketLevel = 1;
		while(bracketLevel != 0)
		{
			BinSafeType readType1;
			size_t		readSize1;
			readTypeAndSize(readType1, readSize1);

			if(readType1 == TYPE_STRING)
			{
				mDataStream->readString(mCurrentLine, readSize1);
				if(mCurrentLine.length() >= 2 && mCurrentLine[0] == '[' && mCurrentLine[mCurrentLine.length() - 1] == ']')
				{
					if(mCurrentLine.length() > 2) // [...] // beginning of chunk
						bracketLevel++;
					else
						bracketLevel--; // [] end of chunk
				}
			}
			else
			{
				mDataStream->skipg(readSize1);
			}
		}
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::checkTypeAndSize(const String& _entryName, BinSafeType _type, size_t _size, BinSafeType _readType, size_t _readSize)
	{
		if(_type != _readType)
		{
			GOTHOGRE_EXCEPT(mName << " - Unexpected data type " << _readType
				<< " (expected: " << _type << ") for entry " << _entryName);
		}
		if(_type != TYPE_STRING)
		{
			if(_size < _readSize)
			{
				GOTHOGRE_EXCEPT(mName << " - Buffer (size = " << _size
					<< ") too small to receive read data (size = " << _readSize
					<< ") for entry " << _entryName);
			}
		}
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplBinSafe::readTypeAndSize(BinSafeType& _outType, size_t& _outSize)
	{
		uchar type1;
		mDataStream->readData(type1);
		_outType = (BinSafeType) type1;
		_outSize = 0;

		switch(_outType)
		{
			case TYPE_INT:
			{
				_outSize = sizeof(int);
				break;
			}
			case TYPE_FLOAT:
			{
				_outSize = sizeof(float);
				break;
			}
			case TYPE_BYTE:
			{
				_outSize = sizeof(uchar);
				break;
			}
			case TYPE_WORD:
			{
				_outSize = sizeof(ushort);
				break;
			}
			case TYPE_BOOL:
			{
				_outSize = sizeof(long);
				break;
			}
			case TYPE_VEC3:
			{
				_outSize = sizeof(float) * 3;
				break;
			}
			case TYPE_COLOR:
			{
				_outSize = sizeof(uchar) * 4;
				break;
			}
			case TYPE_STRING:
			case TYPE_RAW:
			case TYPE_RAW_FLOAT:
			{
				ushort size2;
				mDataStream->readData(size2);
				_outSize = size2;
				break;
			}
			case TYPE_ENUM:
			{
				_outSize = sizeof(int);
				break;
			}
			case TYPE_INSERTION_INDEX:
			{
				_outSize = sizeof(ulong);
				break;
			}
			default:
				GOTHOGRE_EXCEPT(mName << " - Unknown data type " << _outType);
		}
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplBinSafe::parseChunkBegin()
	{
		size_t delimPos1 = mCurrentLine.find(' ');
		if(delimPos1 == String::npos)
			return false;

		size_t delimPos2 = mCurrentLine.find(' ', delimPos1 + 1);
		if(delimPos2 == String::npos)
			return false;

		size_t delimPos3 = mCurrentLine.find(' ', delimPos2 + 1);
		if(delimPos3 == String::npos)
			return false;

		mReadChunk.chunkName = mCurrentLine.substr(1, delimPos1 - 1);
		mReadChunk.chunkType = mCurrentLine.substr(delimPos1 + 1, delimPos2 - delimPos1 - 1);

		mInStream.str( mCurrentLine.substr(delimPos2 + 1, delimPos3 - delimPos2 - 1) );
		mInStream.clear();
		mInStream >> mReadChunk.objectVersion;

		mInStream.str( mCurrentLine.substr(delimPos3 + 1, mCurrentLine.length() - delimPos3 - 2) );
		mInStream.clear();
		mInStream >> mReadChunk.objectIndex;
		return true;
	}

} // namespace GothOgre
