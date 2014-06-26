#ifndef GOTHOGRE_DEFAULT_SCENE_MANAGER_H
#define GOTHOGRE_DEFAULT_SCENE_MANAGER_H

#include "GothOgre_SceneManager.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	/// Default version of SceneManager.
	class DefaultSceneManager : public SceneManager
	{
	public:
		DefaultSceneManager(const String& _instanceName);
		~DefaultSceneManager();
		const String& getTypeName() const;
	};


	//-----------------------------------------------------------------------
	// Factory for DefaultSceneManager
	class DefaultSceneManagerFactory : public Ogre::SceneManagerFactory
	{
	public:
		DefaultSceneManagerFactory();
		~DefaultSceneManagerFactory();
		static const String FACTORY_TYPE_NAME;
		void initMetaData() const;
		Ogre::SceneManager* createInstance(const String& instanceName);
		void destroyInstance(Ogre::SceneManager* instance);
	};
}

#endif // GOTHOGRE_DEFAULT_SCENE_MANAGER_H
