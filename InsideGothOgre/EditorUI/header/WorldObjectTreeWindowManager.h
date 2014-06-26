#ifndef WORLD_OBJECT_TREE_WINDOW_MANAGER_H
#define WORLD_OBJECT_TREE_WINDOW_MANAGER_H


namespace GUISystem
{
	class WorldObjectTreeWindow;

	class WorldObjectTreeWindowManager : public Singleton<WorldObjectTreeWindowManager>
	{
	public:
		WorldObjectTreeWindowManager();
		~WorldObjectTreeWindowManager();

		void setVisible(bool _show);
		bool isVisible() const;

	private:
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED( WorldObjectTreeWindowManager );

	private:
		WorldObjectTreeWindow*  mWindow;
	};
	
} // namespace GUISystem
	
#endif // WORLD_OBJECT_TREE_WINDOW_MANAGER_H