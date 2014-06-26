#include "GothOgre_Precompiled.h"
#include "GothOgre_ResourceEx.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// Helper hack
	//---------------------------------------------------------------------------
	class ResourceManagerHack : public ResourceManager
	{
	public:
		void nameChanging(const String& _oldName, const String& _newName)
		{
			OGRE_LOCK_AUTO_MUTEX
				
			// Update the "mResources" member variable
			ResourceMap::iterator itResource = mResources.find(_oldName);
			if(itResource != mResources.end())
			{
				ResourcePtr res = itResource->second;
				mResources.erase(itResource);
				mResources.insert(std::make_pair(_newName, res));

				// Update the "mResourcesWithGroup" member variable
				ResourceWithGroupMap::iterator itGroup = mResourcesWithGroup.find(res->getGroup());
				if( itGroup != mResourcesWithGroup.end())
				{
					ResourceMap& resourceMap2 = itGroup->second;
					ResourceMap::iterator itResource2 = resourceMap2.find(_oldName);
					if(itResource2 != resourceMap2.end())
					{
						resourceMap2.erase(itResource2);
						resourceMap2.insert(std::make_pair(_newName, res));
					}
				}
			}
		}
	};



	//---------------------------------------------------------------------------
	// ResourceEx
	//---------------------------------------------------------------------------
	void ResourceEx::setName(const String& _name)
	{
		if(mName != _name)
		{
			OGRE_LOCK_AUTO_MUTEX
			ResourceManagerHack* hack = (ResourceManagerHack*)mCreator;
			hack->nameChanging(mName, _name);
			mName = _name;
			_dirtyState();
		}
	}
	//---------------------------------------------------------------------------
	ResourceEx::ResourceEx(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
		const String& _group, bool _isManual, ManualResourceLoader* _loader)
	: Resource(_creator, _name, _handle, _group, _isManual, _loader)
	{
	}
	//---------------------------------------------------------------------------
	ResourceEx::~ResourceEx()
	{
	}


} // namespace GothOgre