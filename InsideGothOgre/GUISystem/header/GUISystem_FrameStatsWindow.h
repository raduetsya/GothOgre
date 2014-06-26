#ifndef GUISYSTEM_FRAME_STATS_WINDOW_H
#define GUISYSTEM_FRAME_STATS_WINDOW_H

#include "GUISystem_BaseLayout.h"


namespace GUISystem
{
	//-------------------------------------------------------------------------
	// BaseFrameStatsWindow
	//-------------------------------------------------------------------------
	// Class attributes
	ATTRIBUTE_CLASS_LAYOUT        ( BaseFrameStatsWindow, "#{FrameStatsPanel_Layout}");
	ATTRIBUTE_CLASS_CONFIG_SECTION( BaseFrameStatsWindow, "FrameStatsPanel");

	/** Base class for the "FrameStatsPanel" and "BaseFrameStatsWindow" classes. */
	class BaseFrameStatsWindow : public BaseLayout
	{
	public:
		BaseFrameStatsWindow();
		~BaseFrameStatsWindow();

	protected:
		template<typename T>
		void initialiseByAttributes(T* _owner, MyGUI::Widget* _parent = nullptr);

	private:
		void initialise2(BaseFrameStatsWindow* _owner, MyGUI::Widget* _parent);
		GOTHOGRE_DECLARE_FRAME_STARTED( BaseFrameStatsWindow );
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFrameStatsWindow, mStaticText, "staticText");
		MyGUI::TextBox*         mStaticText;
	};
	//--------------------------------------------------------------------------------------
	template <typename T>
	void BaseFrameStatsWindow::initialiseByAttributes(T* _owner, MyGUI::Widget* _parent)
	{
		applyClassAttributes(_owner);
		initialise2(_owner, _parent);
	}




	//-------------------------------------------------------------------------
	// FrameStatsPanel
	//-------------------------------------------------------------------------
	// Class attributes
	ATTRIBUTE_CLASS_MAIN_WIDGET   ( FrameStatsPanel, "_Panel");

	/** A panel which can show the frame statistics. */
	class FrameStatsPanel : public BaseFrameStatsWindow
	{
	public:
		FrameStatsPanel()
		{
			initialiseByAttributes(this);
		}
	};



	//--------------------------------------------------------------------------------------
	// FrameStatsWindow
	//--------------------------------------------------------------------------------------
	// Class attributes
	ATTRIBUTE_CLASS_MAIN_WIDGET   ( FrameStatsWindow, "_Window");

	/** A window which can show the frame statistics. */
	class FrameStatsWindow : public BaseFrameStatsWindow
	{
	public:
		FrameStatsWindow()
		{
			initialiseByAttributes(this);
		}
	};

} // namespace GUISystem

#endif // GUISYSTEM_FRAME_STATS_WINDOW_H
