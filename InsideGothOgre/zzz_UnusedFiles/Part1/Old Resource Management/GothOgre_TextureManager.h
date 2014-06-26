#ifndef GOTHOGRE_TEXTURE_MANAGER_H
#define GOTHOGRE_TEXTURE_MANAGER_H

#include "GothOgre_ResourceManager.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT TextureManager : public ResourceManager, public Singleton<TextureManager>
	{
	public:
		static const String GROUP_NAME;
		virtual const String& getDefaultGroupName() const;
		virtual Ogre::ResourceManager* getOgreResourceManager() const;
	};

} // namespace GothOgre	

#endif // GOTHOGRE_TEXTURE_MANAGER_H