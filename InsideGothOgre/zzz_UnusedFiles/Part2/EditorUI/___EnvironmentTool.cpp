#include "Precompiled.h"
#include "CommonInclude.h"
#include "EnvironmentDialog.h"
#include "EnvironmentTool.h"

namespace GUISystem
{
  EnvironmentTool::EnvironmentTool()
  {
    mEnvironmentDialog = nullptr;
  }
  //------------------------------------------------------------------------------------
  EnvironmentTool::~EnvironmentTool()
  {
    if(mEnvironmentDialog)
      delete mEnvironmentDialog;
  }
  //------------------------------------------------------------------------------------
  const String& EnvironmentTool::getName() const
  {
    static const String sName = "EnvironmentParams";
    return sName;
  }
  //------------------------------------------------------------------------------------
  void EnvironmentTool::showUI()
  {
    MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
    mb->insertItem("Sky", MenuItemType::POPUP, "#{Sky}");

    mb->insertItem("EnvironmentParams", MenuItemType::NORMAL, "#{Environment}", "Sky");
    mb->setItemDescription("EnvironmentParams", "#{Environment_Desc}");
    mb->attachTool("EnvironmentParams", this);
  }
  //------------------------------------------------------------------------------------
  void EnvironmentTool::hideUI()
  {
    MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
    mb->removeItem("EnvironmentParams");
    mb->removeItem("Sky");
  }
  //------------------------------------------------------------------------------------
  void EnvironmentTool::start()
  {
    if(!mEnvironmentDialog)
      mEnvironmentDialog = new EnvironmentDialog;

    mEnvironmentDialog->eventEndDialog = MyGUI::newDelegate(this, &EnvironmentTool::notifyEndDialog);
    mEnvironmentDialog->showModal();
  }
  //------------------------------------------------------------------------------------
  void EnvironmentTool::notifyEndDialog()
  {
    exitTool();
  }
  
} // namespace GUISystem
