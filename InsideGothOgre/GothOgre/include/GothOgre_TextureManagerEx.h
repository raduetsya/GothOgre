#ifndef GOTHOGRE_TEXTURE_MANAGER_EX_H
#define GOTHOGRE_TEXTURE_MANAGER_EX_H

#include "GothOgre_ResourceManagerEx.h"

namespace GothOgre
{

	//------------------------------------------------------------------------
	/** Version of the TextureManager class with extended load/save support. */
	class GOTHOGRE_EXPORT TextureManagerEx : public Singleton<TextureManagerEx>, 
		public ResourceManagerEx
	{
	public:
		TextureManagerEx();
		~TextureManagerEx();
		
		Ogre::ResourceManager* getResourceManager() const;
		const String& getDefaultResourceGroup() const;
	};


	//------------------------------------------------------------------------
	/** Prototype for future texture loaders. */
	class GOTHOGRE_EXPORT TextureLoader : public ResourceLoader
	{
	public:
		TextureLoader(const String& _name, Priority _priority = Priority::LOWEST);
	};


	//------------------------------------------------------------------------
	/** Prototype for future texture savers. */
	class GOTHOGRE_EXPORT TextureSaver : public ResourceSaver
	{
	public:
		TextureSaver(const String& _name, Priority _priority = Priority::LOWEST);
	};

} //  namespace GothOgre

#endif // GOTHOGRE_TEXTURE_MANAGER_EX_H