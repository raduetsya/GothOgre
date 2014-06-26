#include "GUISystem_Precompiled.h"
#include "GUISystem_FrameStatsWindow.h"
#include "GUISystem_UISettings.h"


namespace GUISystem
{
	//-----------------------------------------------------------------------------
	// BaseFrameStatsWindow
	//-----------------------------------------------------------------------------
	BaseFrameStatsWindow::BaseFrameStatsWindow()
	{
	}
	//-----------------------------------------------------------------------------
	void BaseFrameStatsWindow::initialise2(BaseFrameStatsWindow* _owner, MyGUI::Widget* _parent)
	{
		BaseLayout::initialiseByAttributes( _owner, _parent);
		getMainWidget()->setAlign(MyGUI::Align::Default);
		registerFrameListener();
	}
	//-----------------------------------------------------------------------------
	BaseFrameStatsWindow::~BaseFrameStatsWindow()
	{
	}
	//-----------------------------------------------------------------------------
	void BaseFrameStatsWindow::frameStarted(const FrameEvent& _evt)
	{
		String textTemplate = UISettings::getSingleton().getSetting<String>( "TextTemplate", "FrameStatsPanel", FrameStats::getSingleton().getDefaultTextTemplate() );
		String text = FrameStats::getSingleton().getText( textTemplate );
		mStaticText->setCaption(text);
	}
	//-----------------------------------------------------------------------------
	void BaseFrameStatsWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
			UISettings::getSingleton().setSetting<bool>( "Visible", "FrameStatsPanel", false );
	}

} // namespace GUISystem
