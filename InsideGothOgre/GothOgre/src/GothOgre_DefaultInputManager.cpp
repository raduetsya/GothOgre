#include "GothOgre_Precompiled.h"
#include "GothOgre_DefaultInputManager.h"


namespace GothOgre
{
	//----------------------------------------------------------------------
	DefaultInputManager::DefaultInputManager()
	{
		GOTHOGRE_INFO("The default input manager is being used. Keyboard and mouse will not work.");
	}
	//----------------------------------------------------------------------
	DefaultInputManager::~DefaultInputManager()
	{
	}
	//----------------------------------------------------------------------
	void DefaultInputManager::_initialise()
	{
	}
	//----------------------------------------------------------------------
	void DefaultInputManager::_shutdown()
	{
	}
	//----------------------------------------------------------------------
	DefaultInputManagerFactory::DefaultInputManagerFactory()
		: InputManagerFactory("DefaultInputManager")
	{
	}
	//----------------------------------------------------------------------
	DefaultInputManagerFactory::~DefaultInputManagerFactory()
	{
	}
	//----------------------------------------------------------------------
	InputManager* DefaultInputManagerFactory::createInstanceImpl()
	{
		return new DefaultInputManager();
	}

}