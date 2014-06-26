#include "GothOgre_Precompiled.h"
#include "GothOgre_DefaultSoundData.h"
#include "GothOgre_DefaultSoundChannelData.h"


namespace GothOgre
{
	DefaultSoundData::DefaultSoundData()
	{
	}
	//------------------------------------------------------------------------
	DefaultSoundData::~DefaultSoundData()
	{
	}
	//------------------------------------------------------------------------
	SoundChannelData* DefaultSoundData::createChannelData2D() 
	{
		return new DefaultSoundChannelData2D;
	}
	//------------------------------------------------------------------------
	SoundChannelData* DefaultSoundData::createChannelData3D()
	{
		return new DefaultSoundChannelData3D;
	}
}