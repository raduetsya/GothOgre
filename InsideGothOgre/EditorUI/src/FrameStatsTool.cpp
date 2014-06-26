#include "Precompiled.h"
#include "FrameStatsTool.h"

namespace GUISystem
{

	FrameStatsTool::FrameStatsTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Windows", MenuItemType::Popup, "#{Windows}");

		mb->insertItem("FrameStats", MenuItemType::Normal, "", "Windows");
		mb->setItemDescription("FrameStats", "#{FrameStats_Desc}");
		mb->attachTool("FrameStats", this);

		registerUISettingsListener();
	}
	//------------------------------------------------------------------------------------
	FrameStatsTool::~FrameStatsTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("ShowFrameStats");
		mb->removeItem("Windows");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* FrameStatsTool::activate(const Any& _parameter)
	{
		bool value = FrameStatsWindowManager::getSingleton().isVisible();
		value = !value;
		FrameStatsWindowManager::getSingleton().setVisible(value);
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	void FrameStatsTool::uiSettingsChanged( const ConfigSettingsEvent& _evt)
	{
		bool value = FrameStatsWindowManager::getSingleton().isVisible();
		String caption;
		if(value)
			caption = "#{FrameStats_Shown}";
		else
			caption = "#{FrameStats_Hidden}";

		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemCaption("FrameStats", caption);
	}
	//------------------------------------------------------------------------------------
	FrameStatsToolFactory::FrameStatsToolFactory()
		: ToolFactory("FrameStatsTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* FrameStatsToolFactory::createTool()
	{
		return new FrameStatsTool;
	}

} // namespace GUISystem
