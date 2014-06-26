#include "GothOgre_StdHeaders.h"
#include "GothOgre_ResourceManager.h"
#include "GothOgre_StringUtil.h"

namespace GothOgre
{
	//----------------------------------------------------------------------
	ResourceManager::ResourceManager()
	{
	}
	//----------------------------------------------------------------------
	ResourceManager::~ResourceManager()
	{
	}
	//----------------------------------------------------------------------
	const String& ResourceManager::getDefaultGroupName() const
	{
		return ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;
	}
	//----------------------------------------------------------------------
	ResourcePtr ResourceManager::getByName(const String& _name)
	{
		if(isCaseSensitive())
		{
			return getOgreResourceManager()->getByName(_name);
		}
		else
		{
			String lowcase_name = _name;
			StringUtil::toLowerCase(lowcase_name);
			return getOgreResourceManager()->getByName(lowcase_name);
		}
	}
	//----------------------------------------------------------------------
	ResourcePtr ResourceManager::create(const String& _name, const String& _groupName)
	{
		if(isCaseSensitive())
		{
			return getOgreResourceManager()->create(_name, _groupName, true, this);
		}
		else
		{
			String lowcase_name = _name;
			StringUtil::toLowerCase(lowcase_name);
			return getOgreResourceManager()->create(lowcase_name, _groupName, true, this);
		}
	}
	//----------------------------------------------------------------------
	ResourceManager::ResourceCreateOrRetrieveResult ResourceManager::createOrRetrieve(const String& _name, const String& _groupName)
	{
		if(isCaseSensitive())
		{
			return getOgreResourceManager()->createOrRetrieve(_name, _groupName, true, this);
		}
		else
		{
			String lowcase_name = _name;
			StringUtil::toLowerCase(lowcase_name);
			return getOgreResourceManager()->createOrRetrieve(lowcase_name, _groupName, true, this);
		}
	}
	//----------------------------------------------------------------------
	class EmptyResourceLoader : public Ogre::ManualResourceLoader
	{
	public:
		virtual void loadResource(Resource* resource) {}
	};
	//----------------------------------------------------------------------
	ResourcePtr ResourceManager::createManual(const String& _name, const String& _groupName)
	{
		static EmptyResourceLoader s_EmptyResourceLoader;
		ResourcePtr ptr;
		if(isCaseSensitive())
		{
			getOgreResourceManager()->remove(_name);
			ptr = getOgreResourceManager()->create(_name, _groupName, true, &s_EmptyResourceLoader);
		}
		else
		{
			String lowcase_name = _name;
			StringUtil::toLowerCase(lowcase_name);
			getOgreResourceManager()->remove(lowcase_name);
			ptr = getOgreResourceManager()->create(lowcase_name, _groupName, true, &s_EmptyResourceLoader);
		}
		ptr->load();
		return ptr;
	}
	//----------------------------------------------------------------------
	void ResourceManager::remove(const String& _name)
	{
		if(isCaseSensitive())
		{
			return getOgreResourceManager()->remove(_name);
		}
		else
		{
			String lowcase_name = _name;
			StringUtil::toLowerCase(lowcase_name);
			return getOgreResourceManager()->remove(lowcase_name);
		}
	}
	//----------------------------------------------------------------------
	void ResourceManager::remove(ResourcePtr& r)
	{
		String name = r->getName();
		r.setNull();
		remove(name);
	}

} // namespace GothOgre

