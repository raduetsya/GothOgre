#include "GUISystem_Precompiled.h"
#include "GUISystem_HotkeyManager.h"
#include "GUISystem_HotkeyManagerList.h"

//-------------------------------------------------------------------------
GUISystem::HotkeyManagerList* 
	Ogre::Singleton<GUISystem::HotkeyManagerList>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GUISystem
{
	HotkeyManagerList::HotkeyManagerList()
	{
	}
	//-------------------------------------------------------------------------
	HotkeyManagerList::~HotkeyManagerList()
	{
	}
	//-------------------------------------------------------------------------
	void HotkeyManagerList::addHotkeyManager(HotkeyManager* _hkManager)
	{
		const String& name = _hkManager->getName();
		mHotkeyManagersByName.insert(std::make_pair(name, _hkManager));
	}
	//-------------------------------------------------------------------------
	void HotkeyManagerList::removeHotkeyManager(HotkeyManager* _hkManager)
	{
		const String& name = _hkManager->getName();
		HotkeyManagersByName::iterator it = mHotkeyManagersByName.find(name);
		if(it != mHotkeyManagersByName.end())
			mHotkeyManagersByName.erase(it);
	}
	//-------------------------------------------------------------------------
	HotkeyManager* HotkeyManagerList::getHotkeyManager(const String& _name) const
	{
		HotkeyManagersByName::const_iterator it = mHotkeyManagersByName.find(_name);
		if(it == mHotkeyManagersByName.end())
		{
			GOTHOGRE_EXCEPT("Hotkey manager '" << _name << "' not found."); 
		}
		return it->second;
	}

} // namespace GUISystem
