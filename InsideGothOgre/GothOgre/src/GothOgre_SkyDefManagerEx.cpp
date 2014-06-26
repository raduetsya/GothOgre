#include "GothOgre_Precompiled.h"
#include "GothOgre_SkyDefManagerEx.h"
#include "GothOgre_SkyDef.h"
#include "GothOgre_ResourceGroup.h"


//-------------------------------------------------------------------------
GothOgre::SkyDefManager* 
	Ogre::Singleton<GothOgre::SkyDefManager>::ms_Singleton 
	= nullptr;

GothOgre::SkyDefManagerEx* 
	Ogre::Singleton<GothOgre::SkyDefManagerEx>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// SkyDefManager
	//-------------------------------------------------------------------------
	SkyDefManager::SkyDefManager()
	{
		// Resource type
		mResourceType = "SkyDef";
		ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	}
	//-------------------------------------------------------------------------
	SkyDefManager::~SkyDefManager()
	{
		ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	}
	//-------------------------------------------------------------------------
	Resource* SkyDefManager::createImpl(const String& _name, ResourceHandle _handle, 
		const String& _group, bool _isManual, ManualResourceLoader* _loader, 
		const NameValuePairList* _createParams)
	{
		return OGRE_NEW SkyDef(this, _name, _handle, _group, _isManual, _loader);
	}


	//-------------------------------------------------------------------------
	// SkyDefManagerEx
	//-------------------------------------------------------------------------
	SkyDefManagerEx::SkyDefManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	SkyDefManagerEx::~SkyDefManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	Ogre::ResourceManager* SkyDefManagerEx::getResourceManager() const
	{
		return SkyDefManager::getSingletonPtr();
	}
	//----------------------------------------------------------------------------
	const String& SkyDefManagerEx::getDefaultResourceGroup() const
	{
		return ResourceGroup::SKIES;
	}
	//----------------------------------------------------------------------------
	SkyDefLoader::SkyDefLoader(const String& _name, Priority _priority)
		: ResourceLoader(SkyDefManagerEx::getSingletonPtr(), _name, _priority)
	{
	}

} // namespace GothOgre