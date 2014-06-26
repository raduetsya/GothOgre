#include "Precompiled.h"
#include "GraphicSettingsDialog.h"
#include "GraphicSettingsTool.h"

namespace GUISystem
{
	GraphicSettingsTool::GraphicSettingsTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Settings", MenuItemType::Popup, "#{Settings}");

		mb->insertItem("GraphicSettings", MenuItemType::Normal, "#{GraphicSettingsConfig}", "Settings");
		mb->setItemDescription("GraphicSettings", "#{GraphicSettingsConfig_Desc}");
		mb->attachTool("GraphicSettings", this);
	}
	//------------------------------------------------------------------------------------
	GraphicSettingsTool::~GraphicSettingsTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("GraphicSettings");
		mb->removeItem("Settings");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* GraphicSettingsTool::activate(const Any&)
	{
		return new GraphicSettingsToolOperation;
	}
	//------------------------------------------------------------------------------------
	GraphicSettingsToolOperation::GraphicSettingsToolOperation()
	{
		mDialog = new GraphicSettingsDialog;
		mDialog->eventEndDialog = MyGUI::newDelegate(this, &GraphicSettingsToolOperation::notifyEndDialog);
	}
	//------------------------------------------------------------------------------------
	GraphicSettingsToolOperation::~GraphicSettingsToolOperation()
	{
		delete mDialog;
	}
	//------------------------------------------------------------------------------------
	void GraphicSettingsToolOperation::notifyEndDialog()
	{
		deactivateTool();
	}
	//------------------------------------------------------------------------------------
	GraphicSettingsToolFactory::GraphicSettingsToolFactory()
		: ToolFactory("GraphicSettings", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* GraphicSettingsToolFactory::createTool()
	{
		return new GraphicSettingsTool;
	}

} // namespace GUISystem
