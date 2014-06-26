#ifndef GOTHOGRE_SOUND_CHANNEL_H
#define GOTHOGRE_SOUND_CHANNEL_H

#include "GothOgre_SoundPtr.h"
#include "GothOgre_SoundChannelStatus.h"

namespace GothOgre
{
	class SoundChannelData;
	class SoundChannelGroupImpl;


	//-----------------------------------------------------------------------
	// Звуковой канал - это источник звука
	class GOTHOGRE_EXPORT SoundChannel
	{
	public:
		SoundChannel(const SoundPtr& _sound, bool _is3D);
		~SoundChannel();

		void setChannelGroup(const String& _channelGroup);
		const String& getChannelGroup() const;

		SoundPtr getSound() const {return mSound;}
		bool is3D() const {return mIs3D;}

		// Sets the volume of the sound, float value in range [0..1]
		void setVolume(Real _volume);

		// Returns the volume of the sound, float value in range [0..1]
		Real getVolume() const;

		// Returns the current playback position, in seconds
		Real getPlayPosition() const;

		void setLoop(bool _loop);
		bool getLoop() const;

		void play();
		void stop();
		void pause();
		void unpause();
		
		SoundChannelStatus getStatus() const;

		void touch();
		SoundChannelData* getData() const {return mData;}

	private:
		friend class Sound;
		void resetToDefault();

	private:
		friend class SoundChannelGroupImpl;
		void updateVolume();

	protected:
		void setData(SoundChannelData* _data);

	private:
		SoundPtr                mSound;
		SoundChannelData*       mData;
		bool                    mIs3D;
		Real                    mVolume;
		SoundChannelGroupImpl*  mChannelGroupImpl;
	};


	//-----------------------------------------------------------------------
	// Двухмерный (рассеянный) звуковой канал
	class GOTHOGRE_EXPORT SoundChannel2D : public SoundChannel
	{
	public:
		SoundChannel2D(const SoundPtr& _sound);
		~SoundChannel2D() {}
	};


	//-----------------------------------------------------------------------
	// Трехмерный (пространственный) звуковой канал
	class GOTHOGRE_EXPORT SoundChannel3D : public SoundChannel
	{
	public:
		SoundChannel3D(const SoundPtr& _sound);
		~SoundChannel3D() {}

		void setPosition(const Vector3& _position);
		Vector3 getPosition() const;
		void setMaxDistance(Real _distance);
		Real getMaxDistance() const;
	};


	//-----------------------------------------------------------------------
	typedef SharedPtr<SoundChannel2D> SoundChannel2DPtr;
	typedef SharedPtr<SoundChannel3D> SoundChannel3DPtr;
} 

#endif // GOTHOGRE_SOUND_CHANNEL_H