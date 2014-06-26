#include "Precompiled.h"
#include "ICUUtfUtil.h"

namespace GothOgre
{
	enum 
	{
		MAX_UTF8_BYTES_PER_UTF16_WORD = 3,
		MAX_UTF16_WORDS_PER_UTF8_BYTE = 1
	};
	//----------------------------------------------------------------------------
	void utf8_to_utf16(Utf16Buffer& _buf16, const uint8_t* _buf8, int32_t _sz8)
	{
		int32_t maxsz16 = _sz8 * MAX_UTF16_WORDS_PER_UTF8_BYTE;
		_buf16.resize(maxsz16);
		int32_t ofs8 = 0, ofs16 = 0;
		while(ofs8 != _sz8)
		{
			UChar32 c;
			U8_NEXT(_buf8, ofs8, _sz8, c);
			U16_APPEND_UNSAFE( _buf16.data(), ofs16, c);
		}
		int32_t sz16 = ofs16;
		_buf16.resize(sz16);
	}
	//----------------------------------------------------------------------------
	void utf8_to_utf16_unsafe(Utf16Buffer& _buf16, const uint8_t* _buf8, int32_t _sz8)
	{
		int32_t maxsz16 = _sz8 * MAX_UTF16_WORDS_PER_UTF8_BYTE;
		_buf16.resize(maxsz16);
		int32_t ofs8 = 0, ofs16 = 0;
		while(ofs8 != _sz8)
		{
			UChar32 c;
			U8_NEXT_UNSAFE(_buf8, ofs8, c);
			U16_APPEND_UNSAFE( _buf16.data(), ofs16, c);
		}
		int32_t sz16 = ofs16;
		_buf16.resize(sz16);
	}
	//----------------------------------------------------------------------------
	void utf16_to_utf8(Utf8Buffer& _buf8, const uint16_t* _buf16, int32_t _sz16)
	{
		int32_t maxsz8 = _sz16 * MAX_UTF8_BYTES_PER_UTF16_WORD;
		_buf8.resize(maxsz8);
		int32_t ofs16 = 0, ofs8 = 0;
		while(ofs16 != _sz16)
		{
			UChar32 c;
			U16_NEXT(_buf16, ofs16, _sz16, c);
			U8_APPEND_UNSAFE( _buf8.data(), ofs8, c);
		}
		int32_t sz8 = ofs8;
		_buf8.resize(sz8);
	}
	//----------------------------------------------------------------------------
	void utf16_to_utf8_unsafe(Utf8Buffer& _buf8, const uint16_t* _buf16, int32_t _sz16)
	{
		int32_t maxsz8 = _sz16 * MAX_UTF8_BYTES_PER_UTF16_WORD;
		_buf8.resize(maxsz8);
		int32_t ofs16 = 0, ofs8 = 0;
		while(ofs16 != _sz16)
		{
			UChar32 c;
			U16_NEXT_UNSAFE(_buf16, ofs16, c);
			U8_APPEND_UNSAFE( _buf8.data(), ofs8, c);
		}
		int32_t sz8 = ofs8;
		_buf8.resize(sz8);
	}
}