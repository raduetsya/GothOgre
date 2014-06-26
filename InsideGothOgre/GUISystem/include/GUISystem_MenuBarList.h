#ifndef GUISYSTEM_MENU_BAR_LIST_H
#define GUISYSTEM_MENU_BAR_LIST_H

namespace GUISystem
{
	class MenuBar;

	/** Contains a list of menu bars, for accessing them by name. */
	class GUISYSTEM_EXPORT MenuBarList : public Singleton<MenuBarList>
	{
	public:
		MenuBarList();
		~MenuBarList();

		/** Returns a tool manager which has been created for the specified
		UI mode. This function throws an exception if there is no tool manager. */
		MenuBar* getMenuBar(const String& _name) const;

	private:
		friend class MenuBar;
		void addMenuBar(MenuBar* _menuBar);
		void removeMenuBar(MenuBar* _menuBar);

	private:
		typedef map<String, MenuBar*>::type MenuBarsByName;
		MenuBarsByName    mMenuBarsByName;
	};

} // namespace GUISystem

#endif // GUISYSTEM_MENU_BAR_LIST_H
