#ifndef LIBSNDFILE_AUDIO_CODEC_H
#define LIBSNDFILE_AUDIO_CODEC_H

#include "AudioCodec.h"


namespace GothOgre
{
	/** Specialisation of the AudioDecoder class
	which decodes sound files using the "libsndfile" library.
	It allows to decode compressed *.wav, *.ogg, *.flac files. */
	class LibSndFileAudioCodec : public AudioCodec
	{
	public:
		LibSndFileAudioCodec();
		~LibSndFileAudioCodec();

	protected:
		/** Read the header of the file. */
		void readHeader();

		/** Read the footer of the file. */
		void readFooter();

		/** Read a next portion of data from the file and decode it to buffer. */
		size_t readData(int16* _data, size_t _maxFrames);

		/** Sets the frame offset from the sound's beginning. */
		void setOffset(size_t _frames);

		/** Returns the frame offset from the sound's beginning. */
		size_t getOffset();

	private:
		SNDFILE*              mSndFile;
	};

} // namespace GothOgre

#endif // LIBSNDFILE_AUDIO_CODEC_H