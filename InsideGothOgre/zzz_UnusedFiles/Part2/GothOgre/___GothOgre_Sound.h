#ifndef GOTHOGRE_SOUND_H
#define GOTHOGRE_SOUND_H

#include "GothOgre_SoundPtr.h"


namespace GothOgre
{
	class SoundData;

	// Звуковой ресурс, соответствует одному аудиофайлу,
	// одному звуковому ресурсу может соответствовать много звуковых каналов
	class GOTHOGRE_EXPORT Sound : public Resource
	{
	public:
		Sound(Ogre::ResourceManager* _creator, const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual = false, ManualResourceLoader* _loader = 0);
		virtual ~Sound();

		/** Returns length of the sound, in seconds;
		if the resource was not loaded yet, returns 0. */
		Real getDuration() const;

		void setData(SoundData* _data, bool _takeOwnership = true);
		SoundData* getData() const {return mData;}

	protected:
		void loadImpl();
		void postLoadImpl();
		void unloadImpl();
		
		/** Calculate the size of a resource; 
		this will only be called after 'load' */
		size_t calculateSize() const;

	private:
		friend class SoundChannel;
		void addChannelWithNoDefaultData(SoundChannel* _channel);
		void removeChannelWithNoDefaultData(SoundChannel* _channel);

	private:
		void resetToDefault();

	private:
		SoundData*   mData;
		bool         mDataOwner;
		Real         mDuration;

		typedef list<SoundChannel*>::type  ChannelList;
		ChannelList  mChannelsWithNoDefaultData;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_H