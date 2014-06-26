#ifndef GUISYSTEM_BACKGROUND_COLOR_SETTER_H
#define GUISYSTEM_BACKGROUND_COLOR_SETTER_H


namespace GUISystem
{
	/** This class reads from the tags file the background color
	and sets it for the background viewport. */
	class BackgroundColorSetter
	{
	public:
		BackgroundColorSetter();
		~BackgroundColorSetter();

	private:
		GOTHOGRE_DECLARE_UI_MODE_ENTERED( BackgroundColorSetter );
	};
	
} // namespace GUISystem

#endif // GUISYSTEM_BACKGROUND_COLOR_SETTER_H