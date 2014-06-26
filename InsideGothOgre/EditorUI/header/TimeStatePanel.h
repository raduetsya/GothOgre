#ifndef TIME_STATE_PANEL_H
#define TIME_STATE_PANEL_H

namespace GUISystem
{
	class TimeStatePanel
	{
	public:
		TimeStatePanel();
		~TimeStatePanel();
		void update();

	private:
		MyGUI::StaticTextPtr mInfo;
	};

} // namespace GUISystem

#endif // TIME_STATE_PANEL_H
