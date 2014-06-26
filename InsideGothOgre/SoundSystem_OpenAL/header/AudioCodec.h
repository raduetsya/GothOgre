#ifndef AUDIO_CODEC_H
#define AUDIO_CODEC_H


namespace GothOgre
{
	/** Abstract class for decoding audio data. */
	class AudioCodec
	{
	public:
		AudioCodec();
		virtual ~AudioCodec();

		/** Reads header of a specified file. */
		void open(const String& _filename, const String& _resourceGroup);

		/** Closes the file, if there is no more users of it. */
		void close();

		/** Information from header of audio file. */
		struct SoundInfo
		{
			/** Number of samples in the sound file (per one channel). */
			size_t  frames;

			/** Number of sample per second (per one channel), i.e. frequency.
			Common value are 44100, 22050, 11025, 8000. */
			size_t  framesPerSec;

			/** Number of channels; 1 or 2 (1=MONO, 2=STEREO). */
			uchar   channels;
		};

		/** Returns an information which was read from header. */
		const SoundInfo& getSoundInfo() const {return mSoundInfo;}

		/** Reads a next portion of data from the file and decode it to buffer. 
		@param _data A pointer to a preallocated buffer, where the read data will be written to.
		The size of the buffer must not be less than _maxFrames * channels.
		The sound is supposed to be 16-bit, so the parameter is a pointer to int16.
		@param _maxFrames The maximum number of frames which can be read to the buffer.
		@return The number of frames which was read and decoded to the buffer. */
		virtual size_t readData(int16* _data, size_t _maxFrames) = 0;

		/** Sets the frame offset from the sound's beginning. */
		virtual void setOffset(size_t _frames) = 0;

		/** Returns the frame offset from the sound's beginning. */
		virtual size_t getOffset() = 0;

	protected:
		virtual void readHeader() {}
		virtual void readFooter() {}

	protected:
		DataStreamExPtr  mDataStream;
		SoundInfo        mSoundInfo;
	};

} // namespace GothOgre

#endif // AUDIO_CODEC_H