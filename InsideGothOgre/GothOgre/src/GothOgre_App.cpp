#include "GothOgre_Precompiled.h"
#include "GothOgre_App.h"
#include "GothOgre_AppImpl.h"

//----------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#	define SHOW_EXCEPTION_MESSAGE_BOX(_message) \
		MessageBox( NULL, _message, "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL)
#else
#	define SHOW_EXCEPTION_MESSAGE_BOX(_message) \
		std::cerr << "An exception has occured: " << _message
#endif

#define TRY \
	try {

#define END_TRY \
	} \
	catch (std::exception& _exception) \
	{ \
		if(LogSystem::getSingletonPtr()) \
			GOTHOGRE_CRITICAL(_exception.what()); \
		SHOW_EXCEPTION_MESSAGE_BOX(_exception.what()); \
	}
//----------------------------------------------------------------------------


namespace GothOgre
{
	
	App::App(int _argc, char** _argv)
	{
		mImpl = new AppImpl;
		TRY
		mImpl->initialise(_argc, _argv);
		END_TRY
	}
	//------------------------------------------------------------------------
	App::App(char* _cmdLine)
	{
		mImpl = new AppImpl;
		TRY
		mImpl->initialise(_cmdLine);
		END_TRY
	}
	//------------------------------------------------------------------------
	void App::run()
	{
		if(!mImpl->initOk())
			return;

		TRY
		mImpl->renderingLoop();
		END_TRY
	}
	//------------------------------------------------------------------------
	App::~App()
	{
		TRY
		mImpl->shutdown();
		END_TRY
		delete mImpl;
	}

} // namespace GothOgre