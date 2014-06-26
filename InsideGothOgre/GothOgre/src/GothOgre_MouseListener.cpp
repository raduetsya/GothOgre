#include "GothOgre_Precompiled.h"
#include "GothOgre_MouseListener.h"
#include "GothOgre_InputSystem.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	MouseListener::MouseListener()
	{
		InputSystem::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	MouseListener::~MouseListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void MouseListener::registerListener(Priority _priority)
	{
		InputSystem::getSingleton()._addListener(this, _priority);
	}
	//---------------------------------------------------------------------------
	void MouseListener::unregisterListener()
	{
		if(InputSystem::getSingletonPtr())
			InputSystem::getSingleton()._removeListener(this);
	}

}