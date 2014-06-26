#ifndef GOTHOGRE_DEFAULT_SOUND_DATA_H
#define GOTHOGRE_DEFAULT_SOUND_DATA_H

#include "GothOgre_SoundData.h"


namespace GothOgre
{
	class GOTHOGRE_EXPORT DefaultSoundData : public SoundData
	{
	public:
		DefaultSoundData();
		~DefaultSoundData();

		size_t getSize() const {return 0;}
		Real getDuration() const {return 0;}
		SoundChannelData* createChannelData2D();
		SoundChannelData* createChannelData3D();

	protected:
		bool isDefault() const {return true;}
	};

} // namespace GothOgre

#endif // GOTHOGRE_DEFAULT_SOUND_DATA_H