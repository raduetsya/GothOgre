#include "GothOgre_Precompiled.h"
#include "GothOgre_KeyListener.h"
#include "GothOgre_InputSystem.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	KeyListener::KeyListener()
	{
		InputSystem::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	KeyListener::~KeyListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void KeyListener::registerListener(Priority _priority)
	{
		InputSystem::getSingleton()._addListener(this, _priority);
	}
	//---------------------------------------------------------------------------
	void KeyListener::unregisterListener()
	{
		if(InputSystem::getSingletonPtr())
			InputSystem::getSingleton()._removeListener(this);
	}

}