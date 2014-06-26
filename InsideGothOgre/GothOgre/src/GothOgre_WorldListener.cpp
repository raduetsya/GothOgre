#include "GothOgre_Precompiled.h"
#include "GothOgre_WorldListener.h"
#include "GothOgre_WorldManager.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	WorldListener::WorldListener()
	{
		WorldManager::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	WorldListener::~WorldListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void WorldListener::registerListener()
	{
		WorldManager::getSingleton()._addListener(this);
	}
	//---------------------------------------------------------------------------
	void WorldListener::unregisterListener()
	{
		WorldManager::getSingleton()._removeListener(this);
	}

}