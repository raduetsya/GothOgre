#ifndef OPENAL_PLAYER_H
#define OPENAL_PLAYER_H

#include "AudioCodec.h"


namespace GothOgre
{
	/** Main manager to play sound via OpenAL. */
	class OpenALPlayer : public SoundPlayer
	{
	public:
		/** Constructor */
		OpenALPlayer();

		/** Destructor */
		~OpenALPlayer();

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

	protected:
		virtual AudioCodec* createAudioCodec() = 0;

	private:
		void openALDevice();

	private:
		ALCdevice*   mALDevice;
		ALCcontext*  mALContext;
	};

} // namespace GothOgre

#endif // OPENAL_PLAYER_H