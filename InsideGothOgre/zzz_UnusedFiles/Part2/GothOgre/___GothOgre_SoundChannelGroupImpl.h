#ifndef GOTHOGRE_SOUND_CHANNEL_GROUP_IMPL_H
#define GOTHOGRE_SOUND_CHANNEL_GROUP_IMPL_H


namespace GothOgre
{
	class SoundChannel;


	//-------------------------------------------------------------------
	class SoundChannelGroupImpl
	{
	public:
		SoundChannelGroupImpl(const String& _name);
		~SoundChannelGroupImpl();

		const String& getName() const;

		void add(SoundChannel* _soundChannel);
		void remove(SoundChannel* _soundChannel);

		void setVolumeFactor(Real _factor);
		Real getVolumeFactor() const;

		void pause();
		void unpause();
		void stop();

	private:
		typedef set<SoundChannel*>::type  SoundChannels;

		String          mName;
		Real            mVolumeFactor;
		SoundChannels   mSoundChannels;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_CHANNEL_GROUP_IMPL_H