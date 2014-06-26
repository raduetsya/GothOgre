#ifndef GOTHOGRE_ZEN_ARCHIVE_IMPL_H
#define GOTHOGRE_ZEN_ARCHIVE_IMPL_H

#include "GothOgre_ZenArchive.h"
#include "GothOgre_DataStreamEx.h"

namespace GothOgre
{
	// јбстрактный базовый класс архива
	class ZenArchiveImpl
	{
	protected:
		typedef ZenArchive::Vec3 Vec3;
		typedef ZenArchive::Color Color;
		typedef ZenArchive::ObjectReadList ObjectReadList;
		typedef ZenArchive::ObjectWriteMap ObjectWriteMap;

	public:
		ZenArchiveImpl() {}
		virtual ~ZenArchiveImpl() {}

		void init( const DataStreamExPtr& _dataStream, ZenArchiveFlags _flags, ObjectReadList* _objectReadList )
		{
			mDataStream = _dataStream;
			mName = mDataStream->getName();
			mFlags = _flags;
			mObjectReadList = _objectReadList;
		}

		void init( const DataStreamExPtr& _dataStream, ZenArchiveFlags _flags, ObjectWriteMap* _objectWriteMap )
		{
			mDataStream = _dataStream;
			mName = mDataStream->getName();
			mFlags = _flags;
			mObjectWriteMap = _objectWriteMap;
		}

		virtual void readHeader() {}
		virtual void writeHeader() {}
		virtual void readFooter() {}
		virtual void writeFooter() {}

	public:
		// «аписать в архив параметр - целое число (4 байта)
		virtual void writeInt(const String& _entryName, int _value) = 0;

		// «аписать в архив параметр - байт
		virtual void writeByte(const String& _entryName, uchar _value) = 0;
		
		// «аписать в архив параметр - слово (2 байта)
		virtual void writeWord(const String& _entryName, ushort _value) = 0;

		// «аписать в архив параметр - вещественное значение (4 байта)
		virtual void writeFloat(const String& _entryName, float _value) = 0;

		// «аписать в архив параметр - логическое значение
		virtual void writeBool(const String& _entryName, bool _value) = 0;

		// «аписать в архив параметр - строку
		virtual void writeString(const String& _entryName, const String& _value) = 0;

		// «аписать в архив параметр - вектор
		virtual void writeVec3(const String& _entryName, const Vec3& _value) = 0;

		// «аписать в архив параметр - цвет
		virtual void writeColor(const String& _entryName, const Color& _value) = 0;
		
		// «аписать в архив параметр - массив байт
		virtual void writeRaw(const String& _entryName, const void* _rawData, size_t _sizeInBytes) = 0;

		// «аписать в архив параметр - массив вещественных чисел
		virtual void writeRawFloat(const String& _entryName, const float* _rawData, size_t _sizeInBytes) = 0;

		// «аписать в архив параметр - перечисление;
		// второй аргумент указывает имена констант перечислени€,
		// через точку с зап€той.
		virtual void writeEnum(const String& _entryName, int _value) = 0;

		// Ќачать запись чанка в архив.
		virtual ZenArchive::Chunk* writeChunkBegin(const String& _chunkName, const String& _chunkType, ushort _objectVersion, ulong _objectIndex) = 0;
		
		// «авершить запись чанка в архив.
		virtual void writeChunkEnd(ZenArchive::Chunk* _chunk) = 0;

	public:
		// ѕрочитать из архива параметр - целое число (4 байта)
		virtual bool readInt(const String& _entryName, int& _outValue) = 0;

		// ѕрочитать из архива параметр - байт
		virtual bool readByte(const String& _entryName, uchar& _outValue) = 0;

		// ѕрочитать из архива параметр - слово (2 байта)
		virtual bool readWord(const String& _entryName, ushort& _outValue) = 0;

		// ѕрочитать из архива параметр - вещественное число (4 байта)
		virtual bool readFloat(const String& _entryName, float& _outValue) = 0;

		// ѕрочитать из архива параметр - логическое значение
		virtual bool readBool(const String& _entryName, bool& _outValue) = 0;

		// ѕрочитать из архива параметр - строку
		virtual bool readString(const String& _entryName, String& _outValue) = 0;

		// ѕрочитать из архива параметр - вектор
		virtual bool readVec3(const String& _entryName, Vec3& _outValue) = 0;

		// ѕрочитать из архива параметр - цвет
		virtual bool readColor(const String& _entryName, Color& _outValue) = 0;

		// ѕрочитать из архива параметр - перечисление
		virtual bool readEnum(const String& _entryName, int& _outValue) = 0;

		// ѕрочитать из архива параметр - массив байт
		virtual size_t readRaw(const String& _entryName, void* _rawData, size_t _sizeInBytes) = 0;

		// ѕрочитать из архива параметр - массив вещественных чисел
		virtual size_t readRawFloat(const String& _entryName, float* _rawData, size_t _sizeInBytes) = 0;

		// ѕрочитать из архива чанк с указанным именем.
		// ≈сли чанк с указанным именем не найден, возвращаетс€ nullptr.
		virtual ZenArchive::Chunk* readChunkBegin(const String& _chunkName) = 0;

		// ѕрочитать конец чанка из архива, т.е. пропустить все 
		// до конца текущего чанка включительно.
		virtual void readChunkEnd(ZenArchive::Chunk* _chunk) = 0;

	protected:
		String           mName;
		DataStreamExPtr	 mDataStream;
		ZenArchiveFlags  mFlags;
		ObjectReadList*  mObjectReadList;
		ObjectWriteMap*  mObjectWriteMap;
	};

} // namespace GothOgre

#endif // GOTHOGRE_ZEN_ARCHIVE_IMPL_H