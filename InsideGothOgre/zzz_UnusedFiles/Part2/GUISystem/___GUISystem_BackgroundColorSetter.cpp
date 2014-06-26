#include "GUISystem_Precompiled.h"
#include "GUISystem_BackgroundColorSetter.h"
#include "GUISystem_UIInitPriority.h"
#include "GUISystem_ViewportManager.h"


namespace GUISystem
{
	//-------------------------------------------------------------------------	
	BackgroundColorSetter::BackgroundColorSetter()
	{
		registerUIModeListener( UIInitPriority::AFTER_SKIN );
	}
	//-------------------------------------------------------------------------	
	BackgroundColorSetter::~BackgroundColorSetter()
	{
	}
	//-------------------------------------------------------------------------	
	void BackgroundColorSetter::uiModeEntered(const UIModeEvent& _evt)
	{
		if(MyGUI::LanguageManager::getInstancePtr())
		{
			MyGUI::UString colorAsStr = MyGUI::LanguageManager::getInstance().replaceTags("#{BackgroundViewport_Color}");
			MyGUI::Colour color = MyGUI::Colour::parse(colorAsStr);
			Ogre::ColourValue ogreColor(color.red, color.green, color.blue);
			Viewport* viewport = ViewportManager::getSingleton().getBackgroundViewport();
			viewport->setBackgroundColour(ogreColor);
		}
	}


} // namespace GUISystem