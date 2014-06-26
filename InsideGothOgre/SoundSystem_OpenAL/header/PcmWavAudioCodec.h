#ifndef PCMWAV_AUDIO_CODEC_H
#define PCMWAV_AUDIO_CODEC_H

#include "AudioCodec.h"


namespace GothOgre
{
	/** Specialisation of the AudioDecoder class
	which can read simple .WAV files of the "PCM" format 
	(i.e. without any compression). */
	class PcmWavAudioCodec : public AudioCodec
	{
	public:
		PcmWavAudioCodec();
		~PcmWavAudioCodec();

	protected:
		/** Read the header of the file. */
		void readHeader();

		/** Read a next portion of data from the file and decode it to buffer. */
		size_t readData(int16* _data, size_t _maxFrames);

		/** Sets the frame offset from the sound's beginning. */
		void setOffset(size_t _frames);

		/** Returns the frame offset from the sound's beginning. */
		size_t getOffset();

	private:
		#pragma pack(push, 1)

		struct Chunk
		{
			uint8  type[4];
			uint32 size;
		};

		enum Format : uint16
		{
			FORMAT_PCM = 1
		};

		struct Fmt
		{
			Format   format;
			uint16   channels;
			uint32   samplesPerSec;
			uint32   avgBytesPerSec;
			uint16   blockAlign;
			uint16   bitsPerSample;
		};

		#pragma pack(pop)

	private:
		uchar         mBytesPerSampleInFile;
		size_t        mStartOffset;
		size_t        mEndOffset;
	};

} // namespace GothOgre

#endif // PCMWAV_AUDIO_CODEC_H