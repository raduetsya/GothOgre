#include "GothOgre_Precompiled.h"
#include "GothOgre_SkyListener.h"
#include "GothOgre_SkySystem.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	SkyListener::SkyListener()
	{
		SkySystem::getSingleton()._initListener(this);
	}
	//---------------------------------------------------------------------------
	SkyListener::~SkyListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void SkyListener::registerListener()
	{
		SkySystem::getSingleton()._addListener(this);
	}
	//---------------------------------------------------------------------------
	void SkyListener::unregisterListener()
	{
		if(SkySystem::getSingletonPtr())
			SkySystem::getSingleton()._removeListener(this);
	}

}