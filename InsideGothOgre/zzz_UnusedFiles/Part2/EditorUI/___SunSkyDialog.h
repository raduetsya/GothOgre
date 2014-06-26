#ifndef SUNSKY_DIALOG_H
#define SUNSKY_DIALOG_H

namespace GUISystem
{
  class SunSkyDialog
  {
  public:
    typedef MyGUI::delegates::CDelegate0 EventHandle_Result;

  public:
    SunSkyDialog();
    ~SunSkyDialog();

    // Run the dialog
    void showModal();
    
    // event for pressed load/save button
    EventHandle_Result eventEndDialog;

  private:
    void createMyGUICtrl();
    void destroyMyGUICtrl();

    void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);

  private:
    String        mPrefix;
    MyGUI::WindowPtr  mMain;
  };

} // namespace GUISystem

#endif // SUNSKY_DIALOG_H