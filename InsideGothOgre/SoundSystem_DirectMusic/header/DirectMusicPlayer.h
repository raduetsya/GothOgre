#ifndef DIRECT_MUSIC_PLAYER_H
#define DIRECT_MUSIC_PLAYER_H


namespace GothOgre
{
	/** Main manager to play sound via DirectMusic. */
	class DirectMusicPlayer : public SoundPlayer
	{
	public:
		/** Constructor */
		DirectMusicPlayer();

		/** Destructor */
		~DirectMusicPlayer();

		/** This function is called by SoundPlayerEnumerator 
		before the first update. */
		void initialise();

		/** This function is called by SoundPlayerEnumerator 
		before removing the SoundPlayer. */
		void shutdown();

		/** Updates the player.
		This function is called for every playing context from time to time. */
		void update(const Vector3& _listenerPosition, 
			const Quaternion& _listenerOrientation);

		/** Creates a player's context. */
		SoundPlayContext* createPlayContext();

		/** Returns true if the player can play a specified file.
		This function can change a filename if it wants. */
		bool canPlay(bool _3D, String& _filename, const String& _resourceGroup) const;

	public:
		IDirectMusicPerformance8*  getPerformance() {return mPerformance;}
		IDirectMusicLoader8*       getLoader() {return mLoader;}
		HANDLE                     getEvent() {return mEvent;}

	private:
		IDirectMusicPerformance8*  mPerformance;
		IDirectMusicLoader8*       mLoader;
		HANDLE                     mEvent;
	};

} // namespace GothOgre

#endif // DIRECT_MUSIC_PLAYER_H