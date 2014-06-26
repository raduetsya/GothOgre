#ifndef EXTSCENEMANAGER_FACTORY_H
#define EXTSCENEMANAGER_FACTORY_H

namespace GothOgre
{

	/// Factory for fast scene manager
	class ExtSceneManagerFactory : public SceneManagerFactory
	{
	protected:
		void initMetaData() const;
	public:
		ExtSceneManagerFactory();
		~ExtSceneManagerFactory();
		/// Factory type name
		static const String FACTORY_TYPE_NAME;
		Ogre::SceneManager* createInstance(const String& instanceName);
		void destroyInstance(Ogre::SceneManager* instance);
	};
	
} // namespace GothOgre

#endif // EXT_SCENE_MANAGER_FACTORY_H
