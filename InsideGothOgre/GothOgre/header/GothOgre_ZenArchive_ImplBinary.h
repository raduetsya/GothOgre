#ifndef GOTHOGRE_ZEN_ARCHIVE_IMPL_BINARY_H
#define GOTHOGRE_ZEN_ARCHIVE_IMPL_BINARY_H

#include "GothOgre_ZenArchive_Impl.h"

namespace GothOgre
{
	// јбстрактный базовый класс архива
	class ZenArchiveImplBinary : public ZenArchiveImpl
	{
	public:
		ZenArchiveImplBinary();
		virtual ~ZenArchiveImplBinary();

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
		virtual ZenArchive::Chunk* writeChunkBegin(const String& _chunkName, const String& _chunkType, ushort _objectVersion, ulong objectIndex);
		
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
		class Chunk : public ZenArchive::Chunk
		{
			friend class ZenArchiveImplBinary;
			Chunk() {mChunkSize = 0;}

			size_t mChunkSize;
		};

	private:
		size_t	mPosNumObjects;
		String  mCurrentLine;
		String  mTempStr;
	};

} // namespace GothOgre

#endif // GOTHOGRE_ZEN_ARCHIVE_IMPL_BINARY_H