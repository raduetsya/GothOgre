#ifndef GOTHOGRE_SOUND_CHANNEL_DATA_H
#define GOTHOGRE_SOUND_CHANNEL_DATA_H

#include "GothOgre_SoundPtr.h"
#include "GothOgre_SoundChannelStatus.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	// Данные звукового канала
	class GOTHOGRE_EXPORT SoundChannelData
	{
	public:
		SoundChannelData() {}
		virtual ~SoundChannelData() {}

		virtual void setVolume(Real _volume) = 0;
		virtual Real getVolume() const = 0;
		virtual Real getPlayPosition() const = 0;
		virtual void setLoop(bool _loop) = 0;
		virtual bool getLoop() const = 0;
		virtual void play() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void unpause() = 0;
		virtual SoundChannelStatus getStatus() const = 0;

		// Implemented only for 3D channels
		virtual void setPosition(const Vector3& _position) {};
		virtual Vector3 getPosition() const {return Vector3::ZERO;}
		virtual void setMaxDistance(Real _distance) {}
		virtual Real getMaxDistance() const {return 0;}

	protected:
		friend class SoundChannel;
		virtual bool isDefault() const {return false;}
		virtual void initBy(const SoundChannelData* _src);
	};
}

#endif // GOTHOGRE_SOUND_CHANNEL_DATA_H