#include "GothOgre_Precompiled.h"
#include "GothOgre_WindowEventListener.h"
#include "GothOgre_InputSystem.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	NewWindowEventListener::NewWindowEventListener()
	{
		InputSystem::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	NewWindowEventListener::~NewWindowEventListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void NewWindowEventListener::registerListener()
	{
		InputSystem::getSingleton()._addListener(this);
	}
	//---------------------------------------------------------------------------
	void NewWindowEventListener::unregisterListener()
	{
		InputSystem::getSingleton()._removeListener(this);
	}

}