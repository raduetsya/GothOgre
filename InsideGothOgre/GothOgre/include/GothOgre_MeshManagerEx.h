#ifndef GOTHOGRE_MESH_MANAGER_EX_H
#define GOTHOGRE_MESH_MANAGER_EX_H

#include "GothOgre_ResourceManagerEx.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------------
	/** Resource manager for new resource type "MeshEx",
	with extended support for load/save functionality. */
	class GOTHOGRE_EXPORT MeshManagerEx : public ResourceManagerEx,
		public Singleton<MeshManagerEx>
	{
	public:
		MeshManagerEx();
		~MeshManagerEx();
		
		Ogre::ResourceManager* getResourceManager() const;
		const String& getDefaultResourceGroup() const;
	};

	//------------------------------------------------------------------------
	/** Prototype for future skydef loaders. */
	class GOTHOGRE_EXPORT MeshLoader : public ResourceLoader
	{
	public:
		MeshLoader(const String& _name, Priority _priority = Priority::LOWEST);
	};

	//------------------------------------------------------------------------
	/** Prototype for future mesh savers. */
	class GOTHOGRE_EXPORT MeshSaver : public ResourceSaver
	{
	public:
		MeshSaver(const String& _name, Priority _priority = Priority::LOWEST);
	};

} // namespace GothOgre

#endif // GOTHOGRE_MESH_MANAGER_EX_H
