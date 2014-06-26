#ifndef WIN_UTF_UTIL_H
#define WIN_UTF_UTIL_H

#include "UtfBuffers.h"

namespace GothOgre
{
	// UTF-8 -> ?
	void utf8_to_utf16le(ByteBuffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf8_to_utf16be(ByteBuffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf8_to_utf32le(ByteBuffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf8_to_utf32be(ByteBuffer& _destBuf, const uint8* _srcData, size_t _srcSize);

	// UTF-16 -> ?
	void utf16_to_utf8(ByteBuffer& _destBuf, const uint16* _srcData, size_t _srcSize);
	void utf16_to_utf16le(ByteBuffer& _destBuf, const uint16* _srcData, size_t _srcSize);
	void utf16_to_utf16be(ByteBuffer& _destBuf, const uint16* _srcData, size_t _srcSize);
	void utf16_to_utf32le(ByteBuffer& _destBuf, const uint16* _srcData, size_t _srcSize);
	void utf16_to_utf32be(ByteBuffer& _destBuf, const uint16* _srcData, size_t _srcSize);

	// ? -> UTF-8
	void utf16le_to_utf8(Utf8Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf16be_to_utf8(Utf8Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf32le_to_utf8(Utf8Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf32be_to_utf8(Utf8Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);

	// ? -> UTF-16
	void utf8_to_utf16(Utf16Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf16le_to_utf16(Utf16Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf16be_to_utf16(Utf16Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf32le_to_utf16(Utf16Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
	void utf32be_to_utf16(Utf16Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);

	void utf16_to_utf8_unsafe(ByteBuffer& _destBuf, const uint16* _srcData, size_t _srcSize);
	void utf8_to_utf16_unsafe(Utf16Buffer& _destBuf, const uint8* _srcData, size_t _srcSize);
}

#endif // WIN_UTF_UTIL_H