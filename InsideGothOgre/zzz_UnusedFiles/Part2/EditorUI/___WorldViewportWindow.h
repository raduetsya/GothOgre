#ifndef WORLD_VIEWPORT_WINDOW_H
#define WORLD_VIEWPORT_WINDOW_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_LAYOUT        ( WorldViewportWindow, "WorldViewportWindow.layout");
	ATTRIBUTE_CLASS_CONFIG_SECTION( WorldViewportWindow, "WorldViewportWindow");

	class WorldViewportWindow : BaseLayout
	{
	public:
		WorldViewportWindow();

	private:
		void notifyWindowChangeCoord(MyGUI::Window* _sender);
	};

} // namespace GUISystem

#endif // WORLD_VIEWPORT_WINDOW_H