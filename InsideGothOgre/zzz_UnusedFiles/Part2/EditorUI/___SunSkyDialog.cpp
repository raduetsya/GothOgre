#include "Precompiled.h"
#include "CommonInclude.h"
#include "SunSkyDialog.h"

namespace GUISystem
{
  SunSkyDialog::SunSkyDialog()
  {
    mMain = nullptr;
  }
  //--------------------------------------------------------------------------
  SunSkyDialog::~SunSkyDialog()
  {
    destroyMyGUICtrl();
  }
  //--------------------------------------------------------------------------
  void SunSkyDialog::showModal()
  {
    createMyGUICtrl();
  }
  //--------------------------------------------------------------------------
  void SunSkyDialog::createMyGUICtrl()
  {
    if(!mMain)
    {
      mPrefix = MyGUI::utility::toString(this, "_");
      MyGUI::LayoutManager& lm = MyGUI::LayoutManager::getInstance();
      mMain = lm.loadLayout("SunSkyDialog.layout", mPrefix).at(0)->castType<MyGUI::Window>();

      mMain->setVisible(true);
      MyGUI::InputManager::getInstance().addWidgetModal(mMain);
      mMain->eventWindowButtonPressed = MyGUI::newDelegate(this, &SunSkyDialog::notifyWindowButtonPressed);
    }
  }
  //--------------------------------------------------------------------------
  void SunSkyDialog::destroyMyGUICtrl()
  {
    if(MyGUI::InputManager::getInstancePtr())
      MyGUI::InputManager::getInstance().removeWidgetModal(mMain);

    if(MyGUI::Gui::getInstancePtr())
      MyGUI::Gui::getInstance().destroyWidget(mMain);

    mMain = nullptr;
  }
  //--------------------------------------------------------------------------
  void SunSkyDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
  {
    if (_name == "close")
    {
      destroyMyGUICtrl();
      eventEndDialog();
    }
  }

} // namespace GUISystem
