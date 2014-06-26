#ifndef WORLD_OBJECT_PROPERTIES_WINDOW_MANAGER_H
#define WORLD_OBJECT_PROPERTIES_WINDOW_MANAGER_H


namespace GUISystem
{
	class WorldObjectPropertiesWindow;

	class WorldObjectPropertiesWindowManager : public Singleton<WorldObjectPropertiesWindowManager>
	{
	public:
		WorldObjectPropertiesWindowManager();
		~WorldObjectPropertiesWindowManager();

		void setVisible(bool _show);
		bool isVisible() const;

	private:
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED( WorldObjectPropertiesWindowManager );

	private:
		WorldObjectPropertiesWindow*  mWindow;
	};
	
} // namespace GUISystem
	
#endif // WORLD_OBJECT_PROPERTIES_WINDOW_MANAGER_H