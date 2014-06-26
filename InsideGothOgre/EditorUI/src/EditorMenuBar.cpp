#include "Precompiled.h"
#include "EditorMenuBar.h"


//------------------------------------------------------------------------------------
GUISystem::EditorMenuBar* 
	Ogre::Singleton<GUISystem::EditorMenuBar>::ms_Singleton 
	= nullptr;
//------------------------------------------------------------------------------------


namespace GUISystem
{
	EditorMenuBar::EditorMenuBar()
		: MenuBar("EditorMenuBar")
	{
	}

} // namespace GUISystem