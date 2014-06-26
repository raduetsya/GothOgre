#include "GothOgre_Precompiled.h"
#include "GothOgre_DataStreamEx.h"
#include "GothOgre_ZenArchive_ImplAscii.h"

namespace GothOgre
{
	const String ZenArchiveImplAscii::TYPENAME_INT       = "int";
	const String ZenArchiveImplAscii::TYPENAME_BOOL      = "bool";
	const String ZenArchiveImplAscii::TYPENAME_STRING    = "string";
	const String ZenArchiveImplAscii::TYPENAME_FLOAT     = "float";
	const String ZenArchiveImplAscii::TYPENAME_RAW       = "raw";
	const String ZenArchiveImplAscii::TYPENAME_RAW_FLOAT = "rawFloat";
	const String ZenArchiveImplAscii::TYPENAME_COLOR     = "color";
	const String ZenArchiveImplAscii::TYPENAME_VEC3      = "vec3";
	const String ZenArchiveImplAscii::TYPENAME_ENUM      = "enum";
	//----------------------------------------------------------------------------------
	ZenArchiveImplAscii::ZenArchiveImplAscii()
	{
		mGhostRootChunk = new Chunk;
		mCurrentChunk = mGhostRootChunk;
	}
	//----------------------------------------------------------------------------------
	ZenArchiveImplAscii::~ZenArchiveImplAscii()
	{
		if(mGhostRootChunk)
			delete mGhostRootChunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::readHeader()
	{
		// read number of objects
		mDataStream->readLine(mCurrentLine, EOL::LF);
		
		size_t delimpos = mCurrentLine.find(' ');
		String s = mCurrentLine.substr(0, delimpos);
		if(s != "objects")
			GOTHOGRE_EXCEPT(mName << " - Keyword 'objects' expected!");

		mInStream.str( mCurrentLine.substr(delimpos) );
		mInStream.clear();
		size_t numObjects;
		mInStream >> numObjects;
		mObjectReadList->resize(numObjects);

		// skip "END"
		mDataStream->readLine(mCurrentLine, EOL::LF);
		if(mCurrentLine != "END")
			GOTHOGRE_EXCEPT(mName << " - Keyword 'END' expected!");
		
		// skip empty line
		mDataStream->readLine(mCurrentLine, EOL::LF);
		if(!mCurrentLine.empty())
			GOTHOGRE_EXCEPT(mName << " - Empty line after 'END' expected!");

		mStartPos            = mDataStream->tellg();
		mNumNotFoundEntries  = 0;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::readFooter()
	{
		if(mNumNotFoundEntries != 0)
		{
			GOTHOGRE_WARNING(mName << " - Some entries (" << mNumNotFoundEntries << ") were not found.");
		}
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeHeader()
	{
		mDataStream->writeString("objects ");
		mNumObjectsPos = mDataStream->tellp();
		mDataStream->writeLine("0        ", EOL::LF);

		mDataStream->writeLine("END", EOL::LF);
		mDataStream->writeLine(StringUtil::BLANK, EOL::LF);
		
		mStartPos = mDataStream->tellp();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeFooter()
	{
		size_t endPos = mDataStream->tellp();
		mDataStream->seekp(mNumObjectsPos);

		// записываем в заголовок число объектов
		mOutStream.str(StringUtil::BLANK);
		mOutStream << mObjectWriteMap->size();
		mDataStream->writeString(mOutStream.str());
	
		mDataStream->seekp(endPos);
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeInt(const String& _entryName, int _value)
	{
		writeEntryBegin(_entryName, TYPENAME_INT);
		mOutStream << _value;
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeByte(const String& _entryName, uchar _value)
	{
		writeEntryBegin(_entryName, TYPENAME_INT);
		mOutStream << _value;
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeWord(const String& _entryName, ushort _value)
	{
		writeEntryBegin(_entryName, TYPENAME_INT);
		mOutStream << _value;
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeBool(const String& _entryName, bool _value)
	{
		writeEntryBegin(_entryName, TYPENAME_BOOL);
		mOutStream << _value;
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeFloat(const String& _entryName, float _value)
	{
		writeEntryBegin(_entryName, TYPENAME_FLOAT);
		mOutStream.precision(20);
		mOutStream << _value;
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeString(const String& _entryName, const String& _value)
	{
		writeEntryBegin(_entryName, TYPENAME_STRING);
		mOutStream << _value;
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeVec3(const String& _entryName, const Vec3& _value)
	{
		writeEntryBegin(_entryName, TYPENAME_VEC3);
		mOutStream.precision(20);
		mOutStream << _value.getX();
		mOutStream << ' ';
		mOutStream << _value.getY();
		mOutStream << ' ';
		mOutStream << _value.getZ();
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeColor(const String& _entryName, const Color& _value)
	{
		writeEntryBegin(_entryName, TYPENAME_COLOR);
		mOutStream << _value.getRed();
		mOutStream << ' ';
		mOutStream << _value.getGreen();
		mOutStream << ' ';
		mOutStream << _value.getBlue();
		mOutStream << ' ';
		mOutStream << _value.getAlpha();
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeRaw(const String& _entryName, const void* _rawData, size_t _sizeInBytes)
	{
		writeEntryBegin(_entryName, TYPENAME_RAW);

		std::ios_base::fmtflags oldFlags = mOutStream.flags(std::ios_base::hex | std::ios_base::uppercase);
		size_t oldWidth = mOutStream.width(2);
		String::value_type oldFill = mOutStream.fill('0');
	
		for(size_t i = 0; i != _sizeInBytes; ++i)
		{
			mOutStream << ((uchar*)_rawData)[i];
		}

		mOutStream.flags(oldFlags);
		mOutStream.width(oldWidth);
		mOutStream.fill(oldFill);

		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeRawFloat(const String& _entryName, const float* _rawData, size_t _sizeInBytes)
	{
		writeEntryBegin(_entryName, TYPENAME_RAW_FLOAT);
		mOutStream.precision(20);
		size_t numFloats = _sizeInBytes / sizeof(float);
		for(size_t i = 0; i != numFloats; i++)
		{
			if(i != 0) 
				mOutStream << ' ';
			mOutStream << _rawData[i];
		}
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeEnum(const String& _entryName, int _value)
	{
		writeEntryBegin(_entryName, TYPENAME_ENUM);
		mOutStream << _value;
		writeEntryEnd();
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeEntryBegin(const String& _entryName, const String& _entryType)
	{
		mOutStream.str(StringUtil::BLANK);
		mOutStream << mCurrentIndent;
		mOutStream << _entryName;
		mOutStream << '=';
		mOutStream << _entryType;
		mOutStream << ':';
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeEntryEnd()
	{
		mDataStream->writeLine(mOutStream.str(), EOL::LF);
	}
	//----------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchiveImplAscii::writeChunkBegin(const String& _chunkName, const String& _chunkType, ushort _objectVersion, ulong _objectIndex)
	{
		Chunk* chunk = new Chunk;
		chunk->mChunkName     = _chunkName;
		chunk->mChunkType     = _chunkType;
		chunk->mObjectVersion = _objectVersion;
		chunk->mObjectIndex   = _objectIndex;
		chunk->mChunkPos      = mDataStream->tellp();

		mOutStream.str(StringUtil::BLANK);
		mOutStream << mCurrentIndent;
		mOutStream << '[';
		mOutStream << _chunkName;
		mOutStream << ' ';
		mOutStream << _chunkType;
		mOutStream << ' ';
		mOutStream << _objectVersion;
		mOutStream << ' ';
		mOutStream << _objectIndex;
		mOutStream << ']';
		mDataStream->writeLine(mOutStream.str(), EOL::LF);
		mCurrentIndent += "\t";

		chunk->mDataPos = mDataStream->tellp();
		return chunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::writeChunkEnd(ZenArchive::Chunk* _chunk)
	{
		GOTHOGRE_ASSERT(mCurrentIndent.size() > 0, mName << " - ZenArchive::writeChunkEnd was called before ZenArchive::writeChunkBegin!");
		mCurrentIndent.resize(mCurrentIndent.size() - 1);

		mOutStream.str(StringUtil::BLANK);
		mOutStream << mCurrentIndent;
		mOutStream << "[]";
		mDataStream->writeLine(mOutStream.str(), EOL::LF);
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readInt(const String& _entryName, int& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		mInStream >> _outValue;
		return !mInStream.fail();
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readByte(const String& _entryName, uchar& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		mInStream >> _outValue;
		return !mInStream.fail();
	}		
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readWord(const String& _entryName, ushort& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		mInStream >> _outValue;
		return !mInStream.fail();
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readFloat(const String& _entryName, float& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		mInStream >> _outValue;
		return !mInStream.fail();
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readBool(const String& _entryName, bool& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		mInStream >> _outValue;
		return !mInStream.fail();
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readString(const String& _entryName, String& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		_outValue = mInStream.str();
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readVec3(const String& _entryName, Vec3& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		float x, y, z;
		mInStream >> x;
		if(mInStream.fail())
			return false;
		
		mInStream >> y;
		if(mInStream.fail())
			return false;
		
		mInStream >> z;
		if(mInStream.fail())
			return false;

		_outValue.setX(x);
		_outValue.setY(y);
		_outValue.setZ(z);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readColor(const String& _entryName, Color& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		int r, g, b, a;
		mInStream >> r;
		if(mInStream.fail())
			return false;
		
		mInStream >> g;
		if(mInStream.fail())
			return false;
		
		mInStream >> b;
		if(mInStream.fail())
			return false;

		mInStream >> a;
		if(mInStream.fail())
			return false;

		_outValue.setRed(   (uchar)r );
		_outValue.setGreen( (uchar)g );
		_outValue.setBlue(  (uchar)b );
		_outValue.setAlpha( (uchar)a );

		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readEnum(const String& _entryName, int& _outValue)
	{
		if(!readEntry(_entryName))
			return false;

		mInStream >> _outValue;
		return !mInStream.fail();
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplAscii::readRaw(const String& _entryName, void* _rawData, size_t _sizeInBytes)
	{
		if(!readEntry(_entryName))
			return 0;

		uchar* ptr = (uchar*) _rawData;
		size_t numBytesRead = 0;
		
		std::ios_base::fmtflags oldFlags = mInStream2.flags(std::ios_base::hex);
		while(true)
		{
			mInStream2.clear();
			mInStream2.seekp(0);

			mInStream2.put(mInStream.get());
			if(!mInStream.gcount())
				break;

			mInStream2.put(mInStream.get());
			if(!mInStream.gcount())
				break;
			
			uint u;
			mInStream2.seekg(0);
			mInStream2 >> u;
			if(mInStream2.fail())
				break;

			GOTHOGRE_ASSERT(numBytesRead < _sizeInBytes, mName << " - Not enough size to store size of 'raw' data " << _entryName);
			*ptr++ = (uchar) u;
			++numBytesRead;
		}

		mInStream2.flags(oldFlags);
		return numBytesRead;
	}
	//----------------------------------------------------------------------------------
	size_t ZenArchiveImplAscii::readRawFloat(const String& _entryName, float* _rawData, size_t _sizeInBytes)
	{
		if(!readEntry(_entryName))
			return 0;

		size_t numFloats = _sizeInBytes / sizeof(float);

		float* ptr = _rawData;
		size_t numFloatsRead = 0;
		while(true)
		{
			float f;
			mInStream >> f;
			if(mInStream.fail())
				break;

			GOTHOGRE_ASSERT(numFloatsRead < numFloats, mName << " - Not enough size to store size of 'rawFloat' data " << _entryName);
			*ptr++ = f;
			++numFloatsRead;
		}
		return numFloatsRead * sizeof(float);
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::readEntry(const String& _entryName)
	{
		bool found = false;

		// вначале ищем среди пропущенных параметров
		if(mCurrentChunk)
		{
			map<String, SkippedEntry>::type& ses = mCurrentChunk->mSkippedEntries;
			map<String, SkippedEntry>::type::iterator it = ses.find(_entryName);
			if(it != ses.end())
			{
				SkippedEntry& se = it->second;
				mReadEntry.value = se.entryValue;
				ses.erase(it);
				found = true;
			}
		}

		// поиск параметра с указанным именем
		while(!mDataStream->eof() && !found)
		{
			// читаем очередную строку из архива, если это возможно
			size_t linePos = mDataStream->tellg();
			mDataStream->readLine(mCurrentLine, EOL::LF);

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
						sc.chunkPos = linePos;
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
			else
			{
				// нашли какой-то параметр, проверяем совпадение имен
				if(parseEntry())
				{
					if(mReadEntry.name == _entryName)
					{
						found = true;
					}
					else
					{
						SkippedEntry& se = mCurrentChunk->mSkippedEntries.insert(std::make_pair(mReadEntry.name, SkippedEntry())).first->second;
						se.entryValue = mReadEntry.value;
					}
				}
			}
		}

		if(found)
		{
			// параметр найден
			mInStream.str(mReadEntry.value, StrStream::DONT_COPY);
			mInStream.clear();
		}
		else
		{
			// параметр не найден
			++mNumNotFoundEntries;
		}
		return found;
	}
	//----------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchiveImplAscii::readChunkBegin(const String& _chunkName)
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

		// поиск чанка с указанным именем в родительском чанке
		while(!mDataStream->eof() && !chunk)
		{
			// читаем очередную строчку из файла
			size_t linePos = mDataStream->tellg();
			mDataStream->readLine(mCurrentLine, EOL::LF);

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
							chunk->mChunkPos = linePos;
							chunk->mDataPos = mDataStream->tellg();
							chunk->mObjectIndex = mReadChunk.objectIndex;
							chunk->mObjectVersion = mReadChunk.objectVersion;
							chunk->mStoredPos = -1;
						}
						else if(mCurrentChunk)
						{
							SkippedSubchunk& sc = mCurrentChunk->mSkippedSubchunks.insert(std::make_pair(mReadChunk.chunkName, SkippedSubchunk())).first->second;
							sc.chunkName = mReadChunk.chunkName;
							sc.chunkType = mReadChunk.chunkType;
							sc.chunkPos = linePos;
							sc.dataPos = mDataStream->tellg();
							sc.objectIndex = mReadChunk.objectIndex;
							sc.objectVersion = mReadChunk.objectVersion;
						}
					}
				}
				else
				{
					// если дошли до конца родительского чанка
					mDataStream->seekg(linePos);
					break;
				}
			}
			else
			{
				// нашли какой-то параметр
				if(parseEntry())
				{
					SkippedEntry& se = mCurrentChunk->mSkippedEntries.insert(std::make_pair(mReadEntry.name, SkippedEntry())).first->second;
					se.entryValue = mReadEntry.value;
				}				
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
	void ZenArchiveImplAscii::readChunkEnd(ZenArchive::Chunk* _chunk)
	{
		Chunk* chunk = static_cast<Chunk*>(_chunk);
		// 
		if(chunk->mStoredPos != -1)
			mDataStream->seekg(chunk->mStoredPos);
		else
			findChunkEnd();

		mCurrentChunk = static_cast<Chunk*>(chunk->getParent());
		if(!mCurrentChunk)
			mCurrentChunk = mGhostRootChunk;
	}
	//----------------------------------------------------------------------------------
	void ZenArchiveImplAscii::findChunkEnd()
	{
		int bracketLevel = 1;
		while(!mDataStream->eof() && bracketLevel != 0)
		{
			mDataStream->readLine(mCurrentLine, EOL::LF);
			if(mCurrentLine.length() >= 2 && mCurrentLine[0] == '[' && mCurrentLine[mCurrentLine.length() - 1] == ']')
			{
				if(mCurrentLine.length() > 2) // [...] // beginning of chunk
					bracketLevel++;
				else
					bracketLevel--; // [] end of chunk
			}
		}
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::parseEntry()
	{
		size_t equalPos = mCurrentLine.find_first_of(" =\t");
		size_t colonPos = mCurrentLine.find(':', equalPos);
		if(equalPos == String::npos || colonPos == String::npos)
			return false;

		mReadEntry.name = mCurrentLine.substr(0, equalPos);
		mReadEntry.value = mCurrentLine.substr(colonPos + 1);
		return true;
	}
	//----------------------------------------------------------------------------------
	bool ZenArchiveImplAscii::parseChunkBegin()
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

