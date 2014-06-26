#include "GothOgre_Precompiled.h"
#include "GothOgre_MtlManagerEx.h"
#include "GothOgre_Mtl.h"


//-------------------------------------------------------------------------
GothOgre::MtlManager* 
	Ogre::Singleton<GothOgre::MtlManager>::ms_Singleton 
	= nullptr;

GothOgre::MtlManagerEx* 
	Ogre::Singleton<GothOgre::MtlManagerEx>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// MtlManager
	//-------------------------------------------------------------------------
	MtlManager::MtlManager()
	{
		// Resource type
		mResourceType = "Mtl";
		ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	}
	//-------------------------------------------------------------------------
	MtlManager::~MtlManager()
	{
		ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	}
	//-------------------------------------------------------------------------
	Resource* MtlManager::createImpl(const String& _name, ResourceHandle _handle, 
		const String& _group, bool _isManual, ManualResourceLoader* _loader, 
		const NameValuePairList* _createParams)
	{
		return OGRE_NEW Mtl(this, _name, _handle, _group, _isManual, _loader);
	}


	//-------------------------------------------------------------------------
	// MtlManagerEx
	//-------------------------------------------------------------------------
	MtlManagerEx::MtlManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	MtlManagerEx::~MtlManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	Ogre::ResourceManager* MtlManagerEx::getResourceManager() const
	{
		return MtlManager::getSingletonPtr();
	}

} // namespace GothOgre