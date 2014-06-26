#include "GothOgre_StdHeaders.h"
#include "GothOgre_MeshManager.h"

GothOgre::MeshManager* Ogre::Singleton<GothOgre::MeshManager>::ms_Singleton = nullptr;
//-----------------------------------------------------------------------------

namespace GothOgre
{

	const String MeshManager::GROUP_NAME = "Meshes";
	//-------------------------------------------------------------------------
	Ogre::ResourceManager* MeshManager::getOgreResourceManager() const
	{
		return Ogre::MeshManager::getSingletonPtr();
	}
	//-------------------------------------------------------------------------
	const String& MeshManager::getDefaultGroupName() const
	{
		return GROUP_NAME;
	}

} // namespace GothOgre

