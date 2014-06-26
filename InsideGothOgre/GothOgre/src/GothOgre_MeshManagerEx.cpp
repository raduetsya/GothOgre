#include "GothOgre_Precompiled.h"
#include "GothOgre_MeshManagerEx.h"
#include "GothOgre_MeshEx.h"
#include "GothOgre_ResourceGroup.h"


//-------------------------------------------------------------------------
GothOgre::MeshManagerEx* 
	Ogre::Singleton<GothOgre::MeshManagerEx>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// MeshManagerEx
	//-------------------------------------------------------------------------
	MeshManagerEx::MeshManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	MeshManagerEx::~MeshManagerEx()
	{
	}
	//----------------------------------------------------------------------------
	Ogre::ResourceManager* MeshManagerEx::getResourceManager() const
	{
		return MeshManager::getSingletonPtr();
	}
	//----------------------------------------------------------------------------
	const String& MeshManagerEx::getDefaultResourceGroup() const
	{
		return ResourceGroup::MESHES;
	}
	//----------------------------------------------------------------------------
	MeshLoader::MeshLoader(const String& _name, Priority _priority)
		: ResourceLoader(MeshManagerEx::getSingletonPtr(), _name, _priority)
	{
	}
	//----------------------------------------------------------------------------
	MeshSaver::MeshSaver(const String& _name, Priority _priority)
		: ResourceSaver(MeshManagerEx::getSingletonPtr(), _name, _priority)
	{
	}

} // namespace GothOgre