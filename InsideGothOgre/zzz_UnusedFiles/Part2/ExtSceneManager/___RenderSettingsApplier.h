#ifndef VIEWPORT_CONFIGURATOR_H
#define VIEWPORT_CONFIGURATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;

	//-------------------------------------------------------------------------
	class ViewportConfigurator
	{
	public:
		ViewportConfigurator(SceneManagerExImpl* _sm);
		~ViewportConfigurator();

		/** Called prior to the scene being rendered. */
		void preRenderScene(Viewport* _viewport);

		/** Called after the scene has been rendered. */
		void postRenderScene(Viewport* _viewport);

	private:
		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED( ViewportConfigurator );

	private:
		bool                 mFogDisabled;
		Real                 mMaxFogStart;
		Real                 mMaxFogEnd;
		Real                 mMaxFarClipDistance;
		bool                 mNoHardwareFarClipping;
		PolygonMode          mPolygonMode;
		SceneManagerExImpl*  mSceneManager;
	};

} // namespace GothOgre

#endif // VIEWPORT_CONFIGURATOR_H