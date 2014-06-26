#include "GUISystem_Precompiled.h"
#include "GUISystem_FrameStatsWindow.h"
#include "GUISystem_UISettings.h"


namespace GUISystem
{

	BaseFrameStatsWindow::BaseFrameStatsWindow()
	{
	}
	//-----------------------------------------------------------------------------
	BaseFrameStatsWindow::~BaseFrameStatsWindow()
	{
	}
	//-----------------------------------------------------------------------------
	void BaseFrameStatsWindow::initialise2(BaseFrameStatsWindow* _owner, MyGUI::Widget* _parent)
	{
		BaseFrameStatsPanel::initialiseByAttributes( _owner, _parent);
	}
	//-----------------------------------------------------------------------------
	void BaseFrameStatsWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
			UISettings::getSingleton().setSetting<bool>( "Visible", "FrameStatsPanel", false );
	}

} // namespace GUISystem
