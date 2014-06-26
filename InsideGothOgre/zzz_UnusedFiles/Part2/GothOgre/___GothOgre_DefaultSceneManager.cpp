#include "GothOgre_Precompiled.h"
#include "GothOgre_DefaultSceneManager.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	DefaultSceneManager::DefaultSceneManager(const String& _instanceName)
		: SceneManager(_instanceName)
	{
		GOTHOGRE_WARNING("The default scene manager used. Some features will be unavailable.");
	}

	DefaultSceneManager::~DefaultSceneManager()
	{
	}

	const String& DefaultSceneManager::getTypeName() const
	{
		return DefaultSceneManagerFactory::FACTORY_TYPE_NAME;
	}



	//-----------------------------------------------------------------------
	const String DefaultSceneManagerFactory::FACTORY_TYPE_NAME = "GothOgre::DefaultSceneManager";

	void DefaultSceneManagerFactory::initMetaData() const
	{
		mMetaData.typeName = FACTORY_TYPE_NAME;
		mMetaData.description = FACTORY_TYPE_NAME;
		mMetaData.sceneTypeMask = ST_GOTHOGRE_DEFAULT;
		mMetaData.worldGeometrySupported = false;
	}

	Ogre::SceneManager* DefaultSceneManagerFactory::createInstance(const String& _instanceName)
	{
		return new DefaultSceneManager(_instanceName);
	}

	void DefaultSceneManagerFactory::destroyInstance(Ogre::SceneManager* _instance)
	{
		delete _instance;
	}

	DefaultSceneManagerFactory::DefaultSceneManagerFactory()
	{
		SceneManagerEnumerator::getSingleton().addFactory(this);
	}

	DefaultSceneManagerFactory::~DefaultSceneManagerFactory()
	{
		SceneManagerEnumerator::getSingleton().removeFactory(this);
	}

} // namespace GothOgre

