#ifndef GOTHOGRE_DEFAULT_SOUND_CHANNEL_DATA_H
#define GOTHOGRE_DEFAULT_SOUND_CHANNEL_DATA_H

#include "GothOgre_SoundChannelData.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	class GOTHOGRE_EXPORT DefaultSoundChannelData : public SoundChannelData
	{
	public:
		DefaultSoundChannelData();
		~DefaultSoundChannelData();

		bool isDefault() const {return true;}

		void setVolume(Real _volume) {mVolume = _volume;}
		Real getVolume() const {return mVolume;}
		Real getPlayPosition() const {return 0;}
		void setLoop(bool _loop) {mLoop = _loop;}
		bool getLoop() const  {return mLoop;}

		void pause() {mStatus = SoundChannelStatus::Paused;}
		void unpause() {mStatus = SoundChannelStatus::None;}
		void play() {}
		void stop() {}
		SoundChannelStatus getStatus() const {return mStatus;}

	private:
		Real                mVolume;
		bool                mLoop;
		SoundChannelStatus  mStatus;
	};


	//------------------------------------------------------------------------
	class DefaultSoundChannelData2D : public DefaultSoundChannelData
	{
	};


	//------------------------------------------------------------------------
	class DefaultSoundChannelData3D : public DefaultSoundChannelData
	{
	public:
		void setPosition(const Vector3& _position) {mPosition = _position;}
		Vector3 getPosition() const {return mPosition;}
		void setMaxDistance(Real _distance) {mMaxDistance = _distance;}
		Real getMaxDistance() const {return mMaxDistance;}

	private:
		Vector3     mPosition;
		Real        mMaxDistance;
	};


} // namespace GothOgre

#endif // GOTHOGRE_DEFAULT_SOUND_CHANNEL_2D_DATA_H