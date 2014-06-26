#include "Precompiled.h"
#include "ICUCodepageConverter.h"
#include "ICUUtfUtil.h"
#include "ICUError.h"

namespace GothOgre
{

	ICUCodepageConverter::ICUCodepageConverter(UConverter* _cnv)
	{
		mCnv = _cnv;
	}
	//--------------------------------------------------------------------------
	ICUCodepageConverter::~ICUCodepageConverter()
	{
		ucnv_close(mCnv);
	}
	//--------------------------------------------------------------------------
	void ICUCodepageConverter::unicodeToBuffer(ByteBuffer& _destBuffer, const String& _srcString)
	{
		_unicodeToBuffer<sizeof(String::value_type)> (_destBuffer, _srcString);
	}
	//--------------------------------------------------------------------------
	void ICUCodepageConverter::utf16_to_buffer(ByteBuffer& _destBuffer, const uint16_t* _source, size_t _size)
	{
		size_t oldSize = _destBuffer.size();
		size_t newSize = oldSize + UCNV_GET_MAX_BYTES_FOR_STRING(_size, ucnv_getMaxCharSize(mCnv));
		_destBuffer.resize(newSize);
		char* target0 = char_cast<char*>(&_destBuffer[0]);
		char* target = char_cast<char*>(&_destBuffer[oldSize]);
		char* targetLimit = target + newSize;
		const UChar* source = char_cast<const UChar*>(_source);
		const UChar* sourceLimit = source + _size;
		
		UErrorCode errorCode = U_ZERO_ERROR;
		ucnv_fromUnicode(mCnv, &target, targetLimit, &source, sourceLimit, 0, TRUE, &errorCode);
		if(U_FAILURE(errorCode))
			getUnicodeSupport()->_throwCouldNotConvert(
				getName(), "UTF-16", _source, _size * 2);

		newSize = target - target0;
		_destBuffer.resize(newSize);
	}
	//--------------------------------------------------------------------------
	template<>
	void ICUCodepageConverter::_unicodeToBuffer<1>(ByteBuffer& _destBuffer, ConstStringHolder<1> _srcString)
	{
		if(_srcString.empty())
			return;

		Utf16Buffer buf16;
		utf8_to_utf16(buf16, _srcString.c_str(), _srcString.size());
		utf16_to_buffer(_destBuffer, buf16.data(), buf16.size());
	}
	//--------------------------------------------------------------------------
	template<>
	void ICUCodepageConverter::_unicodeToBuffer<2>(ByteBuffer& _destBuffer, ConstStringHolder<2> _srcString)
	{
		if(_srcString.empty())
			return;

		utf16_to_buffer(_destBuffer, _srcString.c_str(), _srcString.size());
	}
	//--------------------------------------------------------------------------
	void ICUCodepageConverter::bufferToUnicode(String& _destString, const ByteBuffer& _srcBuffer)
	{
		_bufferToUnicode<sizeof(String::value_type)> (_destString, _srcBuffer);
	}
	//--------------------------------------------------------------------------
	void ICUCodepageConverter::buffer_to_utf16(Utf16Buffer& _destBuf16, const ByteBuffer& _srcBuffer)
	{
		const char* source = char_cast<const char*>( _srcBuffer.data() );
		const char* sourceLimit = source + _srcBuffer.size();
		_destBuf16.resize(_srcBuffer.size() * MAX_UTF16_WORDS);
		UChar* target0 = char_cast<UChar*>( _destBuf16.data() );
		UChar* target = target0;
		UChar* targetLimit = target + _destBuf16.size();
		
		UErrorCode errorCode = U_ZERO_ERROR;
		ucnv_toUnicode(mCnv, &target, targetLimit, &source, sourceLimit, 0, TRUE, &errorCode);
		if(U_FAILURE(errorCode))
			getUnicodeSupport()->_throwCouldNotConvert(
				"UTF-16", getName(), _srcBuffer.data(), _srcBuffer.size());
		
		int32_t sz16 = target - target0;
		_destBuf16.resize(sz16);
	}
	//--------------------------------------------------------------------------
	template<>
	void ICUCodepageConverter::_bufferToUnicode<1>(StringHolder<1> _destString, const ByteBuffer& _srcBuffer)
	{
		Utf16Buffer buf16;
		buffer_to_utf16(buf16, _srcBuffer);

		Utf8Buffer buf8;
		utf16_to_utf8_unsafe(buf8, buf16.data(), buf16.size());

		_destString.append( buf8.data(), buf8.size());
	}
	//--------------------------------------------------------------------------
	template<>
	void ICUCodepageConverter::_bufferToUnicode<2>(StringHolder<2> _destString, const ByteBuffer& _srcBuffer)
	{
		Utf16Buffer buf16;
		buffer_to_utf16(buf16, _srcBuffer);
		_destString.append( buf16.data(), buf16.size());
	}
	//--------------------------------------------------------------------------
	void ICUCodepageConverter::singleCharToUnicode(String& _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset)
	{
		_singleCharToUnicode<sizeof(String::value_type)> (_destString, _srcBuffer, _srcOffset);
	}
	//--------------------------------------------------------------------------
	UChar32 ICUCodepageConverter::getNextUChar(const ByteBuffer& _srcBuffer, size_t& _srcOffset)
	{
		const char* source0 = char_cast<const char*>(_srcBuffer.data());
		const char* source = source0 + _srcOffset;
		const char* sourceLimit = source0 + _srcBuffer.size();
		
		UErrorCode errorCode = U_ZERO_ERROR;
		UChar32 c = ucnv_getNextUChar(mCnv, &source, sourceLimit, &errorCode);
		if(errorCode == U_INDEX_OUTOFBOUNDS_ERROR)
			return -1;

		if(U_FAILURE(errorCode))
			getUnicodeSupport()->_throwCouldNotConvert(
				"UTF-32", getName(), _srcBuffer.data() + _srcOffset, _srcBuffer.size() - _srcOffset);
		
		_srcOffset = source - source0;
		return c;
	}
	//--------------------------------------------------------------------------
	template<>
	void ICUCodepageConverter::_singleCharToUnicode<1>(StringHolder<1> _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset)
	{
		UChar32 c = getNextUChar(_srcBuffer, _srcOffset);
		if(c == -1) return;

		uint8_t buf8[MAX_UTF8_BYTES];
		int32_t ofs8 = 0;
		U8_APPEND_UNSAFE( buf8, ofs8, c);
		int32_t sz8 = ofs8;

		_destString.append( buf8, sz8);
	}
	//--------------------------------------------------------------------------
	template<>
	void ICUCodepageConverter::_singleCharToUnicode<2>(StringHolder<2> _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset)
	{
		UChar32 c = getNextUChar(_srcBuffer, _srcOffset);
		if(c == -1) return;

		uint16_t buf16[MAX_UTF16_WORDS];
		int32_t ofs16 = 0;
		U16_APPEND_UNSAFE( buf16, ofs16, c);
		int32_t sz16 = ofs16;

		_destString.append( buf16, sz16);
	}
} 