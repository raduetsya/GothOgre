#include "GothOgre_Precompiled.h"
#include "GothOgre_CodepageConverter.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	CodepageConverter::CodepageConverter() 
		: mUnicodeSupport(nullptr)
	{
	}
	//------------------------------------------------------------------------
	CodepageConverter::~CodepageConverter() 
	{
	}
	//------------------------------------------------------------------------
	void CodepageConverter::_setUnicodeSupport(UnicodeSupport* _usupport)
	{
		mUnicodeSupport = _usupport;
	}
	//------------------------------------------------------------------------
	void CodepageConverter::_setName(const String& _name)
	{
		mName = _name;
	}
}