#include "GothOgre_Precompiled.h"
#include "GothOgre_ResourceManagerEx.h"
#include "GothOgre_ResourceGroup.h"
#include "GothOgre_UnicodeUtil.h"


namespace GothOgre
{
	//----------------------------------------------------------------------
	ResourceManagerEx::ResourceManagerEx()
	{
	}
	//----------------------------------------------------------------------
	ResourceManagerEx::~ResourceManagerEx()
	{
	}
	//----------------------------------------------------------------------
	const String& ResourceManagerEx::getDefaultResourceGroup() const
	{
		return StringUtil::BLANK;
	}
	//----------------------------------------------------------------------
	bool ResourceManagerEx::isCaseSensitive() const
	{
		return false;
	}
	//----------------------------------------------------------------------
	ResourcePtr ResourceManagerEx::findByName(const String& _name)
	{
		return getResourceManager()->getByName(
			uppercasedName(_name),
			ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	}
	//----------------------------------------------------------------------
	ResourcePtr ResourceManagerEx::findByName(const String& _name, const String& _groupName)
	{
		return getResourceManager()->getByName(
			uppercasedName(_name),
			_groupName);
	}
	//----------------------------------------------------------------------
	ResourcePtr ResourceManagerEx::create(const String& _name, const String& _group)
	{
		ResourcePtr resource = getResourceManager()->createOrRetrieve(
			uppercasedName(_name),
			_group, true, this).first;

		//const String& resType = getOgreResourceManager()->getResourceType();
		//ResourceBackgroundQueue::getSingleton().prepare(resType, _name, _group,
		//	true, this); // prepare resource in background thread

		return resource;
	}
	//----------------------------------------------------------------------
	ResourcePtr ResourceManagerEx::create(const String& _name)
	{
		return create(_name, getDefaultResourceGroup());
	}
	//----------------------------------------------------------------------
	class EmptyResourceLoader : public ManualResourceLoader
	{
	public:
		virtual void loadResource(Ogre::Resource* resource) {}
	};
	//----------------------------------------------------------------------
	ResourcePtr ResourceManagerEx::createManual(const String& _name, const String& _groupName)
	{
		UppercasedName upName = uppercasedName(_name);

		// Always create a new resource
		static EmptyResourceLoader s_EmptyResourceLoader;
		getResourceManager()->remove(upName);

		// Prevent exception in ResourceGroupManager if the resource group doesn't exist
		if( !ResourceGroupManager::getSingleton().resourceGroupExists( _groupName ))
			ResourceGroupManager::getSingleton().createResourceGroup(_groupName);

		// Dummy "loading" the resource (with EmptyResourceLoader)
		ResourcePtr resource = getResourceManager()->create(upName, _groupName, true, &s_EmptyResourceLoader);
		resource->load();
		return resource;
	}
	//----------------------------------------------------------------------
	ResourcePtr ResourceManagerEx::createManual(const String& _name)
	{
		return createManual(uppercasedName(_name), ResourceGroup::MANUAL);
	}
	//----------------------------------------------------------------------
	void ResourceManagerEx::remove(const String& _name)
	{
		return getResourceManager()->remove(
			uppercasedName(_name));
	}
	//----------------------------------------------------------------------
	void ResourceManagerEx::remove(ResourceHandle _handle)
	{
		getResourceManager()->remove(_handle);
	}
	//----------------------------------------------------------------------
	bool ResourceManagerEx::canLoad(const String& _filename) const
	{
		return canLoad(_filename, getDefaultResourceGroup());
	}
	//----------------------------------------------------------------------
	bool ResourceManagerEx::canLoad(const String& _filename, const String& _resourceGroup) const
	{
		return ResourceLoaderEnumerator::canLoad(_filename, _resourceGroup);
	}
	//----------------------------------------------------------------------
	bool ResourceManagerEx::canLoad(Resource* _destResource) const
	{
		return ResourceLoaderEnumerator::canLoad(_destResource);
	}
	//----------------------------------------------------------------------
	bool ResourceManagerEx::canSave(const String& _filename) const
	{
		return canSave(_filename, getDefaultResourceGroup());
	}
	//----------------------------------------------------------------------
	bool ResourceManagerEx::canSave(const String& _filename, const String& _resourceGroup) const
	{
		return ResourceSaverEnumerator::canSave(_filename, _resourceGroup);
	}
	//----------------------------------------------------------------------
	ResourceManagerEx::UppercasedName::UppercasedName(const String& _name, bool _dontConvertToUpperCase)
	{
		if(_dontConvertToUpperCase)
		{
			mResult = &_name;
		}
		else
		{
			mTemp = _name;
			UnicodeUtil::toUpperCase(mTemp);
			mResult = &mTemp;
		}
	}

} // namespace GothOgre

