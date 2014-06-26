#ifndef EDITOR_MENU_BAR_H
#define EDITOR_MENU_BAR_H


namespace GUISystem
{
	/// Главное меню редактора
	class EditorMenuBar : public Singleton<EditorMenuBar>, public MenuBar
	{
	public:
		EditorMenuBar();
	};

} // namespace GUISystem

#endif // EDITOR_MENU_BAR_H