#include "GothOgre_Precompiled.h"
#include "GothOgre_MaterialManagerEx.h"


//-------------------------------------------------------------------------
GothOgre::MaterialManagerEx* 
	Ogre::Singleton<GothOgre::MaterialManagerEx>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{

	MaterialManagerEx::MaterialManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	MaterialManagerEx::~MaterialManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	const String& MaterialManagerEx::getDefaultResourceGroup() const
	{
		static const String s = "Materials";
		return s;
	}
	//----------------------------------------------------------------------------
	ResourceManager* MaterialManagerEx::getResourceManager() const
	{
		return MaterialManager::getSingletonPtr();
	}
	//----------------------------------------------------------------------------
	bool MaterialManagerEx::isCaseSensitive() const
	{
		return false;
	}

} // namespace GothOgre