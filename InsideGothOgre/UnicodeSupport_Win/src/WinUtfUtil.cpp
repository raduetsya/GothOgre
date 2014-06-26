#include "Precompiled.h"
#include "WinUtfUtil.h"
#include "Utf8.h"
#include "Utf16.h"
#include "Utf16Endian.h"
#include "Utf32Endian.h"

namespace GothOgre
{
	template<typename T> struct BufType;
	template<>	struct BufType<Utf8> { typedef Utf8Buffer Type;};
	template<>	struct BufType<Utf16> { typedef Utf16Buffer Type;};
	template<>	struct BufType<Utf16LE> { typedef ByteBuffer Type;};
	template<>	struct BufType<Utf16BE> { typedef ByteBuffer Type;};
	template<>	struct BufType<Utf32LE> { typedef ByteBuffer Type;};
	template<>	struct BufType<Utf32BE> { typedef ByteBuffer Type;};

	template<typename T> struct ValueType;
	template<>	struct ValueType<Utf8> { typedef uint8 Type;};
	template<>	struct ValueType<Utf16> { typedef uint16 Type;};
	template<>	struct ValueType<Utf16LE> { typedef uint8 Type;};
	template<>	struct ValueType<Utf16BE> { typedef uint8 Type;};
	template<>	struct ValueType<Utf32LE> { typedef uint8 Type;};
	template<>	struct ValueType<Utf32BE> { typedef uint8 Type;};

	template<typename T> struct MaxSize;
	template<> struct MaxSize<Utf8> {enum {value = 4};};
	template<> struct MaxSize<Utf16> {enum {value = 2};};
	template<> struct MaxSize<Utf16LE> {enum {value = 4};};
	template<> struct MaxSize<Utf16BE> {enum {value = 4};};
	template<> struct MaxSize<Utf32LE> {enum {value = 4};};
	template<> struct MaxSize<Utf32BE> {enum {value = 4};};

#define IMPLEMENT_UTF_UTIL(NAME, SRC, DEST) \
	void NAME(BufType<DEST>::Type& _destBuf, const ValueType<SRC>::Type* _srcBuf, size_t _size) \
	{ \
		size_t size2 = _size * MaxSize<DEST>::value; \
		_destBuf.resize(size2); \
		size_t ofs = 0, ofs2 = 0; \
		while(ofs != _size) \
		{ \
			uint32 c; \
			SRC::next(_srcBuf, ofs, _size, c); \
			if(c == -1) \
			{ \
				UnicodeSupport::_throwCouldNotConvert("UTF-32", SRC::getName(), _srcBuf, _size * sizeof(ValueType<SRC>::Type)); \
			} \
			DEST::append_unsafe(_destBuf.data(), ofs2, c); \
		} \
		_destBuf.resize(ofs2); \
	}

#define IMPLEMENT_UTF_UTIL_UNSAFE(NAME, SRC, DEST) \
	void NAME(BufType<DEST>::Type& _destBuf, const ValueType<SRC>::Type* _srcBuf, size_t _size) \
	{ \
		size_t size2 = _size * MaxSize<DEST>::value; \
		_destBuf.resize(size2); \
		size_t ofs = 0, ofs2 = 0; \
		while(ofs != _size) \
		{ \
			uint32 c; \
			SRC::next_unsafe(_srcBuf, ofs, c); \
			DEST::append_unsafe(_destBuf.data(), ofs2, c); \
		} \
		_destBuf.resize(ofs2); \
	}

	IMPLEMENT_UTF_UTIL(utf8_to_utf16le, Utf8, Utf16LE)
	IMPLEMENT_UTF_UTIL(utf8_to_utf16be, Utf8, Utf16BE)
	IMPLEMENT_UTF_UTIL(utf8_to_utf32le, Utf8, Utf32LE)
	IMPLEMENT_UTF_UTIL(utf8_to_utf32be, Utf8, Utf32BE)

	IMPLEMENT_UTF_UTIL(utf16_to_utf8, Utf16, Utf8)
	IMPLEMENT_UTF_UTIL(utf16_to_utf16le, Utf16, Utf16LE)
	IMPLEMENT_UTF_UTIL(utf16_to_utf16be, Utf16, Utf16BE)
	IMPLEMENT_UTF_UTIL(utf16_to_utf32le, Utf16, Utf32LE)
	IMPLEMENT_UTF_UTIL(utf16_to_utf32be, Utf16, Utf32BE)

	IMPLEMENT_UTF_UTIL(utf16le_to_utf8, Utf16LE, Utf8)
	IMPLEMENT_UTF_UTIL(utf16be_to_utf8, Utf16BE, Utf8)
	IMPLEMENT_UTF_UTIL(utf32le_to_utf8, Utf32LE, Utf8)
	IMPLEMENT_UTF_UTIL(utf32be_to_utf8, Utf32BE, Utf8)

	IMPLEMENT_UTF_UTIL(utf8_to_utf16, Utf8, Utf16)
	IMPLEMENT_UTF_UTIL(utf16le_to_utf16, Utf16LE, Utf16)
	IMPLEMENT_UTF_UTIL(utf16be_to_utf16, Utf16BE, Utf16)
	IMPLEMENT_UTF_UTIL(utf32le_to_utf16, Utf32LE, Utf16)
	IMPLEMENT_UTF_UTIL(utf32be_to_utf16, Utf32BE, Utf16)

	IMPLEMENT_UTF_UTIL_UNSAFE(utf8_to_utf16_unsafe, Utf8, Utf16)
	IMPLEMENT_UTF_UTIL_UNSAFE(utf16_to_utf8_unsafe, Utf16, Utf8)
}