#include "Precompiled.h"
#include "QuitTool.h"

namespace GUISystem
{
	//------------------------------------------------------------------------------------
	QuitTool::QuitTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("File", MenuItemType::Popup, "#{File}");
		mb->insertItem("QuitSep", MenuItemType::Separator, "", "File");
		mb->insertItem("Quit", MenuItemType::Normal, "#{Quit}", "File");
		mb->setItemDescription("Quit", "#{Quit_Desc}");
		mb->attachTool("Quit", this);
	}
	//------------------------------------------------------------------------------------
	QuitTool::~QuitTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("Quit");
		mb->removeItem("QuitSep");
		mb->removeItem("File");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* QuitTool::activate(const Any&)
	{
		InputSystem::getSingleton().setExitFlag();
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	QuitToolFactory::QuitToolFactory()
		: ToolFactory("QuitTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* QuitToolFactory::createTool()
	{
		return new QuitTool;
	}
		
} // namespace GUISystem