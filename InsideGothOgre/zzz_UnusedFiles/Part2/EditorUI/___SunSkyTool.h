#ifndef SUNSKY_TOOL_H
#define SUNSKY_TOOL_H


namespace GUISystem
{
  class SunSkyDialog;

  // Tool parameter is used: if it is string "<Merge>"
  // then the tool will merge worlds
  class SunSkyTool : public Tool
  {
  public:
    SunSkyTool();
    virtual ~SunSkyTool();

    virtual const String& getName() const;

  protected:
    virtual void showUI();
    virtual void hideUI();
    virtual void start();

  private:
    void notifyEndDialog();

  private:
    SunSkyDialog* mSunSkyDialog;
  };
  
} // namespace GUISystem
  
#endif // SUNSKY_TOOL_H
