#ifndef GUISYSTEM_FRAME_STATS_WINDOW_MANAGER_H
#define GUISYSTEM_FRAME_STATS_WINDOW_MANAGER_H

#include "GUISystem_UISettings.h"


namespace GUISystem
{
	class FrameStatsPanel;
	class FrameStatsWindow;


	//-----------------------------------------------------------------------
	/** This class manages frame stats panel.
	It can show and hide FrameStatsPanel and FrameStatsWindow
	according to settings in the configuration file. */
	class GUISYSTEM_EXPORT FrameStatsWindowManager : public Singleton<FrameStatsWindowManager>
	{
	public:
		FrameStatsWindowManager();
		~FrameStatsWindowManager();

		void setVisible(bool _visible);
		bool isVisible() const;

		void setMovable(bool _movable);
		bool isMovable() const;

	private:
		GOTHOGRE_DECLARE_UI_MODE_ENTERED_EXITED( FrameStatsWindowManager );
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED( FrameStatsWindowManager );

		void createWidgets();
		void destroyWidgets();

	private:
		bool              mVisible;
		bool              mMovable;
		bool              mUIModeEntered;
		FrameStatsPanel*  mPanel;
		FrameStatsWindow* mWindow;
	};

	
} // namespace GUISystem
	
#endif // GUISYSTEM_FRAME_STATS_WINDOW_MANAGER_H