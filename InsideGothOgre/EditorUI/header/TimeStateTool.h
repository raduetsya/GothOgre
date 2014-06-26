#ifndef TIME_STATE_TOOL_H
#define TIME_STATE_TOOL_H

namespace GUISystem
{
	class TimeStatePanel;

	class TimeStateTool : public Tool, public AutoFrameListener
	{
	public:
		TimeStateTool();
		virtual const String& getName() const;

		bool getStateVisible() const;
		void setStateVisible(bool _visible);

	protected:
		virtual void showUI();
		virtual void hideUI();
		virtual void start();
		virtual bool frameEnded(const FrameEvent& evt);

	private:
		void showTimeStatePanel(bool _show);
		TimeStatePanel* mTimeStatePanel;
	};
	
} // namespace GUISystem
	
#endif // TIME_STATE_TOOL_H