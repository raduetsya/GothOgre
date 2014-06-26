#include "Precompiled.h"
#include "EditorInitSkin.h"

namespace GUISystem
{
	//-------------------------------------------------------------------------
	EditorInitSkin::EditorInitSkin()
	{
		registerUIModeListener( UIInitPriority::SKIN );
	}
	//-------------------------------------------------------------------------
	void EditorInitSkin::uiModeEntered(const UIModeEvent& _evt)
	{
		if(_evt.getNewUIMode() == UIMode::EDITOR)
		{
			MyGUI::ResourceManager::getInstance().load("EditorUI.xml");
		}
	}

}
