#include "GothOgre_Precompiled.h"
#include "GothOgre_Sound.h"
#include "GothOgre_SoundChannel.h"
#include "GothOgre_DefaultSoundData.h"


namespace GothOgre
{

	Sound::Sound(Ogre::ResourceManager* _creator, const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual, ManualResourceLoader* _loader)
		: Resource(_creator, _name, _handle, _group, _isManual, _loader)
	{
		mData = new DefaultSoundData;
		mDataOwner = true;
	}
	//---------------------------------------------------------------------
	Sound::~Sound()
	{
		if(mDataOwner)
			delete mData;
	}
	//---------------------------------------------------------------------
	size_t Sound::calculateSize() const
	{
		return mData->getSize();
	}
	//---------------------------------------------------------------------
	void Sound::setData(SoundData* _data, bool _takeOwnership)
	{
		if(mData != _data)
		{
			if(mDataOwner)
				delete mData;

			mData = _data;
			mDataOwner = _takeOwnership;
		}
	}
	//---------------------------------------------------------------------
	void Sound::addChannelWithNoDefaultData(SoundChannel* _channel)
	{
		mChannelsWithNoDefaultData.push_back(_channel);
	}
	//---------------------------------------------------------------------
	void Sound::removeChannelWithNoDefaultData(SoundChannel* _channel)
	{
		ChannelList::iterator it = std::find(mChannelsWithNoDefaultData.begin(), 
			mChannelsWithNoDefaultData.end(), _channel);
		
		if(it != mChannelsWithNoDefaultData.end())
			mChannelsWithNoDefaultData.erase(it);
	}
	//---------------------------------------------------------------------
	void Sound::unloadImpl()
	{
		GOTHOGRE_INFO(getName() << " - Resource unloaded");
		resetToDefault();
	}
	//---------------------------------------------------------------------
	void Sound::resetToDefault()
	{
		for(ChannelList::iterator it = mChannelsWithNoDefaultData.begin();
			it != mChannelsWithNoDefaultData.end(); ++it)
		{
			SoundChannel* channel = *it;
			channel->resetToDefault();
		}
		mChannelsWithNoDefaultData.clear();

		if(!mData->isDefault())
		{
			setData(new DefaultSoundData, true);
		}
	}
	//---------------------------------------------------------------------
	void Sound::loadImpl()
	{
		/** Internal implementation of the meat of the 'load' action, 
			must not be called because this resource type must be
			loaded only via ManualResourceLoader. */
		GOTHOGRE_EXCEPT("Sound::loadImpl never should be called!");
	}
	//---------------------------------------------------------------------
	void Sound::postLoadImpl()
	{
		Resource::postLoadImpl();
		if(!mData->isDefault())
			mDuration = mData->getDuration();
	}
}