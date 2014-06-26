#ifndef GOTHOGRE_WINDOW_EVENT_UTILITIES_H
#define GOTHOGRE_WINDOW_EVENT_UTILITIES_H


namespace GothOgre
{
	class GOTHOGRE_EXPORT WindowEventUtilities
	{
	public:
		static void addWindowEventListener( RenderWindow* _window, WindowEventListener* _listener );
		static void removeWindowEventListener( RenderWindow* _window, WindowEventListener* _listener );
		static size_t getNumWindowEventListeners();
	};

} // namespace GothOgre


#endif // GOTHOGRE_WINDOW_EVENT_UTILITIES_H