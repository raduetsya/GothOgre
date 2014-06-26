#include "GUISystem_Precompiled.h"
#include "GUISystem_MenuBar.h"
#include "GUISystem_MenuBarList.h"

//-------------------------------------------------------------------------
GUISystem::MenuBarList* 
	Ogre::Singleton<GUISystem::MenuBarList>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GUISystem
{
	MenuBarList::MenuBarList()
	{
	}
	//-------------------------------------------------------------------------
	MenuBarList::~MenuBarList()
	{
	}
	//-------------------------------------------------------------------------
	void MenuBarList::addMenuBar(MenuBar* _menuBar)
	{
		const String& name = _menuBar->getName();
		mMenuBarsByName.insert(std::make_pair(name, _menuBar));
	}
	//-------------------------------------------------------------------------
	void MenuBarList::removeMenuBar(MenuBar* _menuBar)
	{
		const String& name = _menuBar->getName();
		MenuBarsByName::iterator it = mMenuBarsByName.find(name);
		if(it != mMenuBarsByName.end())
			mMenuBarsByName.erase(it);
	}
	//-------------------------------------------------------------------------
	MenuBar* MenuBarList::getMenuBar(const String& _name) const
	{
		MenuBarsByName::const_iterator it = mMenuBarsByName.find(_name);
		if(it == mMenuBarsByName.end())
		{
			GOTHOGRE_EXCEPT("Menu bar '" << _name << "' not found."); 
		}
		return it->second;
	}

} // namespace GUISystem
