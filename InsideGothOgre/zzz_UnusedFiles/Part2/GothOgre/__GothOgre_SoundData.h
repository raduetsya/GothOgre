#ifndef GOTHOGRE_SOUND_SAMPLE_H
#define GOTHOGRE_SOUND_SAMPLE_H

#include "GothOgre_SoundPtr.h"

namespace GothOgre
{
	class SoundChannelData;

	// Данные звукового ресурса
	class SoundData
	{
	public:
		SoundData() {}
		virtual ~SoundData() {}

		/** Returns number of bytes allocated to store this sound resource. */
		virtual size_t getSize() const = 0;

		/** Returns the sound's total length, in seconds. */
		virtual Real getDuration() const = 0;
		
		/** Creates sound channel (source) */
		virtual SoundChannelData* createChannelData2D() {return nullptr;}
		virtual SoundChannelData* createChannelData3D() {return nullptr;}

	protected:
		friend class Sound;
		friend class SoundChannel;
		virtual bool isDefault() const {return false;}
	};

}

#endif // GOTHOGRE_SOUND_SAMPLE_H