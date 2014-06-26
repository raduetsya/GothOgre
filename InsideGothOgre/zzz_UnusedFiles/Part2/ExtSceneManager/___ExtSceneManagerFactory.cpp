#include "Precompiled.h"
#include "ExtSceneManagerFactory.h"
#include "ExtSceneManager.h"

namespace GothOgre
{
	const String ExtSceneManagerFactory::FACTORY_TYPE_NAME = "GothOgre::ExtSceneManager";
	//-----------------------------------------------------------------------------
	void ExtSceneManagerFactory::initMetaData() const
	{
		mMetaData.typeName = FACTORY_TYPE_NAME;
		mMetaData.description = FACTORY_TYPE_NAME;
		mMetaData.sceneTypeMask = ST_GOTHOGRE;
		mMetaData.worldGeometrySupported = false;
	}
	//-----------------------------------------------------------------------------
	ExtSceneManagerFactory::ExtSceneManagerFactory() 
	{
		SceneManagerEnumerator::getSingleton().addFactory(this);
	}
	//-----------------------------------------------------------------------------
	ExtSceneManagerFactory::~ExtSceneManagerFactory() 
	{
		SceneManagerEnumerator::getSingleton().removeFactory(this);
	}
	//-----------------------------------------------------------------------------
	Ogre::SceneManager* ExtSceneManagerFactory::createInstance(const String& _instanceName)
	{
		return OGRE_NEW ExtSceneManager(_instanceName);
	}
	//-----------------------------------------------------------------------------
	void ExtSceneManagerFactory::destroyInstance(Ogre::SceneManager* _instance)
	{
		OGRE_DELETE _instance;
	}


} // namespace GothOgre