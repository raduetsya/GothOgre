#ifndef GOTHOGRE_RESOURCE_MANAGER_H
#define GOTHOGRE_RESOURCE_MANAGER_H

#include "GothOgre_ResourceLoader.h"
#include "GothOgre_ResourceSaver.h"


namespace GothOgre
{
	class GOTHOGRE_EXPORT ResourceManager : public ResourceLoaderManager, public ResourceSaverManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		virtual Ogre::ResourceManager* getOgreResourceManager() const = 0;
		
		virtual bool isCaseSensitive() const {return false;}
		
		virtual const String& getDefaultGroupName() const;

		virtual ResourcePtr getByName(const String& _name);
		
		virtual ResourcePtr create(const String& _name, const String& _groupName);
		
		typedef Ogre::ResourceManager::ResourceCreateOrRetrieveResult ResourceCreateOrRetrieveResult;
		
		virtual ResourceCreateOrRetrieveResult createOrRetrieve(const String& _name, const String& _groupName);
		
		ResourcePtr create(const String& _name)
		{
			return create(_name, getDefaultGroupName());
		}

		ResourceCreateOrRetrieveResult createOrRetrieve(const String& _name)
		{
			return createOrRetrieve(_name, getDefaultGroupName());
		}

		ResourcePtr createManual(const String& _name)
		{
			return createManual(_name, getDefaultGroupName());
		}

		virtual ResourcePtr createManual(const String& _name, const String& _groupName);

		void remove(const String& _name);

		void remove(ResourcePtr& r);
	};

} // namespace GothOgre

#endif // GOTHOGRE_RESOURCE_MANAGER_H