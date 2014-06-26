#include "GothOgre_Precompiled.h"
#include "GothOgre_UnicodeUtil.h"
#include "GothOgre_UnicodeSupport.h"


namespace GothOgre
{
	String& UnicodeUtil::toUpperCase(String& _inOutString)
	{
		UnicodeSupport* usupport = UnicodeSupportRegistration::getSingleton().getSupport();
		usupport->toUpperCase(_inOutString);
		return _inOutString;
	}
	//-------------------------------------------------------------------------------
	String& UnicodeUtil::toLowerCase(String& _inOutString)
	{
		UnicodeSupport* usupport = UnicodeSupportRegistration::getSingleton().getSupport();
		usupport->toLowerCase(_inOutString);
		return _inOutString;
	}
	//-------------------------------------------------------------------------------
	int UnicodeUtil::compareNoCase(const String& _text1, const String& _text2)
	{
		UnicodeSupport* usupport = UnicodeSupportRegistration::getSingleton().getSupport();
		return usupport->compareNoCase(_text1, _text2);
	}
	//-------------------------------------------------------------------------------
	bool UnicodeUtil::equalsNoCase(const String& _text1, const String& _text2)
	{
		return compareNoCase(_text1, _text2) == 0;
	}
	//-------------------------------------------------------------------------------
	const StringVector& UnicodeUtil::getAllEncodings()
	{
		UnicodeSupport* usupport = UnicodeSupportRegistration::getSingleton().getSupport();
		return usupport->getAllEncodings();
	}
	//-------------------------------------------------------------------------------
	CodepageConverter* UnicodeUtil::openConverter(const String& _encoding)
	{
		UnicodeSupport* usupport = UnicodeSupportRegistration::getSingleton().getSupport();
		return usupport->openConverter(_encoding);
	}		
	
} // namespace GothOgre