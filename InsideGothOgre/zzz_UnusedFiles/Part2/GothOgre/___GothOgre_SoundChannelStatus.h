#ifndef GOTHOGRE_SOUND_CHANNEL_STATUS_H
#define GOTHOGRE_SOUND_CHANNEL_STATUS_H


namespace GothOgre
{
	/***/
	class SoundChannelStatus
	{
	public:
		enum Enum
		{
			None,
			Playing,
			Stopped,
			Paused
		};

		SoundChannelStatus(Enum _value = None) : mValue(_value) {}
		operator int() const {return mValue;}

	private:
		Enum mValue;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_CHANNEL_STATUS_H