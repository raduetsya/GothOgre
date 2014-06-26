#include "Precompiled.h"
#include "RenderModeTool.h"

namespace GUISystem
{
	RenderModeTool::RenderModeTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Settings", MenuItemType::Popup, "#{Settings}");
		mb->insertItem("RenderMode", MenuItemType::Normal, "", "Settings");
		mb->setItemDescription("RenderMode", "#{RenderMode_Desc}");
		mb->insertHotkey("RenderMode", "RenderMode", "Editor", KeyList(KeyCode::R));
		mb->attachTool("RenderMode", this);
		
		registerGraphicSettingsListener();
	}
	//------------------------------------------------------------------------------------
	RenderModeTool::~RenderModeTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("RenderMode");
		mb->removeItem("Settings");
	}
	//------------------------------------------------------------------------------------
	void RenderModeTool::graphicSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		PolygonMode pm = GraphicSettings::getSingleton().getSetting("PolygonMode", "", PM_SOLID);
		String caption;
		switch(pm)
		{
			case PM_SOLID:     caption = "#{RenderMode_Solid}"; break;
			case PM_WIREFRAME: caption = "#{RenderMode_Wireframe}"; break;
			case PM_POINTS:    caption = "#{RenderMode_Points}"; break;
		}
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemCaption("RenderMode", caption);
	}
	//------------------------------------------------------------------------------------
	ToolOperation* RenderModeTool::activate(const Any&)
	{
		PolygonMode pm = GraphicSettings::getSingleton().getSetting("PolygonMode", "", PM_SOLID);
		switch(pm)
		{
			case PM_SOLID:     pm = PM_WIREFRAME; break;
			case PM_WIREFRAME: pm = PM_POINTS; break;
			case PM_POINTS:    pm = PM_SOLID; break;
		}
		GraphicSettings::getSingleton().setSetting("PolygonMode", "", pm);
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	RenderModeToolFactory::RenderModeToolFactory()
		: ToolFactory("RenderModeTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* RenderModeToolFactory::createTool()
	{
		return new RenderModeTool;
	}
	
} // namespace GUISystem
