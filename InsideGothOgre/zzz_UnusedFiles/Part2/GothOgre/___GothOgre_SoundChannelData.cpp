#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundChannelData.h"
#include "GothOgre_SoundData.h"


namespace GothOgre
{
	//------------------------------------------------------------------------	
	void SoundChannelData::initBy(const SoundChannelData* _src)
	{
		setVolume(_src->getVolume());
		setLoop(_src->getLoop());
		if(_src->getStatus() == SoundChannelStatus::Paused)
			pause();

		setPosition(_src->getPosition());
		setMaxDistance(_src->getMaxDistance());
	}

}
