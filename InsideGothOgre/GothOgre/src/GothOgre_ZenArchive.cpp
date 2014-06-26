#include "GothOgre_Precompiled.h"
#include "GothOgre_StrStream.h"
#include "GothOgre_EncodingSettings.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_ZenArchive.h"
#include "GothOgre_ZenArchive_Impl.h"
#include "GothOgre_ZenArchive_ImplAscii.h"
#include "GothOgre_ZenArchive_ImplBinary.h"
#include "GothOgre_ZenArchive_ImplBinSafe.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------------------
	// ZenArchiveType
	//---------------------------------------------------------------------------------------
	GOTHOGRE_IMPLEMENT_ENUM_IO(ZenArchiveType)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(ASCII)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(BINARY)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(BIN_SAFE)
	GOTHOGRE_IMPLEMENT_ENUM_IO_END
	//---------------------------------------------------------------------------------------
	const String            ZenArchive::CHUNKNAME_EMPTY = "%";
	const String            ZenArchive::CHUNKTYPE_EMPTY = "%";
	const String            ZenArchive::CHUNKTYPE_OBJREF = "\xC2\xA7"; // § - in UTF-8
	const ZenArchive::Color ZenArchive::Color::BLACK = ZenArchive::Color(0, 0, 0, 0);
	const ZenArchive::Vec3  ZenArchive::Vec3::ZERO = ZenArchive::Vec3(0, 0, 0);
	//---------------------------------------------------------------------------------------
	ZenArchive::ZenArchive()
	{
		mCurrentChunk = nullptr;
		mImpl = nullptr;
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::~ZenArchive()
	{
		processFooter();
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::readHeader(const String& _filename, const String& _resourceGroup, ZenArchiveFlags _flags)
	{
		DataStreamPtr dataStream = ResourceGroupManager::getSingleton().openResource(_filename, _resourceGroup);
		readHeader(dataStream, _flags);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::readHeader(const DataStreamExPtr& _dataStream, ZenArchiveFlags _flags)
	{
		mDataStream = _dataStream;
		mWriting = false;
		mName = _dataStream->getName();
		makeBaseName();
		mFlags = _flags;
		readHeader();
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::readHeader()
	{
		GOTHOGRE_ASSERT(mImpl == nullptr, mName << " - Header of the zen archive is already read. Cannot read it again!");

		// первая строка архива должна быть "ZenGin Archive"
		EncodingSettings::getSingleton().readLineAndGuessEncoding(mDataStream, 
			"ZenGin Archive", EOL::LF);

		// получаем номер версии
		String line;
		mDataStream->readLine(line, EOL::LF);
		size_t delimPos = line.find(' ');
		String name = line.substr(0, delimPos);
		String value = (delimPos != String::npos) ? line.substr(delimPos) : StringUtil::BLANK;
		if(name != "ver")
			GOTHOGRE_EXCEPT(mName << " - Keyword 'ver' expected at the second line of the file.");
		StrStream ss(value, StrStream::DONT_COPY);
		int version;
		ss >> version;

		// имя архивного класса просто пропускаем
		if(version >= 1)
		{
			mDataStream->readLine(line, EOL::LF);
		}

		// тип архива
		mDataStream->readLine(line, EOL::LF);
		String typeAsStr = line;
		ss.str(typeAsStr, StrStream::DONT_COPY);
		ss.clear();
		ss >> mType;
		if(ss.fail())
			GOTHOGRE_EXCEPT(mName << " - Type " << typeAsStr << " is not supported by zen archiv.");

		// читаем остаток заголовка
		while(true)
		{
			size_t offset = mDataStream->tellg();
			mDataStream->readLine(line, EOL::LF);
			delimPos = line.find(' ');
			name = line.substr(0, delimPos);
			value = (delimPos != String::npos) ? line.substr(delimPos) : StringUtil::BLANK;
			if(name == "END")
			{
				break;
			}
			else if(name == "objects")
			{
				mDataStream->seekg(offset);
				break;
			}
			else if(name == "saveGame")
			{
				if(value == "1")
					mFlags.set( ZenArchiveFlags::SAVE_GAME );
			}
			else if(name == "encoding")
			{
				mDataStream->setEncoding(value);
			}
		}
		
		switch(mType)
		{
			case ZenArchiveType::ASCII: mImpl = new ZenArchiveImplAscii; break;
			case ZenArchiveType::BINARY: mImpl = new ZenArchiveImplBinary; break;
			case ZenArchiveType::BIN_SAFE: mImpl = new ZenArchiveImplBinSafe; break;
			default: GOTHOGRE_EXCEPT( "Unexpected type of zen archive: " << (int) mType);
		}

		mImpl->init(mDataStream, mFlags, &mObjectReadList);

		if(!mName.empty() && !mFlags.check( ZenArchiveFlags::NO_LOG ))
			GOTHOGRE_INFO(mName << " - Reading zen archive (type = " << typeAsStr << ")");

		mImpl->readHeader();
	}		
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeHeader(const String& _filename, const String& _resourceGroup, ZenArchiveType _type, ZenArchiveFlags _flags)
	{
		DataStreamPtr dataStream = ResourceGroupManager::getSingleton().createResource(_filename, _resourceGroup, true);
		writeHeader(dataStream, _type, _flags);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeHeader(const DataStreamExPtr& _dataStream, ZenArchiveType _type, ZenArchiveFlags _flags)
	{
		mDataStream = _dataStream;
		mWriting = true;
		mName = _dataStream->getName();
		makeBaseName();
		mType = _type;
		mFlags = _flags;
		writeHeader();
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeHeader()
	{
		GOTHOGRE_ASSERT(mImpl == nullptr, mName << " - Header of the zen archive is already written. Cannot write it again!");

		EncodingSettings::getSingleton().setupStreamEncoding(mDataStream);

		mDataStream->writeLine("ZenGin Archive", EOL::LF);
		mDataStream->writeLine("ver 1", EOL::LF);

		StrStream ss;
		ss << mType;
		String typeAsStr = ss.str();

		String piranhaBytesClassName;
		switch(mType)
		{
			case ZenArchiveType::ASCII:
				mImpl = new ZenArchiveImplAscii;
				piranhaBytesClassName = "zCArchiverGeneric"; 
				break;

			case ZenArchiveType::BINARY:
				mImpl = new ZenArchiveImplBinary;
				piranhaBytesClassName = "zCArchiverGeneric"; 
				break;
		
			case ZenArchiveType::BIN_SAFE:
				mImpl = new ZenArchiveImplBinSafe;
				piranhaBytesClassName = "zCArchiverBinSafe"; 
				break;

			default:
				GOTHOGRE_EXCEPT( "Unexpected type of zen archive: " << (int) mType);
		}

		mImpl->init(mDataStream, mFlags, &mObjectWriteMap);
		mDataStream->writeLine(piranhaBytesClassName, EOL::LF);
		mDataStream->writeLine(typeAsStr, EOL::LF);

		String saveGameLine = "saveGame " + String(mFlags.check( ZenArchiveFlags::SAVE_GAME ) ? "1" : "0");
		mDataStream->writeLine(saveGameLine, EOL::LF);

		if(!mFlags.check( ZenArchiveFlags::NO_DATE ))
		{
			time_t timer;
			time(&timer);
			tm timeinfo;
			localtime_s(&timeinfo, &timer);
			StrStream ss;
			ss << "date " << 
				timeinfo.tm_mday << "." << timeinfo.tm_mon + 1 << "." << timeinfo.tm_year + 1900 << " " << 
				timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec;
			mDataStream->writeLine(ss.str(), EOL::LF);
		}

		if(!mFlags.check( ZenArchiveFlags::NO_USER_INFO ))
		{
			String userName = getUserName();
			if(!userName.empty())
			{
				String userNameLine = "user " + userName;
				mDataStream->writeLine(userNameLine, EOL::LF);
			}
		}

		if(!mFlags.check( ZenArchiveFlags::NO_CODE_PAGE ))
		{
			String encodingLine = "codePage " + mDataStream->getEncoding();
			mDataStream->writeLine(encodingLine, EOL::LF);
		}
		
		mDataStream->writeLine("END", EOL::LF);

		// сообщение о начале записи архива
		if(!mName.empty() && !mFlags.check( ZenArchiveFlags::NO_LOG ))
			GOTHOGRE_INFO(mName << " - Writing zen archive (type = " << typeAsStr << ")");

		mImpl->writeHeader();
	}
	//---------------------------------------------------------------------------------------
	String ZenArchive::getUserName()
	{
		String userName;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		const int bufsize = 200;
		char buf[bufsize] = "";
		::GetUserName(buf, (LPDWORD) &bufsize);
		userName = buf;
#endif
		return userName;
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::makeBaseName()
	{
		mBaseName = mName;
		size_t i = mBaseName.rfind('\\');
		if(i != String::npos)
			mBaseName = mBaseName.substr(i + 1);
		i = mBaseName.rfind('/');
		if(i != String::npos)
			mBaseName = mBaseName.substr(i + 1);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::readFooter()
	{
		processFooter();
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeFooter()
	{
		processFooter();
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::processFooter()
	{
		if(mImpl)
		{
			endAllChunks();

			if(isWriting())
			{
				mImpl->writeFooter();
				if(!mName.empty() && !mFlags.check( ZenArchiveFlags::NO_LOG ))
				{
					GOTHOGRE_INFO(mName << " - Zen archive has been written.");
				}
			}
			else
			{
				mImpl->readFooter();
				if(!mName.empty() && !mFlags.check( ZenArchiveFlags::NO_LOG ))
				{
					GOTHOGRE_INFO(mName << " - Zen archive has been read.");
				}
			}

			delete mImpl;
			mImpl = nullptr;
			mObjectReadList.clear();
			mObjectWriteMap.clear();
			mDataStream.setNull();
			mName.clear();
			mFlags = 0;
		}
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::endAllChunks()
	{
		while(mCurrentChunk)
			endCurrentChunk();
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::endCurrentChunk()
	{
		if(mCurrentChunk)
		{
			if(isWriting())
				mImpl->writeChunkEnd(mCurrentChunk);
			else
				mImpl->readChunkEnd(mCurrentChunk);

			Chunk* parent = mCurrentChunk->mParent;
			delete mCurrentChunk;
			mCurrentChunk = parent;
		}
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchive::getCurrentChunk() const
	{
		return mCurrentChunk;
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeInt(const String& _entryName, int _value)
	{
		mImpl->writeInt(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeByte(const String& _entryName, uchar _value)
	{
		mImpl->writeByte(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeWord(const String& _entryName, ushort _value)
	{
		mImpl->writeWord(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeFloat(const String& _entryName, float _value)
	{
		mImpl->writeFloat(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeReal(const String& _entryName, Real _value)
	{
		mImpl->writeFloat(_entryName, (float) _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeBool(const String& _entryName, bool _value)
	{
		mImpl->writeBool(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeString(const String& _entryName, const String& _value)
	{
		mImpl->writeString(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeVec3(const String& _entryName, const Vec3& _value)
	{
		mImpl->writeVec3(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeVector3(const String& _entryName, const Vector3& _value)
	{
		mImpl->writeVec3(_entryName, (Vec3) _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeColor(const String& _entryName, const Color& _value)
	{
		mImpl->writeColor(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeColourValue(const String& _entryName, const ColourValue& _value)
	{
		mImpl->writeColor(_entryName, (Color) _value);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeRaw(const String& _entryName, const void* _rawData, size_t _sizeInBytes)
	{
		mImpl->writeRaw(_entryName, _rawData, _sizeInBytes);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeRawFloat(const String& _entryName, const float* _rawData, size_t _sizeInBytes)
	{
		GOTHOGRE_ASSERT(_sizeInBytes % sizeof(float) == 0, mName << " - Illegal size of rawFloat data: " << _sizeInBytes);
		mImpl->writeRawFloat(_entryName, _rawData, _sizeInBytes);
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeRawReal(const String& _entryName, const Real* _rawData, size_t _sizeInBytes)
	{
#if OGRE_DOUBLE_PRECISION == 1
		GOTHOGRE_ASSERT(_sizeInBytes % sizeof(Real) == 0, mName << " - Illegal size of rawFloat data: " << _sizeInBytes);
		size_t numReals = _sizeInBytes / sizeof(Real);
		mFloatVector.resize(numReals);
		for(size_t i = 0; i != numReals; ++i)
			mFloatVector[i] = (float) _rawData[i];
		mImpl->writeRawFloat(_entryName, &mFloatVector[0], numReals * sizeof(float));
#else
		mImpl->writeRawFloat(_entryName, _rawData, _sizeInBytes);
#endif
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeMatrix3(const String& _entryName, const Matrix3& _matrix)
	{
		Real data[9] = {_matrix[0][0], _matrix[0][1], _matrix[0][2],
						_matrix[1][0], _matrix[1][1], _matrix[1][2],
						_matrix[2][0], _matrix[2][1], _matrix[2][2]};
		writeRaw(_entryName, data, sizeof(data));
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeEnum(const String& _entryName, int _value)
	{
		mImpl->writeEnum(_entryName, _value);
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchive::writeChunkBegin(const String& _chunkName, const String& _chunkType, ushort _objectVersion, ulong _objectIndex)
	{
		ZenArchive::Chunk* chunk = mImpl->writeChunkBegin(
			_chunkName.empty() ? CHUNKNAME_EMPTY : _chunkName,
			_chunkType.empty() ? CHUNKTYPE_EMPTY : _chunkType,
			_objectVersion,
			_objectIndex);

		chunk->mParent = mCurrentChunk;
		mCurrentChunk = chunk;
		return chunk;
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeChunkEnd(Chunk* _chunk)
	{
		bool found = false;
		while(!found)
		{
			found = (mCurrentChunk == _chunk);
			endCurrentChunk();
		}
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeObject(ZenObjectPtr _zenObject)
	{
		writeObject( StringUtil::BLANK, _zenObject );
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::writeObject(const String& _chunkName, ZenObjectPtr _zenObject)
	{
		// случай, когда вместо объекта передан NULL 
		// или объект не подлежит записи в архив
		// - пишем пустой чанк
		if(_zenObject.isNull())
		{
			Chunk* chunk = writeChunkBegin(_chunkName);
			writeChunkEnd(chunk);
			return;
		}

		// проверка, вдруг этот объект уже был сохранен в архив ранее
		// - если так, сохраняем не объект, а только индекс первого сохранения
		ObjectWriteMap::iterator it = mObjectWriteMap.find(_zenObject);
		if(it != mObjectWriteMap.end())
		{
			ulong objectIndex = it->second;
			Chunk* chunk = writeChunkBegin(_chunkName, CHUNKTYPE_OBJREF, 0, objectIndex);
			writeChunkEnd(chunk);
			return;
		}
		
		// если объект не был сохранен ранее, сохраняем сейчас
		const String& compatibleTypeName = _zenObject->getCompatibleTypeName(this);
		ushort compatibleVersion = _zenObject->getCompatibleVersion(this);

		// добавляем объект в список уже сохраненных объектов
		ulong objectIndex = mObjectWriteMap.size();
		mObjectWriteMap[_zenObject] = objectIndex;

		// оформляем вывод объекта в виде чанка
		Chunk* chunk = writeChunkBegin(_chunkName, compatibleTypeName, compatibleVersion, objectIndex);
		_zenObject->write(this, compatibleVersion);
		writeChunkEnd(chunk);		
	}
	//---------------------------------------------------------------------------------------
	int ZenArchive::readInt(const String& _entryName, int _defaultValue)
	{
		int ret;
		if(mImpl->readInt(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	uchar ZenArchive::readByte(const String& _entryName, uchar _defaultValue)
	{
		uchar ret;
		if(mImpl->readByte(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	ushort ZenArchive::readWord(const String& _entryName, ushort _defaultValue)
	{
		ushort ret;
		if(mImpl->readWord(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	float ZenArchive::readFloat(const String& _entryName, float _defaultValue)
	{
		float ret;
		if(mImpl->readFloat(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	Real ZenArchive::readReal(const String& _entryName, Real _defaultValue)
	{
		float f;
		if(mImpl->readFloat(_entryName, f))
			return (Real) f;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	bool ZenArchive::readBool(const String& _entryName, bool _defaultValue)
	{
		bool ret;
		if(mImpl->readBool(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	String ZenArchive::readString(const String& _entryName, const String& _defaultValue)
	{
		String ret;
		if(mImpl->readString(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::Vec3 ZenArchive::readVec3(const String& _entryName, const Vec3& _defaultValue)
	{
		Vec3 ret;
		if(mImpl->readVec3(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	Vector3 ZenArchive::readVector3(const String& _entryName, const Vector3& _defaultValue)
	{
		Vec3 vec3;
		if(mImpl->readVec3(_entryName, vec3))
			return vec3.toVector3();
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::Color ZenArchive::readColor(const String& _entryName, const Color& _defaultValue)
	{
		Color color;
		if(mImpl->readColor(_entryName, color))
			return color;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	ColourValue ZenArchive::readColourValue(const String& _entryName, const ColourValue& _defaultValue)
	{
		Color color;
		if(mImpl->readColor(_entryName, color))
			return color.toColourValue();
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	int ZenArchive::readEnum(const String& _entryName, int _defaultValue)
	{
		int ret;
		if(mImpl->readEnum(_entryName, ret))
			return ret;
		else
			return _defaultValue;
	}
	//---------------------------------------------------------------------------------------
	size_t ZenArchive::readRaw(const String& _entryName, void* _rawData, size_t _sizeInBytes)
	{
		return mImpl->readRaw(_entryName, _rawData, _sizeInBytes);
	}
	//---------------------------------------------------------------------------------------
	size_t ZenArchive::readRawFloat(const String& _entryName, float* _rawData, size_t _sizeInBytes)
	{
		GOTHOGRE_ASSERT(_sizeInBytes % sizeof(float) == 0, mName << " - Illegal size of rawFloat data: " << _sizeInBytes);
		return mImpl->readRawFloat(_entryName, _rawData, _sizeInBytes);
	}
	//---------------------------------------------------------------------------------------
	size_t ZenArchive::readRawReal(const String& _entryName, Real* _rawData, size_t _sizeInBytes)
	{
#if OGRE_DOUBLE_PRECISION == 1
		GOTHOGRE_ASSERT(_sizeInBytes % sizeof(Real) == 0, mName << " - Illegal size of rawFloat data: " << _sizeInBytes);
		size_t numReals = _sizeInBytes / sizeof(Real));
		mFloatVector.resize(numReals);
		size_t sz = readRawFloat(_entryName, &mFloatVector[0], numReals * sizeof(float));
		size_t numRealsRead = sz / sizeof(float);
		for(size_t i = 0; i != numRealsRead; ++i)
			_rawData = (Real) mFloatVector[i];
		return numRealsRead * sizeof(Real);
#else
		return readRawFloat(_entryName, _rawData, _sizeInBytes);
#endif
	}
	//---------------------------------------------------------------------------------------
	Matrix3 ZenArchive::readMatrix3(const String& _entryName, const Matrix3& _defaultValue)
	{
		Real data[9];
		size_t sz = readRaw(_entryName, data, sizeof(data));
		GOTHOGRE_ASSERT(sz == 9 * sizeof(Real), "ZenArchive: Matrix is expected to have size = 9*4 bytes");
		return Matrix3(	data[0], data[1], data[2],
						data[3], data[4], data[5],
						data[6], data[7], data[8]);
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchive::readChunkBegin(const String& _chunkName)
	{
		return _readChunkBegin(_chunkName.empty() ? CHUNKNAME_EMPTY : _chunkName);
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchive::readChunkBegin()
	{
		return _readChunkBegin(StringUtil::BLANK);
	}
	//---------------------------------------------------------------------------------------
	ZenArchive::Chunk* ZenArchive::_readChunkBegin(const String& _chunkName)
	{
		ZenArchive::Chunk* chunk = mImpl->readChunkBegin(_chunkName);
		if(chunk)
		{
			chunk->mParent = mCurrentChunk;
			mCurrentChunk = chunk;
			return chunk;
		}
		else
		{
			delete chunk;
			return nullptr;
		}
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::readChunkEnd(Chunk* _chunk)
	{
		bool found = false;
		while(!found)
		{
			found = (mCurrentChunk == _chunk);
			endCurrentChunk();
		}
	}
	//---------------------------------------------------------------------------------------
	ZenObjectPtr ZenArchive::readObject(const String& _chunkName)
	{
		return _readObject(_chunkName.empty() ? CHUNKNAME_EMPTY : _chunkName);
	}
	//---------------------------------------------------------------------------------------
	ZenObjectPtr ZenArchive::readObject()
	{
		return _readObject(StringUtil::BLANK);
	}
	//---------------------------------------------------------------------------------------
	ZenObjectPtr ZenArchive::_readObject(const String& _chunkName)
	{
		// поиск и чтение заголовка чанка с указанным именем
		Chunk* chunk = _readChunkBegin(_chunkName);
		if(!chunk)
			return ZenObjectPtr();
		
		// имя класса не указано => этот чанк не представляет никакого объекта, возможно ошибка
		if(chunk->getChunkType().empty())
		{
			GOTHOGRE_EXCEPT(mName << " - Expected a chunk with non-empty type"
				<< " (which can be used as a class name)"
				<< " at position " << chunk->getChunkPos());
		}
		
		// символ с кодом 0xA7 вместо имени класса => это ссылка на объект, уже загруженный ранее из этого архива
		ulong objectIndex = chunk->getObjectIndex();
		if(chunk->getChunkType() == CHUNKTYPE_OBJREF)
		{
			readChunkEnd(chunk);
			if(objectIndex >= mObjectReadList.size())
			{
				GOTHOGRE_EXCEPT(mName << " - Index of object #" << objectIndex 
					<< " is out of range [0.." << mObjectReadList.size() - 1 << "].");
			}
			ZenObjectPtr referencedObject = mObjectReadList[objectIndex];
			return referencedObject;
		}

		// если объект не задан, то создаем его,
		// зная имя класса, прочитанное из архива
		ZenObjectPtr zenObject = ZenManager::getSingleton().createZenObject(chunk->getChunkType());
		chunk->mObject = zenObject;

		if(zenObject.isNull())
		{
			GOTHOGRE_EXCEPT(mName << " - Unknown type " << chunk->getChunkType()
				<< " of object #" << objectIndex << ".");
		}

		// проверяем соответствие версии объекта
		// версии, сохраненной в архиве
		if(mType == ZenArchiveType::BINARY)
		{
			ushort objectVersion = chunk->getObjectVersion();
			const vector<ushort>::type& supportedVersions = zenObject->getSupportedVersions();
			if(std::find(supportedVersions.begin(), supportedVersions.end(), 
				objectVersion) == supportedVersions.end())
			{
				GOTHOGRE_EXCEPT(mName << " - Unsupported version " << objectVersion
					<< " of object #" << objectIndex);
			}
		}

		// добавляем объект к списку
		if(!mObjectReadList[objectIndex].isNull())
		{
			GOTHOGRE_EXCEPT(mName << " - Object #" << objectIndex
					<< " is duplicated.");
		}
		mObjectReadList[objectIndex] = zenObject;
		
		// вызов функции read для получения объектом своих параметров
		zenObject->read(this, chunk->getObjectVersion());

		readChunkEnd(chunk);
		return zenObject;
	}
	//---------------------------------------------------------------------------------------
	void ZenArchive::skipObject()
	{
		readChunkEnd(readChunkBegin());
	}


} // namespace GothOgre
