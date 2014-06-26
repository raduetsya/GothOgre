#ifndef SCENE_MANAGER_EX_IMPL_PLUGIN_H
#define SCENE_MANAGER_EX_IMPL_PLUGIN_H


namespace GothOgre
{
	class SceneManagerExImplFactory;

	class SceneManagerExImplPlugin : public Plugin
	{
	public:
		SceneManagerExImplPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	protected:
		SceneManagerExImplFactory*	mSceneManagerExImplFactory;
	};

} // namespace GothOgre

#endif // SCENE_MANAGER_EX_IMPL_PLUGIN_H
