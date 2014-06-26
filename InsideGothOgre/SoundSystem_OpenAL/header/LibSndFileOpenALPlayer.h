#ifndef LIBSNDFILE_OPEN_AL_PLAYER_H
#define LIBSNDFILE_OPEN_AL_PLAYER_H

#include "OpenALPlayer.h"
#include "LibSndFileAudioCodec.h"


namespace GothOgre
{
	class LibSndFileOpenALPlayer : public OpenALPlayer
	{
	protected:
		AudioCodec* createAudioCodec()
		{
			return new LibSndFileAudioCodec;
		}
	};
}

#endif // LIBSNDFILE_OPEN_AL_PLAYER_H