#ifndef GUISYSTEM_INIT_COMMON_WIDGETS_H
#define GUISYSTEM_INIT_COMMON_WIDGETS_H

namespace GUISystem
{
	class FrameStatsManager;
	class ProgressDialogManager;

	class InitCommonWidgets : public AutoUIModeListener<UIInitOrder::COMMON_WIDGETS>
	{
	public:
		InitCommonWidgets();
		~InitCommonWidgets();

	private:
		void enterUIMode(const UIModeEvent& _evt);
		void exitUIMode(const UIModeEvent& _evt);
		
		void createCommonWidgets();
		void destroyCommonWidgets();

	private:
		FrameStatsManager*    mFrameStatsManager;
		ProgressDialogManager* mProgressDialogManager;
	};

}

#endif // EDITOR_INIT_COMMON_WIDGETS_H