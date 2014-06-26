#include "GothOgre_Precompiled.h"
#include "GothOgre_ResourceGroup.h"
#include "GothOgre_BspManagerEx.h"
#include "GothOgre_Bsp.h"

//-------------------------------------------------------------------------
GothOgre::BspManager* 
	Ogre::Singleton<GothOgre::BspManager>::ms_Singleton 
	= nullptr;

GothOgre::BspManagerEx*
	Ogre::Singleton<GothOgre::BspManagerEx>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// BspManager
	//-------------------------------------------------------------------------
	BspManager::BspManager()
	{
		// Resource type
		mResourceType = "Bsp";
		ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	}
	//-------------------------------------------------------------------------
	BspManager::~BspManager()
	{
		ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	}
	//-------------------------------------------------------------------------
	Resource* BspManager::createImpl(const String& _name, ResourceHandle _handle, 
		const String& _group, bool _isManual, ManualResourceLoader* _loader, 
		const NameValuePairList* _createParams)
	{
		return OGRE_NEW Bsp(this, _name, _handle, _group, _isManual, _loader);
	}


	//-------------------------------------------------------------------------
	// BspManagerEx
	//-------------------------------------------------------------------------
	BspManagerEx::BspManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	BspManagerEx::~BspManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	const String& BspManagerEx::getDefaultResourceGroup() const
	{
		return ResourceGroup::WORLDS;
	}
	//----------------------------------------------------------------------------
	ResourceManager* BspManagerEx::getResourceManager() const
	{
		return BspManager::getSingletonPtr();
	}
	//----------------------------------------------------------------------------
	bool BspManagerEx::isCaseSensitive() const
	{
		return false;
	}

} // namespace GothOgre