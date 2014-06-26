#include "Precompiled.h"
#include "AudioCodec.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	AudioCodec::AudioCodec()
	{
	}
	//---------------------------------------------------------------------------
	AudioCodec::~AudioCodec()
	{
	}
	//---------------------------------------------------------------------------
	void AudioCodec::open(const String& _filename, const String& _resourceGroup)
	{
		mDataStream = ResourceGroupManager::getSingleton().openResource(_filename, _resourceGroup, false);
		readHeader();
	}
	//---------------------------------------------------------------------------
	void AudioCodec::close()
	{
		readFooter();
		mDataStream.setNull();
	}

}