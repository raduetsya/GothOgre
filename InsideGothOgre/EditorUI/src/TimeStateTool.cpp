#include "Precompiled.h"
#include "CommonInclude.h"
#include "TimeStateTool.h"
#include "TimeStatePanel.h"

namespace GUISystem
{
	TimeStateTool::TimeStateTool()
	{
		mTimeStatePanel = nullptr;
	}
	//------------------------------------------------------------------------------------
	const String& TimeStateTool::getName() const
	{
		static const String sName = "TimeState";
		return sName;
	}
	//------------------------------------------------------------------------------------
	void TimeStateTool::showUI()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Sky", MenuItemType::POPUP, "#{Sky}");
		
		mb->insertItem("TimeState", MenuItemType::NORMAL, "#{TimeState}", "Sky");
		mb->setItemDescription("TimeState", "#{TimeState_Desc}");
		mb->attachTool("TimeState", this);
		
		setStateVisible(false);
	}
	//------------------------------------------------------------------------------------
	void TimeStateTool::hideUI()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("TimeState");
		mb->removeItem("Sky");
		showTimeStatePanel(false);
	}
	//------------------------------------------------------------------------------------
	void TimeStateTool::start()
	{
		setStateVisible(!getStateVisible());
		exitTool();
	}
	//------------------------------------------------------------------------------------
	bool TimeStateTool::getStateVisible() const
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		return mb->isItemChecked("TimeState");
	}
	//------------------------------------------------------------------------------------
	void TimeStateTool::setStateVisible(bool _visible)
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemChecked("TimeState", _visible);
		showTimeStatePanel(_visible);
	}
	//------------------------------------------------------------------------------------
	void TimeStateTool::showTimeStatePanel(bool _show)
	{
		if(_show)
		{
			if(!mTimeStatePanel)
			{
				mTimeStatePanel = new TimeStatePanel();
				Root::getSingleton().addFrameListener(this);
			}
		}
		else
		{
			if(mTimeStatePanel)
			{
				delete mTimeStatePanel;
				mTimeStatePanel = nullptr;
				Root::getSingleton().removeFrameListener(this);
			}
		}
	}
	//------------------------------------------------------------------------------------
	bool TimeStateTool::frameEnded(const FrameEvent& evt)
	{
		mTimeStatePanel->update();
		return true;
	}
	
} // namespace GUISystem
