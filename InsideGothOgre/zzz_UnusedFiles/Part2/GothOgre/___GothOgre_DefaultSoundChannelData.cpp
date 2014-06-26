#include "GothOgre_Precompiled.h"
#include "GothOgre_DefaultSoundChannelData.h"


namespace GothOgre
{
	DefaultSoundChannelData::DefaultSoundChannelData() 
	{
		mVolume = 1.0f;
		mLoop = false;
		mStatus = SoundChannelStatus::None;
	}
	//-------------------------------------------------------------------------	
	DefaultSoundChannelData::~DefaultSoundChannelData() 
	{
	}
}