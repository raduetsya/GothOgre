#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundOgreManager.h"
#include "GothOgre_SoundChannelGroupImpl.h"
#include "GothOgre_SoundChannel.h"
#include "GothOgre_Sound.h"

//-------------------------------------------------------------------------
GothOgre::SoundOgreManager* 
	Ogre::Singleton<GothOgre::SoundOgreManager>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	SoundOgreManager::SoundOgreManager()
	{
		// Resource type
		mResourceType = "Sound";
		ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	}
	//-------------------------------------------------------------------------
	SoundOgreManager::~SoundOgreManager()
	{
		for(ChannelGroupImpls::iterator it = mChannelGroupImpls.begin();
			it != mChannelGroupImpls.end(); ++it)
		{
			SoundChannelGroupImpl* impl = it->second;
			delete impl;
		}

		ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	}
	//-------------------------------------------------------------------------
	Resource* SoundOgreManager::createImpl(const String& _name, ResourceHandle _handle, 
		const String& _group, bool _isManual, ManualResourceLoader* _loader, 
		const NameValuePairList* _createParams)
	{
		return OGRE_NEW Sound(this, _name, _handle, _group, _isManual, _loader);
	}
	//-------------------------------------------------------------------------
	SoundChannel2DPtr SoundOgreManager::createChannel2D(const SoundPtr& _sound)
	{
		return (SoundChannel2DPtr) new SoundChannel2D(_sound);
	}
	//-------------------------------------------------------------------------
	SoundChannel3DPtr SoundOgreManager::createChannel3D(const SoundPtr& _sound)
	{
		return (SoundChannel3DPtr) new SoundChannel3D(_sound);
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::addPlayingChannel(SoundChannel* _channel)
	{
		mPlayingChannels.push_back(_channel);
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::removePlayingChannel(SoundChannel* _channel)
	{
		ChannelVector::iterator it = std::find(mPlayingChannels.begin(), 
			mPlayingChannels.end(), _channel);
		if(it != mPlayingChannels.end())
			mPlayingChannels.erase(it);
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::frameRenderingQueued(const FrameEvent& _evt)
	{
		CachedResourceOgreManager::frameRenderingQueued(_evt);

		size_t i = 0; 
		while(i != mPlayingChannels.size())
		{
			SoundChannel* channel = mPlayingChannels[i];
			SoundChannelStatus status = channel->getStatus();
			if(status == SoundChannelStatus::Playing
				|| status == SoundChannelStatus::Paused)
			{
				channel->getSound()->touch();
				++i;
			}
			else
			{
				mPlayingChannels[i] = mPlayingChannels.back();
				mPlayingChannels.pop_back();
			}
		}
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::setChannelGroupVolume(const String& _channelGroup, Real _volumeFactor)
	{
		SoundChannelGroupImpl* impl = getChannelGroupImpl(_channelGroup);
		impl->setVolumeFactor(_volumeFactor);
	}
	//-------------------------------------------------------------------------
	Real SoundOgreManager::getChannelGroupVolume(const String& _channelGroup) const
	{
		SoundChannelGroupImpl* impl = getChannelGroupImpl(_channelGroup);
		return impl->getVolumeFactor();
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::pauseChannelGroup(const String& _channelGroup)
	{
		SoundChannelGroupImpl* impl = getChannelGroupImpl(_channelGroup);
		impl->pause();
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::unpauseChannelGroup(const String& _channelGroup)
	{
		SoundChannelGroupImpl* impl = getChannelGroupImpl(_channelGroup);
		impl->unpause();
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::stopChannelGroup(const String& _channelGroup)
	{
		SoundChannelGroupImpl* impl = getChannelGroupImpl(_channelGroup);
		impl->stop();
	}
	//-------------------------------------------------------------------------
	SoundChannelGroupImpl* SoundOgreManager::getChannelGroupImpl(const String& _channelGroup) const
	{
		ChannelGroupImpls::const_iterator it = mChannelGroupImpls.find(_channelGroup);
		if(it == mChannelGroupImpls.end())
		{
			it = (const_cast<SoundOgreManager*>(this)->mChannelGroupImpls)
				.insert( std::make_pair(_channelGroup, 
					new SoundChannelGroupImpl(_channelGroup)) ).first;
		}

		SoundChannelGroupImpl* impl = it->second;
		return impl;
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::setListenerPosition(const Vector3& _position)
	{
		mListenerPosition = _position;
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::setListenerDirection(const Vector3& _direction)
	{
		mListenerDirection = _direction;
	}
	//-------------------------------------------------------------------------
	void SoundOgreManager::setListenerUp(const Vector3& _up)
	{
		mListenerUp = _up;
	}


} // namespace GothOgre