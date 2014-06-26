#include "GothOgre_Precompiled.h"
#include "GothOgre_InputManagerFactory.h"
#include "GothOgre_InputManager.h"
#include "GothOgre_InputManagerEnumerator.h"


namespace GothOgre
{
	//----------------------------------------------------------------------
	InputManagerFactory::InputManagerFactory(const String& _name)
	{
		mName = _name;
		InputManagerEnumerator::getSingleton()._addFactory(this);
	}
	//----------------------------------------------------------------------
	InputManagerFactory::~InputManagerFactory()
	{
		InputManagerEnumerator::getSingleton()._removeFactory(this);
	}
	//----------------------------------------------------------------------
	InputManager* InputManagerFactory::createInstance(RenderWindow* _window)
	{
		InputManager* mgr = createInstanceImpl();
		mgr->_setCreator(this);
		mgr->_setRenderWindow(_window);
		mgr->_initialise();
		return mgr;
	}
	//----------------------------------------------------------------------
	void InputManagerFactory::destroyInstance(InputManager* _manager)
	{
		_manager->_shutdown();
		delete _manager;
	}
}