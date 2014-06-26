#ifndef GUISYSTEM_HOTKEY_MANAGER_LIST_H
#define GUISYSTEM_HOTKEY_MANAGER_LIST_H


namespace GUISystem
{
	class HotkeyManager;

	/** Contains a list of hotkey managers, for accessing them by name. */
	class GUISYSTEM_EXPORT HotkeyManagerList : public Singleton<HotkeyManagerList>
	{
	public:
		HotkeyManagerList();
		~HotkeyManagerList();
		HotkeyManager* getHotkeyManager(const String& _name) const;

	private:
		friend class HotkeyManager;
		void addHotkeyManager(HotkeyManager* _hotkeyManager);
		void removeHotkeyManager(HotkeyManager* _hotkeyManager);

	private:
		typedef map<String, HotkeyManager*>::type HotkeyManagersByName;
		HotkeyManagersByName    mHotkeyManagersByName;
	};

} // namespace GUISystem

#endif // GUISYSTEM_HOTKEY_MANAGER_LIST_H
