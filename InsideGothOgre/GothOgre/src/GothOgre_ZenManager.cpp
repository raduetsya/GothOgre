#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenObject.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_ZenFactory.h"

//-------------------------------------------------------------------------
GothOgre::ZenManager* 
	Ogre::Singleton<GothOgre::ZenManager>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//---------------------------------------------------------------------------
	ZenManager::ZenManager()
	{
	}
	//---------------------------------------------------------------------------
	ZenManager::~ZenManager()
	{
	}
	//---------------------------------------------------------------------------
	void ZenManager::addZenFactory(ZenFactory* _zenFactory)
	{
		const String& type = _zenFactory->getTypeName();
		mTypeNameFactoryMap[type] = _zenFactory;
	}
	//---------------------------------------------------------------------------
	void ZenManager::scanSupportedTypeNames()
	{
		list<ZenFactory*>::type zenFactories;
		
		for(TypeNameFactoryMap::iterator it = mTypeNameFactoryMap.begin();
			it != mTypeNameFactoryMap.end(); ++it)
		{
			ZenFactory* zenFactory = it->second;
			zenFactories.push_back(zenFactory);
		}

		for(list<ZenFactory*>::type::iterator it2 = zenFactories.begin();
			it2 != zenFactories.end(); ++it2)
		{
			ZenFactory* zenFactory = *it2;
			const vector<String>::type& suppTypeNames = zenFactory->getSupportedTypeNames();
			for(size_t i = 0; i < suppTypeNames.size(); ++i)
			{
				const String& suppTypeName = suppTypeNames[i];
				mTypeNameFactoryMap[suppTypeName] = zenFactory;
			}
		}
	}
	//---------------------------------------------------------------------------
	void ZenManager::removeZenFactory(ZenFactory* _zenFactory)
	{
		bool found;
		do
		{
			found = false;
			for(TypeNameFactoryMap::iterator it = mTypeNameFactoryMap.begin();
				it != mTypeNameFactoryMap.end(); ++it)
			{
				if(it->second == _zenFactory)
				{
					GOTHOGRE_INFO(_zenFactory->getTypeName() << ": numCreatedObjects = " << _zenFactory->getNumCreatedObjects() << ", numLivingObjects = " << _zenFactory->getNumLivingObjects());
					GOTHOGRE_ASSERT(_zenFactory->getNumLivingObjects() == 0, _zenFactory->getTypeName() << " - There are living objects created by the factory! All objects created by a factory must be destroyed before the factory can be removed.");
					mTypeNameFactoryMap.erase(it);
					found = true;
					break;
				}
			}
		}
		while(found);
	}
	//---------------------------------------------------------------------------
	ZenFactory* ZenManager::getZenFactory(const String& _type)
	{
		TypeNameFactoryMap::const_iterator it = mTypeNameFactoryMap.find(_type);
		if(it == mTypeNameFactoryMap.end())
		{
			scanSupportedTypeNames();
			
			it = mTypeNameFactoryMap.find(_type);
			if(it == mTypeNameFactoryMap.end())
				GOTHOGRE_EXCEPT("zen factory '" << _type << "' not found");
		}

		return it->second;
	}
	//---------------------------------------------------------------------------
	ZenObjectPtr ZenManager::createZenObject(const String& _typeName)
	{
		ZenFactory* factory = getZenFactory(_typeName);
		return factory->createZenObject();
	}

} // namespace GothOgre