#include "GothOgre_Precompiled.h"
#include "GothOgre_WorldTimeListener.h"
#include "GothOgre_WorldTime.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	WorldTimeListener::WorldTimeListener()
	{
		WorldTime::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	WorldTimeListener::~WorldTimeListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void WorldTimeListener::registerListener(const Time& _firstActivation, const Time& _period)
	{
		WorldTime::getSingleton()._addListener(this, _firstActivation, _period);
	}
	//---------------------------------------------------------------------------
	void WorldTimeListener::unregisterListener()
	{
		WorldTime::getSingleton()._removeListener(this);
	}

}