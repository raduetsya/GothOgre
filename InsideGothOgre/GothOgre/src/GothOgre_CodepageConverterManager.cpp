#include "GothOgre_Precompiled.h"
#include "GothOgre_CodepageConverterManager.h"
#include "GothOgre_UnicodeManagerEnumerator.h"
#include "GothOgre_UnicodeManager.h"


//-------------------------------------------------------------------------
GothOgre::CodepageConverterManager* 
	Ogre::Singleton<GothOgre::CodepageConverterManager>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	CodepageConverterManager::CodepageConverterManager()
	{
	}
	//-------------------------------------------------------------------------
	CodepageConverterManager::~CodepageConverterManager()
	{
	}
	//-------------------------------------------------------------------------
	const StringVector& CodepageConverterManager::getAllEncodings()
	{
		UnicodeManager* mgr = UnicodeManagerEnumerator::getSingleton().getUnicodeManager();
		return mgr->getAllEncodings();		
	}
	//-------------------------------------------------------------------------
	CodepageConverter* CodepageConverterManager::openConverter(const String& _encoding)
	{
		UnicodeManager* mgr = UnicodeManagerEnumerator::getSingleton().getUnicodeManager();
		return mgr->openConverter(_encoding);
	}
}
