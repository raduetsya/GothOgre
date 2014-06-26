#ifndef GOTHOGRE_MESH_MANAGER_H
#define GOTHOGRE_MESH_MANAGER_H

#include "GothOgre_ResourceManager.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT MeshManager : public ResourceManager, public Singleton<MeshManager>
	{
	public:
		static const String GROUP_NAME;
		virtual const String& getDefaultGroupName() const;
		virtual Ogre::ResourceManager* getOgreResourceManager() const;
	};

} // namespace GothOgre	

#endif // GOTHOGRE_MESH_MANAGER_H