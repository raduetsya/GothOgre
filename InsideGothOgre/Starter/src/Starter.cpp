#include "GothOgre.h"

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
	GothOgre::App* app = new GothOgre::App(GetCommandLine());
#else
int main(int argc, char **argv)
{
	GothOgre::App* app = new GothOgre::App(argc, argv);
#endif

	app->run();

	delete app;
    return 0;
}

#ifdef __cplusplus
}
#endif


