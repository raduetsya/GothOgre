#ifndef LIGHT_SHADOW_CONFIGURATOR_H
#define LIGHT_SHADOW_CONFIGURATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;

	//-------------------------------------------------------------------------
	class LightShadowConfigurator
	{
	public:
		LightShadowConfigurator(SceneManagerExImpl* _sm);
		~LightShadowConfigurator();

		/** Called prior to the scene being rendered. */
		void preRenderScene(Viewport* _viewport);

		/** Called after the scene has been rendered. */
		void postRenderScene(Viewport* _viewport);

	private:
		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED( LightShadowConfigurator );

	private:
		SceneManagerExImpl*  mSceneManager;
	};

} // namespace GothOgre

#endif // LIGHT_SHADOW_CONFIGURATOR_H