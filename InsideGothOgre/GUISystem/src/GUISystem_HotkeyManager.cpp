#include "GUISystem_Precompiled.h"
#include "GUISystem_InputPriority.h"
#include "GUISystem_HotkeyManager.h"
#include "GUISystem_HotkeyManagerList.h"
#include "GUISystem_Tool.h"
#include "GUISystem_ToolOperation.h"
#include "GUISystem_ToolManagerList.h"

namespace GUISystem
{
	//-------------------------------------------------------------------------
	BaseHotkeyManager::BaseHotkeyManager()
	{
		mUnlockKeyCode = KeyCode::NONE; // "NONE" means "not locked"
		mEnabled = false;
		setEnabled(true);
	}
	//-------------------------------------------------------------------------
	BaseHotkeyManager::~BaseHotkeyManager()
	{
		for(HotkeysById::iterator it = mHotkeysById.begin();
			it != mHotkeysById.end(); ++it)
		{
			delete it->second;
		}
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::setEnabled(bool _enable)
	{
		if(_enable != mEnabled)
		{
			if(_enable)
			{
				registerKeyListener(getCurrentInputPriority());
				registerControlSettingsListener();
			}
			else
			{
				unregisterKeyListener();
				unregisterControlSettingsListener();
			}
			mEnabled = _enable;
		}
	}
	//-------------------------------------------------------------------------
	bool BaseHotkeyManager::isEnabled() const
	{
		return mEnabled;
	}
	//-------------------------------------------------------------------------
	BaseHotkeyManager::Hotkey* BaseHotkeyManager::findHotkey(const String& _id) const
	{
		HotkeysById::const_iterator it = mHotkeysById.find(_id);
		if(it == mHotkeysById.end())
			return nullptr;
		else
			return it->second;
	}
	//-------------------------------------------------------------------------
	BaseHotkeyManager::Hotkey* BaseHotkeyManager::getHotkey(const String& _id) const
	{
		Hotkey* hotkey = findHotkey(_id);
		GOTHOGRE_ASSERT(hotkey != nullptr, "Hotkey with id = '" << _id << "' not found");
		return hotkey;
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::insertHotkey(const String& _id, const String& _keyInControlSettings, const String& _sectionInControlSettings, const KeyList& _defaultValue)
	{
		if(findHotkey(_id))
			return;

		Hotkey* hotkey = new Hotkey;
		hotkey->tool                     = nullptr;
		hotkey->keyInControlSettings     = _keyInControlSettings;
		hotkey->sectionInControlSettings = _sectionInControlSettings;
		hotkey->defaultKeyList           = _defaultValue;
		hotkey->keyList                  = ControlSettings::getSingleton().getSetting(
											_keyInControlSettings, _sectionInControlSettings, _defaultValue);
		
		mHotkeysById.insert(std::make_pair(_id, hotkey));
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::removeHotkey(const String& _id)
	{
		map<String, Hotkey*>::type::iterator it = mHotkeysById.find(_id);
		if(it != mHotkeysById.end())
		{
			Hotkey* hotkey = it->second;
			mHotkeysById.erase(it);
			delete hotkey;
		}
	}
	//-------------------------------------------------------------------------
	bool BaseHotkeyManager::hasHotkey(const String& _id) const
	{
		return findHotkey(_id) != nullptr;
	}
	//-------------------------------------------------------------------------
	const KeyList& BaseHotkeyManager::getKeyList(const String& _id) const
	{
		Hotkey* hotkey = getHotkey(_id);
		return hotkey->keyList;
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::attachTool(const String& _id, Tool* _tool, const Any& _toolParameter)
	{
		Hotkey* hotkey = getHotkey(_id);
		hotkey->tool = _tool;
		hotkey->toolParameter = _toolParameter;
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::detachTool(const String& _id)
	{
		Hotkey* hotkey = getHotkey(_id);
		hotkey->tool = nullptr;
		hotkey->toolParameter = StringUtil::BLANK;
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::setHotkeyEnabled(const String& _id, bool _enable)
	{
		Hotkey* hotkey = getHotkey(_id);
		hotkey->enabled = _enable;
	}
	//-------------------------------------------------------------------------
	bool BaseHotkeyManager::isHotkeyEnabled(const String& _id) const
	{
		Hotkey* hotkey = getHotkey(_id);
		return hotkey->enabled;
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::controlSettingsChanged( const ConfigSettingsEvent& _evt)
	{
		// Reread key lists from control settings.
		for(HotkeysById::iterator it = mHotkeysById.begin(); 
			it != mHotkeysById.end(); ++it)
		{
			Hotkey* hotkey = it->second;
			hotkey->keyList = ControlSettings::getSingleton().getSetting(
				hotkey->keyInControlSettings, 
				hotkey->sectionInControlSettings, 
				hotkey->defaultKeyList);
		}
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::activateTool(Hotkey* _hotkey)
	{
		if(_hotkey->tool)
		{
			_hotkey->tool->activateTool(_hotkey->toolParameter);
		}
	}
	//-------------------------------------------------------------------------
	KeyEventResult BaseHotkeyManager::keyPressed( const KeyEvent& _evt )
	{
		checkIsLocked();
		if(!mEnabled || isLocked())
			return KeyEventResult::NOT_PROCESSED;

		// a key which has been pressed
		KeyCode kc = _evt.getKeyCode();

		// list of match hotkeys
		// (this list can contain one or several hotkeys,
		// for example if there are two hotkeys: "F3" and "Ctrl+F3",
		// and if keys "Ctrl" and "F3" pressed.)
		vector<HkSz>::type matchHkList;

		// Search match hotkeys and fill list "matchHkList".
		for(HotkeysById::iterator it = mHotkeysById.begin(); 
			it != mHotkeysById.end(); ++it)
		{
			Hotkey* hotkey = it->second;
			if(hotkey->enabled)
			{
				for(size_t i = 0; i != hotkey->keyList.getNumKeyCombs(); ++i)
				{
					const KeyComb& keyComb = hotkey->keyList.getKeyComb(i);
					if(keyComb.findKeyCode(kc) != -1)
					{
						if(InputSystem::getSingleton().isKeyDown(keyComb))
						{
							matchHkList.push_back(HkSz());
							HkSz& hk = matchHkList.back();
							hk.hotkey = hotkey;
							hk.keyCombSize = keyComb.size();
						}
					}
				}
			}
		}

		if(matchHkList.size() != 0)
		{
			// Search a match hotkey which corresponds max-sized a key combination.
			size_t maxSizeIndex = 0;
			for(size_t j = 1; j != matchHkList.size(); ++j)
				if(matchHkList[maxSizeIndex].keyCombSize < matchHkList[j].keyCombSize)
					maxSizeIndex = j;

			// Activate a tool.
			Hotkey* hotkey = matchHkList[maxSizeIndex].hotkey;
			if(hotkey && hotkey->tool)
			{
				ToolOperation* toolOperation = ToolManagerList::getSingleton().getActiveOperation();
				if(!toolOperation || toolOperation->getTool() != hotkey->tool)
				{
					lockUntilKeyRelease(kc);
					activateTool(hotkey);
					return KeyEventResult::PROCESSED;
				}
			}
		}

		// A hot key was not found
		return KeyEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------
	KeyEventResult BaseHotkeyManager::keyReleased( const KeyEvent& _evt )
	{
		checkIsLocked();
		return KeyEventResult::NOT_PROCESSED;
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::lockUntilKeyRelease(KeyCode _keyCode)
	{
		mUnlockKeyCode = _keyCode;
		if(mEnabled)
			registerKeyListener(getCurrentInputPriority());
	}
	//-------------------------------------------------------------------------
	void BaseHotkeyManager::checkIsLocked()
	{
		if(mUnlockKeyCode != KeyCode::NONE)
		{
			if(!InputSystem::getSingleton().isKeyDown(mUnlockKeyCode))
			{
				mUnlockKeyCode = KeyCode::NONE;
				if(mEnabled)
					registerKeyListener(getCurrentInputPriority());
			}			
		}
	}
	//-------------------------------------------------------------------------
	bool BaseHotkeyManager::isLocked() const
	{
		return (mUnlockKeyCode != KeyCode::NONE);
	}
	//-------------------------------------------------------------------------
	int BaseHotkeyManager::getCurrentInputPriority() const
	{
		if(isLocked())
			return InputPriority::HOTKEY_MANAGER_HIGH;
		else
			return InputPriority::HOTKEY_MANAGER;
	}





	//-------------------------------------------------------------------------
	// HotkeyManager
	//-------------------------------------------------------------------------
	HotkeyManager::HotkeyManager(const String& _name)
	{
		HotkeyManagerList::getSingleton().addHotkeyManager(this);
	}
	//-------------------------------------------------------------------------
	HotkeyManager::~HotkeyManager()
	{
		HotkeyManagerList::getSingleton().removeHotkeyManager(this);
	}
	//-------------------------------------------------------------------------
	const String& HotkeyManager::getName() const
	{
		return mName;
	}

} // namespace GUISystem
