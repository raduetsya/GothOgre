#ifndef OPENAL_PLAY_CONTEXT_H
#define OPENAL_PLAY_CONTEXT_H


namespace GothOgre
{
	class AudioCodec;

	/** Play context to play sound via OpenAL. */
	class OpenALPlayContext : public SoundPlayContext
	{
	public:
		OpenALPlayContext(AudioCodec* _audioCodec);
		~OpenALPlayContext();

		/** Starts playing of the sound. */
		void play(bool _3D, const String& _filename, const String& _resourceGroup);

		/** Updates this context. For example, 
		an implementation can loads a next segment of the sound's data, and so on. 
		This function is called for every playing context from time to time. */
		void update();

		/** Stops playing of the sound. */
		void stop();

		/** Whether the sound is playing. */
		bool isPlaying();

		/** Turns looping on (true) or off (false) */
		void setLooping(bool _looping);

		/** Sets the attenuation of the sound, in range [0..1]. 
		0 is silence, and 1 is the max volume. */
		void setVolume(Real _volume);

		/** Sets the position of the sound in 3D space.
		Only for 3D sounds. */
		void setPosition(const Vector3& _position);

		/** Sets the maximum distance, which is the distance from the listener 
		beyond which sound is no longer attenuated. 
		Only for 3D sounds. */
		void setMaxDistance(Real _radius);

		/** Pause the playback. */
		void pause();

		/** Unpause the playback. */
		void unpause();

		/** Is the playback on pause? */
		bool isPaused();

		/** Returns the length of the sound track, in seconds. */
		Real getPlayLength();

		/** Returns the playback position, in seconds. */
		Real getPlayPosition();

		/** Sets the playback position. */
		void setPlayPosition(Real _position);

	private:
		void _play(size_t _startFrame);
		void _stop();
		size_t unqueueProcessedBuffers();
		size_t queueNewBuffers();
		bool checkStopPlaying();
		void clearQueue();

	private:
		ALuint                mSource;
		vector<ALuint>::type  mBuffers;
		vector<ALuint>::type  mFreeBuffers;
		float                 mBufferTime;
		size_t                mMaxNumBuffers;
		bool                  mLooping;
		bool                  mPlaying;
		bool                  mPaused;
		bool                  mWaitMoreData;
		size_t                mStartFrame;
		AudioCodec*           mAudioCodec;
		vector<int16>::type   mWaveData;
		size_t                mNumStopsAtFinalBlock;
	};

}

#endif // OPENAL_PLAY_CONTEXT_H