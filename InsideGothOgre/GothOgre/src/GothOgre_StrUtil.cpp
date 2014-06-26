#include "GothOgre_Precompiled.h"
#include "GothOgre_StrUtil.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	template<typename T1, typename T2>
	static int compareTwoStringsNoCase(const T1* _p1, const T2* _p2)
	{
		int c1, c2;
		do
		{
			c1 = toupper(*_p1++);
			c2 = toupper(*_p2++);
			int d = c1 - c2;
			if(d)
			{
				return (d < 0) ? -1 : 1;
			}
		}
		while(c1 && c2);
		return 0;
	}
	//------------------------------------------------------------------------
	int StrUtil::compareNoCase(const String& _text1, const String& _text2)
	{
		const String::value_type* p1 = _text1.c_str();
		const String::value_type* p2 = _text2.c_str();
		return compareTwoStringsNoCase(p1, p2);	
	}
	//------------------------------------------------------------------------
	int StrUtil::compareNoCase(const String& _text1, const char* _text2)
	{
		const String::value_type* p1 = _text1.c_str();
		const char* p2 = _text2;
		return compareTwoStringsNoCase(p1, p2);	
	}
	//------------------------------------------------------------------------
	int StrUtil::compareNoCase(const char* _text1, const String& _text2)
	{
		const char* p1 = _text1;
		const String::value_type* p2 = _text2.c_str();
		return compareTwoStringsNoCase(p1, p2);	
	}
	//------------------------------------------------------------------------
	int StrUtil::compareNoCase(const char* _text1, const char* _text2)
	{
		const char* p1 = _text1;
		const char* p2 = _text2;
		return compareTwoStringsNoCase(p1, p2);	
	}
	//------------------------------------------------------------------------
	bool StrUtil::equalsNoCase(const String& _text1, const String& _text2)
	{
		return compareNoCase(_text1, _text2) == 0;
	}
	//------------------------------------------------------------------------
	bool StrUtil::equalsNoCase(const String& _text1, const char* _text2)
	{
		return compareNoCase(_text1, _text2) == 0;
	}
	//------------------------------------------------------------------------
	bool StrUtil::equalsNoCase(const char* _text1, const String& _text2)
	{
		return compareNoCase(_text1, _text2) == 0;
	}
	//------------------------------------------------------------------------
	bool StrUtil::equalsNoCase(const char* _text1, const char* _text2)
	{
		return compareNoCase(_text1, _text2) == 0;
	}
	//------------------------------------------------------------------------
	String& StrUtil::toUpperCase(String& _inOutString)
	{
		size_t len = _inOutString.length();
		if(len)
		{
			String::value_type* p = &_inOutString[0];
			while(len--)
			{
				*p = (String::value_type) toupper(*p);
				++p;
			}
		}
		return _inOutString;
	}
	//------------------------------------------------------------------------
	String& StrUtil::toLowerCase(String& _inOutString)
	{
		size_t len = _inOutString.length();
		if(len)
		{
			String::value_type* p = &_inOutString[0];
			while(len--)
			{
				*p = (String::value_type) tolower(*p);
				++p;
			}
		}
		return _inOutString;
	}

} // namespace GothOgre