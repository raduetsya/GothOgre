#ifndef ENVIRONMENT_TOOL_H
#define ENVIRONMENT_TOOL_H


namespace GUISystem
{
  class EnvironmentDialog;

  // Tool parameter is used: if it is string "<Merge>"
  // then the tool will merge worlds
  class EnvironmentTool : public Tool
  {
  public:
    EnvironmentTool();
    virtual ~EnvironmentTool();

    virtual const String& getName() const;

  protected:
    virtual void showUI();
    virtual void hideUI();
    virtual void start();

  private:
    void notifyEndDialog();

  private:
    EnvironmentDialog* mEnvironmentDialog;
  };
  
} // namespace GUISystem
  
#endif // ENVIRONMENT_TOOL_H
