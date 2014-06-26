#include "GothOgre_Precompiled.h"
#include "GothOgre_WorldObjectListener.h"
#include "GothOgre_WorldManager.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	WorldObjectListener::WorldObjectListener()
	{
		WorldManager::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	WorldObjectListener::~WorldObjectListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void WorldObjectListener::registerListener()
	{
		WorldManager::getSingleton()._addListener(this);
	}
	//---------------------------------------------------------------------------
	void WorldObjectListener::unregisterListener()
	{
		WorldManager::getSingleton()._removeListener(this);
	}

}