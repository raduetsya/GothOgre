#include "Precompiled.h"
#include "MainViewportTool.h"
#include "MainViewportWindow.h"

namespace GUISystem
{
	MainViewportTool::MainViewportTool()
	{
		mWindow = new MainViewportWindow;
	}
	//------------------------------------------------------------------------------------
	MainViewportTool::~MainViewportTool()
	{
		delete mWindow;
	}
	//------------------------------------------------------------------------------------
	ToolOperation* MainViewportTool::activate(const String&)
	{
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	MainViewportToolFactory::MainViewportToolFactory()
		: ToolFactory("MainViewportTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* MainViewportToolFactory::createTool()
	{
		return new MainViewportTool;
	}
	
} // namespace GUISystem
