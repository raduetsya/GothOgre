#include "GothOgre_Precompiled.h"
#include "GothOgre_TextureManagerEx.h"
#include "GothOgre_ResourceGroup.h"

//-------------------------------------------------------------------------
GothOgre::TextureManagerEx* 
	Ogre::Singleton<GothOgre::TextureManagerEx>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//----------------------------------------------------------------------------
	TextureManagerEx::TextureManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	TextureManagerEx::~TextureManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	Ogre::ResourceManager* TextureManagerEx::getResourceManager() const
	{
		return Ogre::TextureManager::getSingletonPtr();
	}
	//----------------------------------------------------------------------------
	const String& TextureManagerEx::getDefaultResourceGroup() const
	{
		return ResourceGroup::TEXTURES;
	}
	//----------------------------------------------------------------------------
	TextureLoader::TextureLoader(const String& _name, Priority _priority)
		: ResourceLoader(TextureManagerEx::getSingletonPtr(), _name, _priority)
	{
	}
	//----------------------------------------------------------------------------
	TextureSaver::TextureSaver(const String& _name, Priority _priority)
		: ResourceSaver(TextureManagerEx::getSingletonPtr(), _name, _priority)
	{
	}

} // namespace GothOgre