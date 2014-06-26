#include "Precompiled.h"
#include "EditorInitTools.h"
#include "EditorToolManager.h"

namespace GUISystem
{
	//-------------------------------------------------------------------------
	EditorInitTools::EditorInitTools()
	{
		mEditorToolManager = new EditorToolManager;
		registerUIModeListener(UIInitPriority::TOOLS);
	}
	//-------------------------------------------------------------------------
	EditorInitTools::~EditorInitTools()
	{
		delete mEditorToolManager;
	}
	//-------------------------------------------------------------------------
	void EditorInitTools::uiModeEntered(const UIModeEvent& _evt)
	{
		if(_evt.getNewUIMode() == UIMode::EDITOR)
			mEditorToolManager->createTools();
	}
	//-------------------------------------------------------------------------
	void EditorInitTools::uiModeExited(const UIModeEvent& _evt)
	{
		if(_evt.getOldUIMode() == UIMode::EDITOR)
			mEditorToolManager->destroyTools();
	}

}
