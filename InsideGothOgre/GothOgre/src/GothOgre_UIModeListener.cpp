#include "GothOgre_Precompiled.h"
#include "GothOgre_UIModeListener.h"
#include "GothOgre_UIModeManager.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	UIModeListener::UIModeListener()
	{
		UIModeManager::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	UIModeListener::~UIModeListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void UIModeListener::registerListener(Priority _priority)
	{
		UIModeManager::getSingleton()._addListener(this, _priority);
	}
	//---------------------------------------------------------------------------
	void UIModeListener::unregisterListener()
	{
		if(UIModeManager::getSingletonPtr())
			UIModeManager::getSingleton()._removeListener(this);
	}
}