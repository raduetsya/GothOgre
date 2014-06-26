#ifndef GUISYSTEM_ROOT_H
#define GUISYSTEM_ROOT_H


namespace GUISystem
{
	class ToolManagerList;
	class HotkeyManagerList;
	class MenuBarList;
	class ControlSettings;
	class UISettings;
	class WorldEditSettings;
	class ProgressDialogManager;
	class FrameStatsWindowManager;
	class UserTagManager;
	class MyGUIInput;
	class ProfilerViewport;

	class GUISystem
	{
	public:
		GUISystem();
		~GUISystem();

	private:
		GOTHOGRE_DECLARE_UI_MODE_ENTERED_EXITED( GUISystem );

	private:
		void initialise();
		void initialise_before_MyGUI();
		void initialise_MyGUI();
		void initialise_after_MyGUI();

		void shutdown();
		void shutdown_before_MyGUI();
		void shutdown_MyGUI();
		void shutdown_after_MyGUI();

	private:
		static unsigned short getViewportIndex(Viewport* _viewport);

	private:
		bool					mInitialised;
		MyGUIInput*				mMyGUIInput;
		ToolManagerList*		mToolManagerList;
		HotkeyManagerList*		mHotkeyManagerList;
		MenuBarList*			mMenuBarList;
		ControlSettings*		mControlSettings;
		UISettings*				mUISettings;
		WorldEditSettings*      mWorldEditSettings;
		ProgressDialogManager*	mProgressDialogManager;
		FrameStatsWindowManager*  mFrameStatsWindowManager;
		UserTagManager*         mUserTagManager;

		RenderWindow*           mWindow;
		SceneManager*           mUISceneManager;
		Camera*                 mUICamera;
		Viewport*               mUIViewport;

		ProfilerViewport*       mProfilerViewport;

		MyGUI::OgrePlatform*    mPlatform;
		MyGUI::Gui*             mGui;
	};

} // namespace GUISystem

#endif // GUISYSTEM_ROOT_H
