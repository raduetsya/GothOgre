#ifndef GUISYSTEM_HOTKEY_MANAGER_H
#define GUISYSTEM_HOTKEY_MANAGER_H

#include "GUISystem_ControlSettings.h"
#include "GUISystem_InputPriority.h"


namespace GUISystem
{
	class Tool;


	//-------------------------------------------------------------------------
	// BaseHotkeyManager
	//-------------------------------------------------------------------------
	/** Base class for the "HotkeyManager" class. */
	class GUISYSTEM_EXPORT BaseHotkeyManager
	{
	public:
		BaseHotkeyManager();
		~BaseHotkeyManager();

		/** Enable or disable the entire HotkeyManager, i.e. all hotkeys in it. */
		void setEnabled(bool _enable);
		void enable()  {setEnabled(true);}
		void disable() {setEnabled(false);}

		/** Whether this HotkeyManager is enabled */
		bool isEnabled() const;

		/** Insert a new hot key
		@param _id unique identifier of the inserted hot key
		@param _keyInControlSettings The name of the setting in ControlSettings
		which is used to read actual KeyList
		@param _sectionInControlSettings The name of the section it must be in
		*/
		void insertHotkey(const String& _id, const String& _keyInControlSettings, const String& _sectionInControlSettings, const KeyList& _defaultValue);
		
		/** Remove a hot key from the manager */
		void removeHotkey(const String& _id);

		/** Whether if a hot key with specified id exists */
		bool hasHotkey(const String& _id) const;

		/** Returns actual key list for a specified hot key. */
		const KeyList& getKeyList(const String& _id) const;

		/** Attach tool to an inserted hot key.
		The tool will be activated when hot key will be pressed */
		void attachTool(const String& _id, Tool* _tool, const Any& _toolParameter = Any());

		/** Detach tool */
		void detachTool(const String& _id);

		/** Enable or disable single hot key.
		@remark
		unique of the hot key */
		void setHotkeyEnabled(const String& _id, bool _enable);

		/** Enable a hot key. */
		inline void enableHotkey(const String& _id) {setHotkeyEnabled(_id, true);}

		/** Disable a hot key */
		inline void disableHotkey(const String& _id) {setHotkeyEnabled(_id, false);}

		/** Whether a hot key is enabled */
		bool isHotkeyEnabled(const String& _id) const;

	private:
		GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED( BaseHotkeyManager );
		GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED( BaseHotkeyManager );

	private:
		struct Hotkey
		{
			bool        enabled;
			String		keyInControlSettings;
			String		sectionInControlSettings;
			KeyList     defaultKeyList;
			KeyList		keyList;
			Tool*       tool;
			Any  		toolParameter;
		};
		typedef map<String, Hotkey*>::type	HotkeysById;

		struct HkSz
		{
			Hotkey* hotkey;
			size_t  keyCombSize;
		};

	private:
		Hotkey* findHotkey(const String& _id) const;
		Hotkey* getHotkey(const String& _id) const;
		void activateTool(Hotkey* _hotkey);

		void lockUntilKeyRelease(KeyCode _keyCode);
		bool isLocked() const;
		void checkIsLocked();
		int getCurrentInputPriority() const;

	private:
		bool			mEnabled;
		HotkeysById		mHotkeysById;
		KeyCode         mUnlockKeyCode;
	};




	//-------------------------------------------------------------------------
	// HotkeyManager
	//-------------------------------------------------------------------------
	/** Manages hot keys 
	@remark A hot key is a combination of keyboard keys
	which can activate some Tool */
	class GUISYSTEM_EXPORT HotkeyManager : public BaseHotkeyManager
	{
	public:
		HotkeyManager(const String& _name);
		~HotkeyManager();

		/** Returns the name of this HotkeyManager. */
		const String& getName() const;

	private:
		String  mName;
	};

} // namespace GUISystem

#endif // GUISYSTEM_HOTKEY_MANAGER_H
