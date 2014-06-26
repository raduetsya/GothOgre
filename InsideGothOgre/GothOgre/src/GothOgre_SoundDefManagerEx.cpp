#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundDefManagerEx.h"
#include "GothOgre_SoundDef.h"
#include "GothOgre_ResourceGroup.h"


//-------------------------------------------------------------------------
GothOgre::SoundDefManager* 
	Ogre::Singleton<GothOgre::SoundDefManager>::ms_Singleton 
	= nullptr;

GothOgre::SoundDefManagerEx* 
	Ogre::Singleton<GothOgre::SoundDefManagerEx>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// SoundDefManager
	//-------------------------------------------------------------------------
	SoundDefManager::SoundDefManager()
	{
		// Resource type
		mResourceType = "SoundDef";
		ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	}
	//-------------------------------------------------------------------------
	SoundDefManager::~SoundDefManager()
	{
		ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	}
	//-------------------------------------------------------------------------
	Resource* SoundDefManager::createImpl(const String& _name, ResourceHandle _handle, 
		const String& _group, bool _isManual, ManualResourceLoader* _loader, 
		const NameValuePairList* _createParams)
	{
		return OGRE_NEW SoundDef(this, _name, _handle, _group, _isManual, _loader);
	}


	//-------------------------------------------------------------------------
	// SoundDefManagerEx
	//-------------------------------------------------------------------------
	SoundDefManagerEx::SoundDefManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	SoundDefManagerEx::~SoundDefManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	Ogre::ResourceManager* SoundDefManagerEx::getResourceManager() const
	{
		return SoundDefManager::getSingletonPtr();
	}
	//----------------------------------------------------------------------------
	const String& SoundDefManagerEx::getDefaultResourceGroup() const
	{
		return ResourceGroup::SOUND;
	}

} // namespace GothOgre