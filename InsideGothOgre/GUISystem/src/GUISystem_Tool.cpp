#include "GUISystem_Precompiled.h"
#include "GUISystem_Tool.h"
#include "GUISystem_ToolManager.h"
#include "GUISystem_ToolManagerList.h"
#include "GUISystem_ToolOperation.h"
#include "GUISystem_ToolFactory.h"

namespace GUISystem
{
	//-----------------------------------------------------------------
	Tool::Tool()
	{
		mToolFactory = nullptr;
		mToolManager = nullptr;
	}
	//-----------------------------------------------------------------
	void Tool::init(ToolFactory* _toolFactory, ToolManager* _toolManager)
	{
		mToolFactory = _toolFactory;
		mToolManager = _toolManager;
	}
	//-----------------------------------------------------------------
	Tool::~Tool()
	{
	}
	//-----------------------------------------------------------------
	const String& Tool::getName() const
	{
		return getToolFactory()->getName();
	}
	//-----------------------------------------------------------------
	ToolOperation* Tool::activateTool(const Any& _toolParameter) 
	{
		if(mToolManager)
		{
			return mToolManager->activateTool(this, _toolParameter);
		}
		else
		{
			// tool is not initialised yet, will be activated later
			ToolManagerList::getSingleton().setDelayedActTool(this, _toolParameter);
			return nullptr;
		}
	}
	//-----------------------------------------------------------------
	bool Tool::isActive() const
	{
		if(!mToolManager)
			return false;

		ToolOperation* toolOp = mToolManager->getActiveOperation();
		return (toolOp && toolOp->getTool() == this);
	}

}