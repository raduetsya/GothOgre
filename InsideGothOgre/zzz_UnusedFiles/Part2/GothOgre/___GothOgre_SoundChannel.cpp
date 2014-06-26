#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundChannel.h"
#include "GothOgre_Sound.h"
#include "GothOgre_SoundOgreManager.h"
#include "GothOgre_SoundChannelGroupImpl.h"
#include "GothOgre_SoundData.h"
#include "GothOgre_SoundChannelData.h"
#include "GothOgre_DefaultSoundChannelData.h"
#include "GothOgre_StringUtil.h"

namespace GothOgre
{
	//------------------------------------------------------------------------
	// SoundChannel
	//------------------------------------------------------------------------
	SoundChannel::SoundChannel(const SoundPtr& _sound, bool _is3D)
		: mSound(_sound), mIs3D(_is3D), mChannelGroupImpl(nullptr), mData(nullptr)
	{
	}
	//------------------------------------------------------------------------
	SoundChannel::~SoundChannel()
	{
		if(!getData()->isDefault())
		{
			SoundOgreManager::getSingleton().removePlayingChannel(this);
			mSound->removeChannelWithNoDefaultData(this);
		}

		if(mChannelGroupImpl)
			mChannelGroupImpl->remove(this);

		if(mData)
			delete mData;
	}
	//------------------------------------------------------------------------
	void SoundChannel::setChannelGroup(const String& _channelGroup)
	{
		if(_channelGroup != getChannelGroup())
		{
			if(mChannelGroupImpl)
			{
				mChannelGroupImpl->remove(this);
				mChannelGroupImpl = nullptr;
			}
			if(!_channelGroup.empty())
			{
				mChannelGroupImpl = SoundOgreManager::getSingleton().getChannelGroupImpl(_channelGroup);
				mChannelGroupImpl->add(this);
			}
		}
	}
	//------------------------------------------------------------------------
	const String& SoundChannel::getChannelGroup() const
	{
		if(mChannelGroupImpl)
			return mChannelGroupImpl->getName();
		else
			return StringUtil::BLANK;
	}
	//------------------------------------------------------------------------
	void SoundChannel::touch()
	{
		SoundPtr sound = getSound();
		sound->touch();

		SoundData* soundData = sound->getData();
		if(getData()->isDefault() && !soundData->isDefault())
		{
			SoundChannelData* data;
			if(mIs3D)
				data = soundData->createChannelData3D();
			else
				data = soundData->createChannelData2D();

			if(data)
			{
				data->initBy(getData());
				setData(data);
				sound->addChannelWithNoDefaultData(this);
			}
		}
	}
	//------------------------------------------------------------------------
	void SoundChannel::resetToDefault()
	{
		if(!getData()->isDefault())
		{
			SoundChannelData* data;
			if(mIs3D)
				data = new DefaultSoundChannelData3D;
			else
				data = new DefaultSoundChannelData2D;

			data->initBy(getData());
			setData(data);
		}
	}
	//------------------------------------------------------------------------
	void SoundChannel::setData(SoundChannelData* _data)
	{
		if(mData != _data)
		{
			if(mData)
				delete mData;

			mData = _data;
		}
	}
	//------------------------------------------------------------------------
	void SoundChannel::setVolume(Real _volume) 
	{
		mVolume = _volume;
		updateVolume();
	}
	//------------------------------------------------------------------------
	Real SoundChannel::getVolume() const 
	{
		return mVolume;
	}
	//------------------------------------------------------------------------
	void SoundChannel::updateVolume()
	{
		Real factor = (mChannelGroupImpl ? mChannelGroupImpl->getVolumeFactor() : 1);
		getData()->setVolume(mVolume * factor);
	}
	//------------------------------------------------------------------------
	Real SoundChannel::getPlayPosition() const 
	{
		return getData()->getPlayPosition();
	}
	//------------------------------------------------------------------------
	void SoundChannel::setLoop(bool _loop) 
	{
		getData()->setLoop(_loop);
	}
	//------------------------------------------------------------------------
	bool SoundChannel::getLoop() const 
	{
		return getData()->getLoop();
	}
	//------------------------------------------------------------------------
	void SoundChannel::play() 
	{ 
		touch();
		getData()->play();

		if(!getData()->isDefault())
			SoundOgreManager::getSingleton().addPlayingChannel(this);
	}
	//------------------------------------------------------------------------
	void SoundChannel::stop() 
	{ 
		getData()->stop();
	}
	//------------------------------------------------------------------------
	void SoundChannel::pause() 
	{
		getData()->pause();
	}
	//------------------------------------------------------------------------
	void SoundChannel::unpause() 
	{
		getData()->unpause();
	}
	//------------------------------------------------------------------------
	SoundChannelStatus SoundChannel::getStatus() const 
	{
		return getData()->getStatus();
	}
	
	
	
	
	//------------------------------------------------------------------------
	// SoundChannel2D
	//------------------------------------------------------------------------
	SoundChannel2D::SoundChannel2D(const SoundPtr& _sound)
		: SoundChannel(_sound, false)
	{
		setData(new DefaultSoundChannelData2D);
	}
	
	
	
	//------------------------------------------------------------------------
	// SoundChannel3D
	//------------------------------------------------------------------------
	SoundChannel3D::SoundChannel3D(const SoundPtr& _sound)
		: SoundChannel(_sound, true)
	{
		setData(new DefaultSoundChannelData3D);
	}
	//------------------------------------------------------------------------
	void SoundChannel3D::setPosition(const Vector3& _position) 
	{
		getData()->setPosition(_position);
	}
	//------------------------------------------------------------------------
	Vector3 SoundChannel3D::getPosition() const 
	{
		return getData()->getPosition();
	}
	//------------------------------------------------------------------------
	void SoundChannel3D::setMaxDistance(Real _distance) 
	{
		getData()->setMaxDistance(_distance);
	}
	//------------------------------------------------------------------------
	Real SoundChannel3D::getMaxDistance() const 
	{
		return getData()->getMaxDistance();
	}

}