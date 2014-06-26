#include "GothOgre_Precompiled.h"
#include "GothOgre_FrameListener.h"
#include "GothOgre_InputSystem.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------
	NewFrameListener::NewFrameListener()
	{
		InputSystem::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	NewFrameListener::~NewFrameListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void NewFrameListener::registerListener()
	{
		InputSystem::getSingleton()._addListener(this);
	}
	//---------------------------------------------------------------------------
	void NewFrameListener::unregisterListener()
	{
		InputSystem::getSingleton()._removeListener(this);
	}

}