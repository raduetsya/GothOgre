#include "Precompiled.h"
#include "EditorToolManager.h"


//------------------------------------------------------------------------------------
GUISystem::EditorToolManager* 
	Ogre::Singleton<GUISystem::EditorToolManager>::ms_Singleton 
	= nullptr;
//------------------------------------------------------------------------------------


namespace GUISystem
{
	EditorToolManager::EditorToolManager()
		: ToolManager("EditorToolManager")
	{
	}

} // namespace GUISystem