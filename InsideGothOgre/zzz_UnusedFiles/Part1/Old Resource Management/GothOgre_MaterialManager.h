#ifndef GOTHOGRE_MATERIAL_MANAGER_H
#define GOTHOGRE_MATERIAL_MANAGER_H

#include "GothOgre_ResourceManager.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT MaterialManager : public ResourceManager, public Singleton<MaterialManager>
	{
	public:
		static const String GROUP_NAME;
		virtual const String& getDefaultGroupName() const;
		virtual Ogre::ResourceManager* getOgreResourceManager() const;

		virtual ResourcePtr getByName(const String& _name);
		virtual ResourcePtr create(const String& _name, const String& _groupName);
		virtual ResourceCreateOrRetrieveResult createOrRetrieve(const String& _name, const String& _groupName);
		virtual ResourcePtr createManual(const String& _name, const String& _groupName);

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
	};

} // namespace GothOgre	

#endif // GOTHOGRE_MATERIAL_MANAGER_H