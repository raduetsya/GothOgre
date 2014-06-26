#ifndef ENVIRONMENT_DIALOG_H
#define ENVIRONMENT_DIALOG_H

namespace GUISystem
{
  class EnvironmentDialog
  {
  public:
    typedef MyGUI::delegates::CDelegate0 EventHandle_Result;

  public:
    EnvironmentDialog();
    ~EnvironmentDialog();

    // Run the dialog
    void showModal();
    
    // event for pressed load/save button
    EventHandle_Result eventEndDialog;

  private:
    void createMyGUICtrl();
    void destroyMyGUICtrl();

    void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
    void notifyMouseButtonClick(MyGUI::Widget* _sender);
    void notifyEditTextChange(MyGUI::EditBox* _sender);
    void SetFog();

  private:
    String            mPrefix;
    MyGUI::WindowPtr  mMain;
    MyGUI::Button*    mCheckFog;
    MyGUI::EditBox*      mStartFog;
    MyGUI::EditBox*      mEndFog;
    MyGUI::EditBox*      mEditRed;
    MyGUI::EditBox*      mEditGreen;
    MyGUI::EditBox*      mEditBlue;
    ColourValue       mFogColour;
    float             mStart;
    float             mEnd;
  };

} // namespace GUISystem

#endif // ENVIRONMENT_DIALOG_H