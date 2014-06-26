#ifndef GOTHOGRE_ZEN_ARCHIVE_IMPL_BIN_SAFE_H
#define GOTHOGRE_ZEN_ARCHIVE_IMPL_BIN_SAFE_H

#include "GothOgre_ZenArchive_Impl.h"

namespace GothOgre
{
	// јбстрактный базовый класс архива
	class ZenArchiveImplBinSafe : public ZenArchiveImpl
	{
	public:
		ZenArchiveImplBinSafe();
		virtual ~ZenArchiveImplBinSafe();

		virtual void readHeader();
		virtual void writeHeader();
		virtual void readFooter();
		virtual void writeFooter();

	public:
		// «аписать в архив параметр - целое число (4 байта)
		virtual void writeInt(const String& _entryName, int _value);

		// «аписать в архив параметр - байт
		virtual void writeByte(const String& _entryName, uchar _value);
		
		// «аписать в архив параметр - слово (2 байта)
		virtual void writeWord(const String& _entryName, ushort _value);

		// «аписать в архив параметр - вещественное значение (4 байта)
		virtual void writeFloat(const String& _entryName, float _value);

		// «аписать в архив параметр - логическое значение
		virtual void writeBool(const String& _entryName, bool _value);

		// «аписать в архив параметр - строку
		virtual void writeString(const String& _entryName, const String& _value);

		// «аписать в архив параметр - вектор
		virtual void writeVec3(const String& _entryName, const Vec3& _value);

		// «аписать в архив параметр - цвет
		virtual void writeColor(const String& _entryName, const Color& _value);
		
		// «аписать в архив параметр - массив байт
		virtual void writeRaw(const String& _entryName, const void* _rawData, size_t _sizeInBytes);

		// «аписать в архив параметр - массив вещественных чисел
		virtual void writeRawFloat(const String& _entryName, const float* _rawData, size_t _sizeInBytes);

		// «аписать в архив параметр - перечисление;
		// второй аргумент указывает имена констант перечислени€,
		// через точку с зап€той.
		virtual void writeEnum(const String& _entryName, int _value);

		// Ќачать запись чанка в архив.
		virtual ZenArchive::Chunk* writeChunkBegin(const String& _chunkName, const String& _chunkType, ushort _objectVersion, ulong _objectIndex);
		
		// «авершить запись чанка в архив.
		virtual void writeChunkEnd(ZenArchive::Chunk* _chunk);

	public:
		// ѕрочитать из архива параметр - целое число (4 байта)
		virtual bool readInt(const String& _entryName, int& _outValue);

		// ѕрочитать из архива параметр - байт
		virtual bool readByte(const String& _entryName, uchar& _outValue);

		// ѕрочитать из архива параметр - слово (2 байта)
		virtual bool readWord(const String& _entryName, ushort& _outValue);

		// ѕрочитать из архива параметр - вещественное число (4 байта)
		virtual bool readFloat(const String& _entryName, float& _outValue);

		// ѕрочитать из архива параметр - логическое значение
		virtual bool readBool(const String& _entryName, bool& _outValue);

		// ѕрочитать из архива параметр - строку
		virtual bool readString(const String& _entryName, String& _outValue);

		// ѕрочитать из архива параметр - вектор
		virtual bool readVec3(const String& _entryName, Vec3& _outValue);

		// ѕрочитать из архива параметр - цвет
		virtual bool readColor(const String& _entryName, Color& _outValue);

		// ѕрочитать из архива параметр - перечисление
		virtual bool readEnum(const String& _entryName, int& _outValue);

		// ѕрочитать из архива параметр - массив байт
		virtual size_t readRaw(const String& _entryName, void* _rawData, size_t _sizeInBytes);

		// ѕрочитать из архива параметр - массив вещественных чисел
		virtual size_t readRawFloat(const String& _entryName, float* _rawData, size_t _sizeInBytes);

		// ѕрочитать из архива чанк с указанным именем.
		// ≈сли чанк с указанным именем не найден, возвращаетс€ FALSE.
		virtual ZenArchive::Chunk* readChunkBegin(const String& _chunkName);

		// ѕрочитать конец чанка из архива, т.е. пропустить все 
		// до конца текущего чанка включительно.
		virtual void readChunkEnd(ZenArchive::Chunk* _chunk);

	private:
		enum BinSafeType
		{
			TYPE_0 = 0,
			TYPE_STRING,
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_BYTE,
			TYPE_WORD,
			TYPE_BOOL,
			TYPE_VEC3,
			TYPE_COLOR,
			TYPE_RAW,
			TYPE_10,
			TYPE_11,
			TYPE_12,
			TYPE_13,
			TYPE_14,
			TYPE_15,
			TYPE_RAW_FLOAT,
			TYPE_ENUM,
			TYPE_INSERTION_INDEX
		};

		struct SkippedEntry
		{
			BinSafeType        entryType;
			vector<char>::type entryData;
			String             entryDataIfStr;
		};

		struct SkippedSubchunk
		{
			String chunkName;
			String chunkType;
			size_t chunkPos;
			size_t dataPos;
			ushort objectVersion;
			ulong  objectIndex;
		};

		class Chunk : public ZenArchive::Chunk
		{
			Chunk() {mStoredPos = -1;}
			friend class ZenArchiveImplBinSafe;

			map<ushort, SkippedEntry>::type		mSkippedEntries;
			map<String, SkippedSubchunk>::type	mSkippedSubchunks;
			size_t								mStoredPos;
		};

		void writeEntry(const String& _entryName, BinSafeType _type, const void* _data, size_t _size);
		void writeValue(BinSafeType _type, const void* _data, size_t _size);
		void writeTypeAndSize(BinSafeType _type, size_t _size);
		size_t readEntry(const String& _entryName, BinSafeType _type, void* _data, size_t _size);
		void readTypeAndSize(BinSafeType& _outType, size_t& _outSize);
		void checkTypeAndSize(const String& _entryName, BinSafeType _type, size_t _size, BinSafeType _readType, size_t _readSize);
		bool parseChunkBegin();
		void findChunkEnd();

	private:
		class HashTable
		{
		public:
			HashTable();
			ushort find(const String& _key) const;
			ushort insert(const String& _key);
			void clear();
			void write(const DataStreamExPtr& _dataStream);
			void read(const DataStreamExPtr& _dataStream);
			static size_t hashFunc(const String& _key);

		private:
			struct Entry
			{
				String mKey;
				ushort mInsertionIndex;
			};
			enum {HASH_TABLE_SIZE = 97};
			list<Entry>::type mTable[HASH_TABLE_SIZE];
			ushort            mSize;
		};

		struct ReadChunk
		{
			String chunkName;
			String chunkType;
			ushort objectVersion;
			ulong  objectIndex;
		};

	private:
		HashTable       mHashTable;
		size_t			mNumObjectsPos;
		size_t          mHashTableOffsetPos;
		size_t			mStartPos;
		ulong           mBinSafeVersion;
		Chunk*			mCurrentChunk;
		Chunk*			mGhostRootChunk;
		ReadChunk		mReadChunk;
		String			mCurrentLine;
		StrStream	mInStream;
		StrStream	mOutStream;
		size_t			mNumNotFoundEntries;
	};

} // namespace GothOgre

#endif // GOTHOGRE_ZEN_ARCHIVE_IMPL_BIN_SAFE_H