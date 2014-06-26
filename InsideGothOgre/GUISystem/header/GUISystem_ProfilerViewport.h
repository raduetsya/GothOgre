#ifndef GUISYSTEM_PROFILER_VIEWPORT_H
#define GUISYSTEM_PROFILER_VIEWPORT_H


namespace GUISystem
{
	class ProfilerSceneManager;
	class ProfilerSceneManagerFactory;


	// This class can show the standard Ogre profilers
	// in a special viewport.
	class ProfilerViewport
	{
	public:
		ProfilerViewport();
		~ProfilerViewport();

	private:
		void startProfiler();
		void stopProfiler();

		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED(ProfilerViewport)

	private:
		bool                          mProfilerEnabled;
		Real                          mScrollX;
		Real                          mScrollY;
		Viewport*                     mViewport;
		Camera*                       mDummyCamera;
		ProfilerSceneManager*         mProfilerSceneManager;
		ProfilerSceneManagerFactory*  mProfilerSceneManagerFactory;
	};

} // namespace GUISystem

#endif // GUISYSTEM_PROFILER_VIEWPORT_H