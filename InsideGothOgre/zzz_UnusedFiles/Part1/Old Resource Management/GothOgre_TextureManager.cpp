#include "GothOgre_StdHeaders.h"
#include "GothOgre_TextureManager.h"

GothOgre::TextureManager* Ogre::Singleton<GothOgre::TextureManager>::ms_Singleton = nullptr;
//-----------------------------------------------------------------------------

namespace GothOgre
{

	const String TextureManager::GROUP_NAME = "Textures";
	//-------------------------------------------------------------------------
	Ogre::ResourceManager* TextureManager::getOgreResourceManager() const
	{
		return Ogre::TextureManager::getSingletonPtr();
	}
	//-------------------------------------------------------------------------
	const String& TextureManager::getDefaultGroupName() const
	{
		return GROUP_NAME;
	}

} // namespace GothOgre

