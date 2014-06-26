#include "Precompiled.h"
#include "CommonInclude.h"
#include "SunSkyDialog.h"
#include "SunSkyTool.h"

namespace GUISystem
{
  SunSkyTool::SunSkyTool()
  {
    mSunSkyDialog = nullptr;
  }
  //------------------------------------------------------------------------------------
  SunSkyTool::~SunSkyTool()
  {
    if(mSunSkyDialog)
      delete mSunSkyDialog;
  }
  //------------------------------------------------------------------------------------
  const String& SunSkyTool::getName() const
  {
    static const String sName = "SunSkyParams";
    return sName;
  }
  //------------------------------------------------------------------------------------
  void SunSkyTool::showUI()
  {
    MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
    mb->insertItem("Sky", MenuItemType::POPUP, "#{Sky}");

    mb->insertItem("SunSkyParams", MenuItemType::NORMAL, "#{SunSky}", "Sky");
    mb->setItemDescription("SunSkyParams", "#{SunSky_Desc}");
    mb->attachTool("SunSkyParams", this);
  }
  //------------------------------------------------------------------------------------
  void SunSkyTool::hideUI()
  {
    MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
    mb->removeItem("SunSkyParams");
    mb->removeItem("Sky");
  }
  //------------------------------------------------------------------------------------
  void SunSkyTool::start()
  {
    if(!mSunSkyDialog)
      mSunSkyDialog = new SunSkyDialog;

    mSunSkyDialog->eventEndDialog = MyGUI::newDelegate(this, &SunSkyTool::notifyEndDialog);
    mSunSkyDialog->showModal();
  }
  //------------------------------------------------------------------------------------
  void SunSkyTool::notifyEndDialog()
  {
    exitTool();
  }
  
} // namespace GUISystem
