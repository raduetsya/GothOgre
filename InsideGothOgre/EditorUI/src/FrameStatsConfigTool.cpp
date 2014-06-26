#include "Precompiled.h"
#include "FrameStatsConfigTool.h"
#include "FrameStatsConfigDialog.h"

namespace GUISystem
{
	//------------------------------------------------------------------------------------
	FrameStatsConfigTool::FrameStatsConfigTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Settings", MenuItemType::Popup, "#{Settings}");

		mb->insertItem("FrameStatsConfig", MenuItemType::Normal, "#{FrameStatsConfig}", "Settings");
		mb->setItemDescription("FrameStatsConfig", "#{FrameStatsConfig_Desc}");
		mb->attachTool("FrameStatsConfig", this);
	}
	//------------------------------------------------------------------------------------
	FrameStatsConfigTool::~FrameStatsConfigTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("FrameStatsConfig");
		mb->removeItem("Settings");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* FrameStatsConfigTool::activate(const Any&)
	{
		return new FrameStatsConfigToolOperation;
	}
	//------------------------------------------------------------------------------------
	FrameStatsConfigToolOperation::FrameStatsConfigToolOperation()
	{
		mDialog = new FrameStatsConfigDialog;
		mDialog->eventEndDialog = MyGUI::newDelegate(this, &FrameStatsConfigToolOperation::notifyEndDialog);
	}
	//------------------------------------------------------------------------------------
	FrameStatsConfigToolOperation::~FrameStatsConfigToolOperation()
	{
		if(mDialog)
			delete mDialog;
	}
	//------------------------------------------------------------------------------------
	void FrameStatsConfigToolOperation::notifyEndDialog()
	{
		deactivateTool();
	}
	//------------------------------------------------------------------------------------
	FrameStatsConfigToolFactory::FrameStatsConfigToolFactory()
		: ToolFactory("FrameStatsConfigTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* FrameStatsConfigToolFactory::createTool()
	{
		return new FrameStatsConfigTool;
	}

} // namespace GUISystem
