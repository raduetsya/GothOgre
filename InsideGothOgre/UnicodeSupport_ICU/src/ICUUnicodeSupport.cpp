#include "Precompiled.h"
#include "ICUUnicodeSupport.h"
#include "ICUCodepageConverter.h"
#include "ICUUtfUtil.h"
#include "ICUError.h"

namespace GothOgre
{
	//----------------------------------------------------------------------------
	// Case utils
	//----------------------------------------------------------------------------
	void ICUUnicodeSupport::toUpperCase(String& _str)
	{
		_toUpperCase<sizeof(String::value_type)> (_str);
	}
	//----------------------------------------------------------------------------
	template<>
	void ICUUnicodeSupport::_toUpperCase<1>(StringHolder<1> _str)
	{
		if(!_str.empty())
		{
			uint8_t* buf = &_str[0];
			int32_t len = _str.length();
			int32_t ofs = 0, ofs2 = 0;
			while(ofs != len)
			{
				UChar32 c;
				U8_NEXT(buf, ofs, len, c);
				c = u_toupper(c);
				U8_APPEND_UNSAFE( buf, ofs2, c);
			}
		}
	}
	//----------------------------------------------------------------------------
	template<>
	void ICUUnicodeSupport::_toUpperCase<2>(StringHolder<2> _str)
	{
		if(!_str.empty())
		{
			uint16_t* buf = &_str[0];
			int32_t len = _str.length();
			int32_t ofs = 0, ofs2 = 0;
			while(ofs != len)
			{
				UChar32 c;
				U16_NEXT(buf, ofs, len, c);
				c = u_toupper(c);
				U16_APPEND_UNSAFE( buf, ofs2, c);
			}
		}
	}
	//----------------------------------------------------------------------------
	void ICUUnicodeSupport::toLowerCase(String& _str)
	{
		_toLowerCase<sizeof(String::value_type)> (_str);
	}
	//----------------------------------------------------------------------------
	template<>
	void ICUUnicodeSupport::_toLowerCase<1>(StringHolder<1> _str)
	{
		if(!_str.empty())
		{
			uint8_t* buf = &_str[0];
			int32_t len = _str.length();
			int32_t ofs = 0, ofs2 = 0;
			while(ofs != len)
			{
				UChar32 c;
				U8_NEXT(buf, ofs, len, c);
				c = u_tolower(c);
				U8_APPEND_UNSAFE( buf, ofs2, c);
			}
		}
	}
	//----------------------------------------------------------------------------
	template<>
	void ICUUnicodeSupport::_toLowerCase<2>(StringHolder<2> _str)
	{
		if(!_str.empty())
		{
			uint16_t* buf = &_str[0];
			int32_t len = _str.length();
			int32_t ofs = 0, ofs2 = 0;
			while(ofs != len)
			{
				UChar32 c;
				U16_NEXT(buf, ofs, len, c);
				c = u_tolower(c);
				U16_APPEND_UNSAFE( buf, ofs2, c);
			}
		}
	}
	//--------------------------------------------------------------------------
	int ICUUnicodeSupport::compareNoCase(const String& _first, const String& _second)
	{
		return _compareNoCase<sizeof(String::value_type)> (_first, _second);
	}
	//--------------------------------------------------------------------------
	static inline int checkStringEnd(int32_t ofs1, int32_t len1, int32_t ofs2, int32_t len2)
	{
		if(ofs1 == len1)
		{
			if(ofs2 == len2)
				return 0;
			else
				return -1;
		}
		if(ofs2 == len2)
			return 1;

		return 2;
	}
	//--------------------------------------------------------------------------
	template<>
	int ICUUnicodeSupport::_compareNoCase<1>(ConstStringHolder<1> _first, ConstStringHolder<1> _second)
	{
		int32_t len1 = _first.length();
		int32_t len2 = _second.length();
		int32_t ofs1 = 0;
		int32_t ofs2 = 0;

		int r = checkStringEnd(ofs1, len1, ofs2, len2);
		if(r != 2) return r;

		const uint8_t* buf1 = _first.c_str();
		const uint8_t* buf2 = _second.c_str();
		while(true)
		{
			UChar32 c1, c2;
			U8_NEXT(buf1, ofs1, len1, c1);
			U8_NEXT(buf2, ofs2, len2, c2);
			
			c1 = u_tolower(c1);
			c2 = u_tolower(c2);
			if(c1 != c2)
				return (c1 < c2) ? -1 : 1;

			r = checkStringEnd(ofs1, len1, ofs2, len2);
			if(r != 2) return r;
		}
	}
	//--------------------------------------------------------------------------
	template<>
	int ICUUnicodeSupport::_compareNoCase<2>(ConstStringHolder<2> _first, ConstStringHolder<2> _second)
	{
		int32_t len1 = _first.length();
		int32_t len2 = _second.length();
		int32_t ofs1 = 0;
		int32_t ofs2 = 0;

		int r = checkStringEnd(ofs1, len1, ofs2, len2);
		if(r != 2) return r;

		const uint16_t* buf1 = _first.c_str();
		const uint16_t* buf2 = _second.c_str();
		while(true)
		{
			UChar32 c1, c2;
			U16_NEXT(buf1, ofs1, len1, c1);
			U16_NEXT(buf2, ofs2, len2, c2);
			
			c1 = u_tolower(c1);
			c2 = u_tolower(c2);
			if(c1 != c2)
				return (c1 < c2) ? -1 : 1;

			r = checkStringEnd(ofs1, len1, ofs2, len2);
			if(r != 2) return r;
		}
	}





	//-----------------------------------------------------------------------------------
	// encodings and converters
	//-----------------------------------------------------------------------------------
	void ICUUnicodeSupport::buildListEncodings()
	{
		int32_t count = ucnv_countAvailable();
		for(int32_t i = 0; i != count; ++i)
		{
			const char* canonicalName = ucnv_getAvailableName(i);
			
			UErrorCode errorCode = U_ZERO_ERROR;
			int16_t countAliases = ucnv_countAliases(canonicalName, &errorCode);
			CHECK_ICU_ERROR_CODE(errorCode);

			if(countAliases)
			{
				vector<const char*>::type aliases;
				aliases.resize(countAliases);
				ucnv_getAliases(canonicalName, &aliases[0], &errorCode);
				CHECK_ICU_ERROR_CODE(errorCode);

				for(size_t j = 0; j != countAliases; ++j)
					_addEncoding<sizeof(String::value_type)> (aliases[j]);
			}
		}
	}
	//----------------------------------------------------------------------------
	template<>
	void ICUUnicodeSupport::_addEncoding<1>(const char* _encoding)
	{
		addEncoding(_encoding);
	}
	//----------------------------------------------------------------------------
	template<>
	void ICUUnicodeSupport::_addEncoding<2>(const char* _encoding)
	{
		Utf16Buffer buf16;
		utf8_to_utf16_unsafe(buf16, char_cast<const uint8_t*>(_encoding), strlen(_encoding));
		String str;
		StringHolder<2>(str).assign( buf16.data(), buf16.size() );
		addEncoding(str);
	}
	//----------------------------------------------------------------------------
	ICUUnicodeSupport::ICUUnicodeSupport()
	{
	}
	//----------------------------------------------------------------------------
	ICUUnicodeSupport::~ICUUnicodeSupport()
	{
	}
	//----------------------------------------------------------------------------
	CodepageConverter* ICUUnicodeSupport::openConverterImpl(const String& _encoding)
	{
		UConverter* cnv = _openConverter<sizeof(String::value_type)>(_encoding);
		if(!cnv)
			return nullptr;

		return new ICUCodepageConverter(cnv);
	}
	//----------------------------------------------------------------------------
	template<>
	UConverter* ICUUnicodeSupport::_openConverter<1>(ConstStringHolder<1> _encoding)
	{
		UErrorCode errorCode = U_ZERO_ERROR;
		UConverter* cnv = ucnv_open( char_cast<const char*>( _encoding.c_str() ), &errorCode);
		CHECK_ICU_ERROR_CODE(errorCode);
		return cnv;
	}
	//----------------------------------------------------------------------------
	template<>
	UConverter* ICUUnicodeSupport::_openConverter<2>(ConstStringHolder<2> _encoding)
	{
		UErrorCode errorCode = U_ZERO_ERROR;
		UConverter* cnv = ucnv_openU( char_cast<const UChar*>( _encoding.c_str() ), &errorCode);
		CHECK_ICU_ERROR_CODE(errorCode);
		return cnv;
	}
	//----------------------------------------------------------------------------
	const String& ICUUnicodeSupport::getName() const
	{
		static const String s = "ICUUnicodeSupport";
		return s;
	}



} // namespace GothOgre
