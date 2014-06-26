#ifndef PROFILER_CONFIGURATOR_H
#define PROFILER_CONFIGURATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;

	//-------------------------------------------------------------------------
	class ProfilerConfigurator
	{
	public:
		ProfilerConfigurator(SceneManagerExImpl* _sm);
		~ProfilerConfigurator();

		/** Called prior to the scene being rendered. */
		void preRenderScene(Viewport* _viewport);

		/** Called after the scene has been rendered. */
		void postRenderScene(Viewport* _viewport);

	private:
		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED( ProfilerConfigurator );
	
	private:
		bool                 mProfilerEnabled;
		SceneManagerExImpl*  mSceneManager;
	};
}

#endif // PROFILER_CONFIGURATOR_H