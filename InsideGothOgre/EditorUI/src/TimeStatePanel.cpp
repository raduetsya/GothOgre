#include "Precompiled.h"
#include "CommonInclude.h"
#include "TimeStatePanel.h"

namespace GUISystem
{

	TimeStatePanel::TimeStatePanel() 
		: mInfo(nullptr)
	{
		const String layer = "Statistic";
		if ( ! MyGUI::LayerManager::getInstance().isExist(layer)) return;

		const String skin = "StatisticRect";
		if ( ! MyGUI::SkinManager::getInstance().isExist(skin)) return;

//		const MyGUI::IntSize& size = text->getTextSize() + mInfo->getSize() - text->getSize();
		const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
				
//		mInfo->setCoord(20, size_view.height - size.height - 20, size.width, size.height);
//		mInfo = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>(skin, MyGUI::IntCoord(5, 30, 60, 15), MyGUI::Align::Default, layer);
		mInfo = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>(skin, MyGUI::IntCoord(20, size_view.height-18, 60, 15), MyGUI::Align::Default, layer);
	}
	//-------------------------------------------------------------------------------------
	TimeStatePanel::~TimeStatePanel()
	{
		if (mInfo)
		{
			MyGUI::Gui::getInstance().destroyChildWidget(mInfo);
			mInfo = nullptr;
		}
	}
	//-------------------------------------------------------------------------------------
	void TimeStatePanel::update()
	{
		mInfo->setCaption("Time: "+ MyGUI::utility::toString(GameTime::getSingleton().getTimeHour())+
			":"+MyGUI::utility::toString(GameTime::getSingleton().getTimeMin()));
	}
} // namespace GUISystem
