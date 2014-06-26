#ifndef WORLD_OBJECT_PROPERTIES_WINDOW_H
#define WORLD_OBJECT_PROPERTIES_WINDOW_H

#include "ZenObjectPropertiesWindow.h"

namespace GUISystem
{
	ATTRIBUTE_CLASS_CONFIG_SECTION( WorldObjectPropertiesWindow, "WorldObjectProperties");

	class WorldObjectPropertiesWindow : public ZenObjectPropertiesWindow
	{
	public:
		WorldObjectPropertiesWindow();
		~WorldObjectPropertiesWindow();
		
	private:
		GOTHOGRE_DECLARE_WORLD_OBJECT_CHANGED_SELECT_CHANGED (WorldObjectPropertiesWindow );
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED( WorldObjectPropertiesWindow );

		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void updateSelectedObjectsProps();
	};


} // namespace GUISystem

#endif // WORLD_OBJECT_PROPERTIES_WINDOW_H