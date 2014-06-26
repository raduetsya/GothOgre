#ifndef DIRECT_MUSIC_PLAY_CONTEXT_H
#define DIRECT_MUSIC_PLAY_CONTEXT_H


namespace GothOgre
{
	/** Play context to play segments (*.sgt) via DirectMusic. */
	class DirectMusicPlayContext : public SoundPlayContext
	{
	public:
		DirectMusicPlayContext(IDirectMusicPerformance8* _performance, IDirectMusicLoader8* _loader, HANDLE _event);
		~DirectMusicPlayContext();

		/** Starts playing of the sound. */
		void play(bool _3D, const String& _filename, const String& _resourceGroup);

		/** Updates this context. */
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

		/** Returns the length of the sound track, in seconds. */
		Real getPlayLength();

		/** Returns the playback position, in seconds. */
		Real getPlayPosition();

	private:
		void _update(DMUS_NOTIFICATION_PMSG* _message);
		Real musicTimeToSeconds(MUSIC_TIME mtTime);

	private:
		IDirectMusicPerformance8*  mPerformance;
		IDirectMusicLoader8*       mLoader;
		HANDLE                     mEvent;
		IDirectMusicSegment8*      mSegment;
		IDirectMusicAudioPath8*    mAudioPath;
		IDirectMusicSegmentState*  mSegmentState;
		bool                       mPlaying;
		bool                       mLooping;
		MUSIC_TIME                 mStartMusicTime;
		MUSIC_TIME                 mEndMusicTime;
	};

}

#endif // DIRECT_MUSIC_PLAY_CONTEXT_H