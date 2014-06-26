#ifndef PCM_WAV_OPEN_AL_PLAYER_H
#define PCM_WAV_OPEN_AL_PLAYER_H

#include "OpenALPlayer.h"
#include "PcmWavAudioCodec.h"


namespace GothOgre
{
	class PcmWavOpenALPlayer : public OpenALPlayer
	{
	protected:
		AudioCodec* createAudioCodec()
		{
			return new PcmWavAudioCodec;
		}
	};
}

#endif // PCM_WAV_OPEN_AL_PLAYER_H