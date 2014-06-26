#include "GUISystem_Precompiled.h"
#include "GUISystem_ToolManager.h"
#include "GUISystem_ToolOperation.h"
#include "GUISystem_Tool.h"

namespace GUISystem
{
	//-----------------------------------------------------------------------
	ToolOperation::ToolOperation()
	{
		mTool = nullptr;
		mDeactivated = false;
	}
	//-----------------------------------------------------------------------
	void ToolOperation::init(Tool* _tool)
	{
		mTool = _tool;
	}
	//-----------------------------------------------------------------------
	ToolOperation::~ToolOperation()
	{
	}
	//-----------------------------------------------------------------------
	void ToolOperation::deactivateTool()
	{
		mDeactivated = true;
		if(mTool)
			mTool->getToolManager()->deactivateTool(this);
	}

}