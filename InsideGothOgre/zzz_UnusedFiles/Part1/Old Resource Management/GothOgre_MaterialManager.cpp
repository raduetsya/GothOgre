#include "GothOgre_StdHeaders.h"
#include "GothOgre_MaterialManager.h"
#include "GothOgre_StringUtil.h"

//-------------------------------------------------------------------------
GothOgre::MaterialManager* 
	Ogre::Singleton<GothOgre::MaterialManager>::ms_Singleton 
	= nullptr;

namespace GothOgre
{
	const String MaterialManager::GROUP_NAME = "Materials";
	//-------------------------------------------------------------------------
	Ogre::ResourceManager* MaterialManager::getOgreResourceManager() const
	{
		return Ogre::MaterialManager::getSingletonPtr();
	}
	//-------------------------------------------------------------------------
	const String& MaterialManager::getDefaultGroupName() const
	{
		return GROUP_NAME;
	}
	//-------------------------------------------------------------------------
	ResourcePtr MaterialManager::getByName(const String& _name)
	{
		return Ogre::MaterialManager::getSingleton().getByName(_name);
	}
	//-------------------------------------------------------------------------
	ResourcePtr MaterialManager::create(const String& _name, const String& _groupName)
	{
		return Ogre::MaterialManager::getSingleton().create(_name, _groupName);
	}
	//-------------------------------------------------------------------------
	ResourceManager::ResourceCreateOrRetrieveResult MaterialManager::createOrRetrieve(const String& _name, const String& _groupName)
	{
		return Ogre::MaterialManager::getSingleton().createOrRetrieve(_name, _groupName);
	}
	//-------------------------------------------------------------------------
	ResourcePtr MaterialManager::createManual(const String& _name, const String& _groupName)
	{
		return Ogre::MaterialManager::getSingleton().create(_name, _groupName);
	}

} // namespace GothOgre

