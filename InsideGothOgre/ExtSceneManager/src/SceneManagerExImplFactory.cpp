#include "Precompiled.h"
#include "SceneManagerExImplFactory.h"
#include "SceneManagerExImpl.h"

namespace GothOgre
{
	const String SceneManagerExImplFactory::FACTORY_TYPE_NAME = "SceneManagerExImpl";
	//-----------------------------------------------------------------------------
	void SceneManagerExImplFactory::initMetaData() const
	{
		mMetaData.typeName = FACTORY_TYPE_NAME;
		mMetaData.description = FACTORY_TYPE_NAME;
		mMetaData.sceneTypeMask = ST_GOTHOGRE;
		mMetaData.worldGeometrySupported = false;
	}
	//-----------------------------------------------------------------------------
	SceneManagerExImplFactory::SceneManagerExImplFactory() 
	{
		SceneManagerEnumerator::getSingleton().addFactory(this);
	}
	//-----------------------------------------------------------------------------
	SceneManagerExImplFactory::~SceneManagerExImplFactory() 
	{
		SceneManagerEnumerator::getSingleton().removeFactory(this);
	}
	//-----------------------------------------------------------------------------
	Ogre::SceneManager* SceneManagerExImplFactory::createInstance(const String& _instanceName)
	{
		return OGRE_NEW SceneManagerExImpl(_instanceName);
	}
	//-----------------------------------------------------------------------------
	void SceneManagerExImplFactory::destroyInstance(Ogre::SceneManager* _instance)
	{
		OGRE_DELETE _instance;
	}


} // namespace GothOgre