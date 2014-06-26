#ifndef WORLD_VIEW_WINDOW_MANAGER_H
#define WORLD_VIEW_WINDOW_MANAGER_H


namespace GUISystem
{
	class WorldViewWindow;

	class WorldViewWindowManager : public Singleton<WorldViewWindowManager>
	{
	public:
		WorldViewWindowManager();
		~WorldViewWindowManager();

		size_t getNumWindows() const;
		WorldViewWindow* getWindow(size_t _index) const;

		size_t getActiveWindowIndex() const;
		void setActiveWindowIndex(size_t _index);

		WorldViewWindow* getActiveWindow() const;
		void setActiveWindow(WorldViewWindow* _window);

	public:
		void _processWindowClosing();

	private:
		GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_ACTIVE_CHANGED(WorldViewWindowManager);
		GOTHOGRE_DECLARE_WINDOW_CLOSING(WorldViewWindowManager);
		GOTHOGRE_DECLARE_FRAME_STARTED(WorldViewWindowManager);

		void notifyMessageBoxChoice(MessageBoxButton _button);
		void cancelWindowClosing();

	private:
		typedef vector<WorldViewWindow*>::type WindowList;
		WindowList  mWindows;
		size_t      mActiveIndex;
		Tool*       mSaveWorldTool;
		bool        mAppExitAfterWindowClosing;
	};

} // namespace GUISystem

#endif // WORLD_VIEW_WINDOW_MANAGER_H