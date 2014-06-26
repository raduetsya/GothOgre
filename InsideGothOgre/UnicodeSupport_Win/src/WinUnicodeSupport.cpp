#include "Precompiled.h"
#include "WinCodepageConverter.h"
#include "WinUnicodeSupport.h"
#include "UtfCodePageId.h"
#include "Utf8.h"
#include "Utf16.h"

namespace GothOgre
{
	//----------------------------------------------------------------------------
	// Case utils
	//----------------------------------------------------------------------------
	void WinUnicodeSupport::toUpperCase(String& _str)
	{
		_toUpperCase<sizeof(String::value_type)> (_str);
	}
	//----------------------------------------------------------------------------
	template<>
	void WinUnicodeSupport::_toUpperCase<1>(StringHolder<1> _str)
	{
		if(_str.empty())
			return;

		uint8* buf = &_str[0];
		size_t len = _str.length();
		size_t ofs = 0, ofs2 = 0;
		while(ofs != len)
		{
			uint32 c;
			Utf8::next(buf, ofs, len, c);
			if(c < 0xFFFF)
				c = (uint32) CharUpperW( (LPWSTR) c);
			Utf8::append(buf, ofs2, len, c);
		}
	}
	//----------------------------------------------------------------------------
	template<>
	void WinUnicodeSupport::_toUpperCase<2>(StringHolder<2> _str)
	{
		if(_str.empty())
			return;

		CharUpperBuffW( (LPWSTR) &_str[0], _str.length());
	}
	//----------------------------------------------------------------------------
	void WinUnicodeSupport::toLowerCase(String& _str)
	{
		_toLowerCase<sizeof(String::value_type)> (_str);
	}
	//--------------------------------------------------------------------------
	template<>
	void WinUnicodeSupport::_toLowerCase<1>(StringHolder<1> _str)
	{
		if(_str.empty())
			return;

		uint8* buf = &_str[0];
		size_t len = _str.length();
		size_t ofs = 0, ofs2 = 0;
		while(ofs != len)
		{
			uint32 c;
			Utf8::next(buf, ofs, len, c);
			if(c < 0xFFFF)
				c = (uint32) CharLowerW( (LPWSTR) c);
			Utf8::append(buf, ofs2, len, c);
		}
	}
	//--------------------------------------------------------------------------
	template<>
	void WinUnicodeSupport::_toLowerCase<2>(StringHolder<2> _str)
	{
		if(_str.empty())
			return;

		CharLowerBuffW( (LPWSTR) &_str[0], _str.length());
	}
	//--------------------------------------------------------------------------
	int WinUnicodeSupport::compareNoCase(const String& _first, const String& _second)
	{
		return _compareNoCase<sizeof(String::value_type)> (_first, _second);
	}
	//--------------------------------------------------------------------------
	static inline int checkStringEnd(size_t ofs1, size_t len1, size_t ofs2, size_t len2)
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
	int WinUnicodeSupport::_compareNoCase<1>(ConstStringHolder<1> _first, ConstStringHolder<1> _second)
	{
		size_t len1 = _first.length();
		size_t len2 = _second.length();
		size_t ofs1 = 0, ofs2 = 0;
		
		int r = checkStringEnd(ofs1, len1, ofs2, len2);
		if(r != 2) return r;

		const uint8* buf1 = _first.c_str();
		const uint8* buf2 = _second.c_str();
	
		while(true)
		{
			uint32 c1, c2;
			Utf8::next(buf1, ofs1, len1, c1);
			Utf8::next(buf2, ofs2, len2, c2);
			
			if(c1 <= 0xFFFF)
				c1 = (uint32) CharLowerW( (LPWSTR) c1);
			if(c2 <= 0xFFFF)
				c2 = (uint32) CharLowerW( (LPWSTR) c2);

			if(c1 != c2)
				return (c1 < c2) ? -1 : 1;

			r = checkStringEnd(ofs1, len1, ofs2, len2);
			if(r != 2) return r;
		}
	}
	//--------------------------------------------------------------------------
	template<>
	int WinUnicodeSupport::_compareNoCase<2>(ConstStringHolder<2> _first, ConstStringHolder<2> _second)
	{
		return CompareStringW(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE,
			char_cast<LPCWSTR>(_first.c_str()), _first.length(), 
			char_cast<LPCWSTR>(_second.c_str()), _second.length()) - 2;
	}





	//-----------------------------------------------------------------------------------
	// encodings and converters
	//-----------------------------------------------------------------------------------
	static WinUnicodeSupport* sp_Instance = nullptr;
	//-----------------------------------------------------------------------------------
	BOOL CALLBACK WinUnicodeSupport::codePageEnumProc(LPSTR _lpCodePageString)
	{
		uint16 codePage = (uint16) atol(_lpCodePageString);
		String encoding = String("Windows-") + StrConv::toString(codePage);
		sp_Instance->addEncoding2(encoding, codePage);
		return TRUE;
	}
	//----------------------------------------------------------------------------
	void WinUnicodeSupport::buildListEncodings()
	{
		sp_Instance = this;
		::EnumSystemCodePagesA(codePageEnumProc, CP_SUPPORTED);

		addEncoding2("UTF-8",    CP_UTF8);
		addEncoding2("UTF-16LE", CP_UTF16LE);
		addEncoding2("UTF-16BE", CP_UTF16BE);
		addEncoding2("UTF-32LE", CP_UTF32LE);
		addEncoding2("UTF-32BE", CP_UTF32BE);
	}
	//----------------------------------------------------------------------------
	void WinUnicodeSupport::addEncoding2(const String& _name, UINT _codePage)
	{
		addEncoding(_name);
		mCodepageByEncoding.insert(std::make_pair(normalizeName(_name), _codePage));
	}
	//----------------------------------------------------------------------------
	String WinUnicodeSupport::normalizeName(const String& _name)
	{
		String name2 = _name;
		toLowerCase(name2);
		size_t i = name2.length();
		while(i != 0)
		{
			--i;
			String::value_type c = name2[i];
			if(!isalnum(c))
				name2.erase(i, 1);
		}
		return name2;
	}
	//----------------------------------------------------------------------------
	WinUnicodeSupport::WinUnicodeSupport()
	{
		buildListEncodings();
	}
	//----------------------------------------------------------------------------
	WinUnicodeSupport::~WinUnicodeSupport()
	{
	}
	//----------------------------------------------------------------------------
	const String& WinUnicodeSupport::getName() const
	{
		static const String s = "WinUnicodeSupport";
		return s;
	}
	//----------------------------------------------------------------------------
	CodepageConverter* WinUnicodeSupport::openConverterImpl(const String& _encoding)
	{
		CodePageByEncoding::iterator it = mCodepageByEncoding.find(normalizeName(_encoding));
		if(it == mCodepageByEncoding.end())
			return nullptr;

		UINT codePage = it->second;
		return new WinCodepageConverter(codePage);
	}

}