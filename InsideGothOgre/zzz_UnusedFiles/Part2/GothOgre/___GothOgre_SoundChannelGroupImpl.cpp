#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundChannelGroupImpl.h"
#include "GothOgre_SoundChannel.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	SoundChannelGroupImpl::SoundChannelGroupImpl(const String& _name)
		: mName(_name), mVolumeFactor(1)
	{
	}
	//-----------------------------------------------------------------------
	SoundChannelGroupImpl::~SoundChannelGroupImpl()
	{
	}
	//-----------------------------------------------------------------------
	const String& SoundChannelGroupImpl::getName() const
	{
		return mName;
	}
	//-----------------------------------------------------------------------
	void SoundChannelGroupImpl::add(SoundChannel* _soundChannel)
	{
		mSoundChannels.insert(_soundChannel);
	}
	//-----------------------------------------------------------------------
	void SoundChannelGroupImpl::remove(SoundChannel* _soundChannel)
	{
		SoundChannels::iterator it = mSoundChannels.find(_soundChannel);
		if(it != mSoundChannels.end())
			mSoundChannels.erase(it);
	}
	//-----------------------------------------------------------------------
	void SoundChannelGroupImpl::setVolumeFactor(Real _factor)
	{
		if(mVolumeFactor != _factor)
		{
			mVolumeFactor = _factor;
			for(SoundChannels::iterator it = mSoundChannels.begin();
				it != mSoundChannels.end(); ++it)
			{
				SoundChannel* soundChannel = *it;
				soundChannel->updateVolume();
			}
		}
	}
	//-----------------------------------------------------------------------
	Real SoundChannelGroupImpl::getVolumeFactor() const
	{
		return mVolumeFactor;
	}
	//-----------------------------------------------------------------------
	void SoundChannelGroupImpl::pause()
	{
		for(SoundChannels::iterator it = mSoundChannels.begin();
			it != mSoundChannels.end(); ++it)
		{
			SoundChannel* soundChannel = *it;
			soundChannel->pause();
		}
	}
	//-----------------------------------------------------------------------
	void SoundChannelGroupImpl::unpause()
	{
		for(SoundChannels::iterator it = mSoundChannels.begin();
			it != mSoundChannels.end(); ++it)
		{
			SoundChannel* soundChannel = *it;
			soundChannel->unpause();
		}
	}
	//-----------------------------------------------------------------------
	void SoundChannelGroupImpl::stop()
	{
		for(SoundChannels::iterator it = mSoundChannels.begin();
			it != mSoundChannels.end(); ++it)
		{
			SoundChannel* soundChannel = *it;
			soundChannel->stop();
		}
	}

} // namespace GothOgre