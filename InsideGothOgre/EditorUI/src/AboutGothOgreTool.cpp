#include "Precompiled.h"
#include "AboutGothOgreDialog.h"
#include "AboutGothOgreTool.h"

namespace GUISystem
{
	AboutGothOgreTool::AboutGothOgreTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Help", MenuItemType::Popup, "#{Help}");

		mb->insertItem("AboutGothOgre", MenuItemType::Normal, "#{AboutGothOgre}", "Help");
		mb->setItemDescription("AboutGothOgre", "#{AboutGothOgre_Desc}");
		mb->attachTool("AboutGothOgre", this);
	}
	//------------------------------------------------------------------------------------
	AboutGothOgreTool::~AboutGothOgreTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("AboutGothOgre");
		mb->removeItem("Help");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* AboutGothOgreTool::activate(const Any&)
	{
		return new AboutGothOgreToolOperation;
	}
	//------------------------------------------------------------------------------------
	AboutGothOgreToolOperation::AboutGothOgreToolOperation()
	{
		mDialog = new AboutGothOgreDialog;
		mDialog->eventEndDialog = MyGUI::newDelegate(this, &AboutGothOgreToolOperation::notifyEndDialog);
	}
	//------------------------------------------------------------------------------------
	AboutGothOgreToolOperation::~AboutGothOgreToolOperation()
	{
		delete mDialog;
	}
	//------------------------------------------------------------------------------------
	void AboutGothOgreToolOperation::notifyEndDialog()
	{
		deactivateTool();
	}
	//------------------------------------------------------------------------------------
	AboutGothOgreToolFactory::AboutGothOgreToolFactory()
		: ToolFactory("AboutGothOgreTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* AboutGothOgreToolFactory::createTool()
	{
		return new AboutGothOgreTool;
	}
	
} // namespace GUISystem
