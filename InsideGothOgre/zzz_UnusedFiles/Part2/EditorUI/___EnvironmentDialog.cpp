#include "Precompiled.h"
#include "CommonInclude.h"
#include "EnvironmentDialog.h"

namespace GUISystem
{
  EnvironmentDialog::EnvironmentDialog()
  {
    mMain = nullptr;
    mCheckFog = nullptr;
    mEditRed = nullptr;
    mEditGreen = nullptr;
    mEditBlue = nullptr;
    mStartFog = nullptr;
    mEndFog = nullptr;
  }
  //--------------------------------------------------------------------------
  EnvironmentDialog::~EnvironmentDialog()
  {
    destroyMyGUICtrl();
  }
  //--------------------------------------------------------------------------
  void EnvironmentDialog::showModal()
  {
    createMyGUICtrl();
  }
  //--------------------------------------------------------------------------
  void EnvironmentDialog::createMyGUICtrl()
  {
    if(!mMain)
    {
      mPrefix = MyGUI::utility::toString(this, "_");
      MyGUI::LayoutManager& lm = MyGUI::LayoutManager::getInstance();
      mMain = lm.loadLayout("EnvironmentDialog.layout", mPrefix).at(0)->castType<MyGUI::Window>();

      mMain->setVisible(true);
      MyGUI::InputManager::getInstance().addWidgetModal(mMain);
      mMain->eventWindowButtonPressed = MyGUI::newDelegate(this, &EnvironmentDialog::notifyWindowButtonPressed);

      mCheckFog = mMain->findWidget(mPrefix + "check_Fog")->castType<MyGUI::Button>();
      mCheckFog->eventMouseButtonClick = MyGUI::newDelegate(this, &EnvironmentDialog::notifyMouseButtonClick);

      mEditRed = mMain->findWidget(mPrefix + "edit_Red")->castType<MyGUI::EditBox>();
      mEditGreen = mMain->findWidget(mPrefix + "edit_Green")->castType<MyGUI::EditBox>();
      mEditBlue = mMain->findWidget(mPrefix + "edit_Blue")->castType<MyGUI::EditBox>();
      mStartFog = mMain->findWidget(mPrefix + "start_Fog")->castType<MyGUI::EditBox>();
      mEndFog = mMain->findWidget(mPrefix + "end_Fog")->castType<MyGUI::EditBox>();

      mFogColour = ColourValue(
        MyGUI::utility::parseFloat(mEditRed->getCaption()) / 255.0f,
        MyGUI::utility::parseFloat(mEditGreen->getCaption()) / 255.0f,
        MyGUI::utility::parseFloat(mEditBlue->getCaption()) / 255.0f);

      mStart = MyGUI::utility::parseFloat(mStartFog->getCaption());
      mEnd = MyGUI::utility::parseFloat(mEndFog->getCaption());

      SetFog();
      
      mEditRed->eventEditTextChange = MyGUI::newDelegate(this, &EnvironmentDialog::notifyEditTextChange);
      mEditGreen->eventEditTextChange = MyGUI::newDelegate(this, &EnvironmentDialog::notifyEditTextChange);
      mEditBlue->eventEditTextChange = MyGUI::newDelegate(this, &EnvironmentDialog::notifyEditTextChange);
      mStartFog->eventEditTextChange = MyGUI::newDelegate(this, &EnvironmentDialog::notifyEditTextChange);
      mEndFog->eventEditTextChange = MyGUI::newDelegate(this, &EnvironmentDialog::notifyEditTextChange);
    }
  }
  //--------------------------------------------------------------------------
  void EnvironmentDialog::destroyMyGUICtrl()
  {
    if(MyGUI::InputManager::getInstancePtr())
      MyGUI::InputManager::getInstance().removeWidgetModal(mMain);

    if(MyGUI::Gui::getInstancePtr())
      MyGUI::Gui::getInstance().destroyWidget(mMain);

    mCheckFog = nullptr;
    mMain = nullptr;
    mEditRed = nullptr;
    mEditGreen = nullptr;
    mEditBlue = nullptr;
    mStartFog = nullptr;
    mEndFog = nullptr;
  }
  //--------------------------------------------------------------------------
  void EnvironmentDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
  {
    if (_name == "close")
    {
      destroyMyGUICtrl();
      eventEndDialog();
    }
  }
  //--------------------------------------------------------------------------
  void EnvironmentDialog::SetFog()
  {
      if (mCheckFog->getButtonPressed())
      {
        World::getSingleton().getSceneManager()->setFog(FOG_LINEAR, mFogColour, 0, mStart, mEnd);
      }
      else
      {
        World::getSingleton().getSceneManager()->setFog(FOG_NONE);
      }

  }
  //--------------------------------------------------------------------------
  void EnvironmentDialog::notifyMouseButtonClick(MyGUI::Widget* _sender)
  {
    if (_sender == mCheckFog)
    {
      mCheckFog->setButtonPressed( ! mCheckFog->getButtonPressed());
      SetFog();
    }
  }
  //--------------------------------------------------------------------------
  void EnvironmentDialog::notifyEditTextChange(MyGUI::EditBox* _sender)
  {
    if (_sender == mStartFog)
    {
      mStart = MyGUI::utility::parseFloat(mStartFog->getCaption());
      SetFog();
    }
    else
    if (_sender == mEndFog)
    {
      mEnd = MyGUI::utility::parseFloat(mEndFog->getCaption());
      SetFog();
    }
    else
    {
      MyGUI::EditBox* edit = static_cast<MyGUI::EditBox*>(_sender);
      size_t cursor = edit->getTextCursor();
      size_t num = MyGUI::utility::parseSizeT(edit->getCaption());
      if (num > 255) num = 255;
      edit->setCaption(MyGUI::utility::toString(num));
      if (cursor < edit->getTextLength()) edit->setTextCursor(cursor);

      mFogColour = ColourValue(
        MyGUI::utility::parseFloat(mEditRed->getCaption()) / 255.0f,
        MyGUI::utility::parseFloat(mEditGreen->getCaption()) / 255.0f,
        MyGUI::utility::parseFloat(mEditBlue->getCaption()) / 255.0f);

      SetFog();
    } 
  }
} // namespace GUISystem
