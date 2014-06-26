#include "GUISystem_Precompiled.h"
#include "GUISystem_Tool.h"
#include "GUISystem_ToolFactory.h"
#include "GUISystem_ToolManager.h"
#include "GUISystem_ToolManagerList.h"

namespace GUISystem
{
	ToolFactory::ToolFactory(const String& _toolName, const String& _toolManagerName)
	{
		mName = _toolName;
		mToolManager = ToolManagerList::getSingleton().getToolManager(_toolManagerName);
		mToolManager->addToolFactory(this);
	}
	//----------------------------------------------------------------
	ToolFactory::~ToolFactory()
	{
		mToolManager->removeToolFactory(this);
	}
	//----------------------------------------------------------------
	void ToolFactory::destroyTool(Tool* _tool)
	{
		delete _tool;
	}
}