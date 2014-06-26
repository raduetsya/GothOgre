#ifndef SCENE_MANAGER_EX_IMPL_FACTORY_H
#define SCENE_MANAGER_EX_IMPL_FACTORY_H

namespace GothOgre
{

	/// Factory for the scene manager
	class SceneManagerExImplFactory : public SceneManagerFactory
	{
	protected:
		void initMetaData() const;
	public:
		SceneManagerExImplFactory();
		~SceneManagerExImplFactory();
		/// Factory type name
		static const String FACTORY_TYPE_NAME;
		SceneManager* createInstance(const String& instanceName);
		void destroyInstance(SceneManager* instance);
	};
	
} // namespace GothOgre

#endif // SCENE_MANAGER_EX_IMPL_FACTORY_H
