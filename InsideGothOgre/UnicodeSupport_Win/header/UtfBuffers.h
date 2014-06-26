#ifndef UTF_BUFFERS_H
#define UTF_BUFFERS_H

namespace GothOgre
{
	// Temporary UTF buffers' definitions
	typedef CodepageConverter::ByteBuffer ByteBuffer;
	typedef ByteBuffer Utf8Buffer;
	typedef Buffer<uint16, ByteBuffer::PREALLOC_SIZE> Utf16Buffer;

	enum 
	{
		MAX_UTF8_BYTES = 4,
		MAX_UTF16_WORDS = 2,
	};
}

#endif // UTF_BUFFERS_H