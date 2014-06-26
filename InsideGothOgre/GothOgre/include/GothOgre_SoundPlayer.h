#ifndef GOTHOGRE_SOUND_PLAYER_H
#define GOTHOGRE_SOUND_PLAYER_H

#include "GothOgre_Priority.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	/** An abstract class to store data of a playing sound source. 
	This class should not be used directly, it must be used only via
	appropriate resource manager. */
	class SoundPlayContext
	{
	public:
		SoundPlayContext() {}
		virtual ~SoundPlayContext() {}

		/** Starts playing of the sound. */
		virtual void play(bool _3D, const String& _filename, const String& _resourceGroup) = 0;

		/** Updates this context. For example, 
		an implementation can loads a next segment of the sound's data, and so on. 
		This function is called for every playing context from time to time. */
		virtual void update() {}

		/** Stops playing of the sound. */
		virtual void stop() = 0;

		/** Whether the sound is playing. */
		virtual bool isPlaying() = 0;

		/** Turns looping on (true) or off (false) */
		virtual void setLooping(bool _looping) {}

		/** Sets the attenuation of the sound, in range [0..1]. 
		0 is silence, and 1 is the max volume. */
		virtual void setVolume(Real _volume) {}

		/** Sets the position of the sound in 3D space.
		Only for 3D sounds. */
		virtual void setPosition(const Vector3& _position) {}

		/** Sets the maximum distance, which is the distance from the listener 
		beyond which sound is no longer attenuated. 
		Only for 3D sounds. */
		virtual void setMaxDistance(Real _radius) {}

		/** Pause the playback. */
		virtual void pause() {}

		/** Unpause the playback. */
		virtual void unpause() {}

		/** Is the playback on pause? */
		virtual bool isPaused() {return false;}

		/** Returns the length of the sound track, in seconds. */
		virtual Real getPlayLength() {return 0;}

		/** Returns the playback position, in seconds. */
		virtual Real getPlayPosition() {return 0;}

		/** Sets the playback position. */
		virtual void setPlayPosition(Real _position) {}
	};

	typedef SharedPtr<SoundPlayContext> SoundPlayContextPtr;



	//-----------------------------------------------------------------------
	/** An abstract class for a sound player.
	Derive your own class from this class to support a new file format.
	This class should not be used directly, it must be used only via SoundSystem. */
	class GOTHOGRE_EXPORT SoundPlayer
	{
	public:
		/** Constructor */
		SoundPlayer(const String& _name, Priority _priority = Priority::LOWEST);

		/** Destructor */
		virtual ~SoundPlayer();

		/** Returns name of the player. */
		const String& getName() const {return mName;}

		/** This function is called by SoundPlayerEnumerator 
		before the first update. */
		virtual void initialise() {}

		/** This function is called by SoundPlayerEnumerator 
		before removing the SoundPlayer. */
		virtual void shutdown() {}

		/** Updates the player.
		This function is called for every playing context from time to time. */
		virtual void update(const Vector3& _listenerPosition, 
			const Quaternion& _listenerOrientation) {}

		/** Returns true if the player can play a specified file.
		This function can change a filename if it wants.
		@remark
		The default implementation returns true if the specified file
		exists and the extension of the specified file matchs to an extension
		returned by the getFileExt function. */
		virtual bool canPlay(bool _3D, String& _filename, const String& _resourceGroup) const;

		/** Creates a player's context. */
		virtual SoundPlayContext* createPlayContext() = 0;

	protected:
		/** Returns file extension required by this player (with leading dot). 
		Only the default implementation of the "canPlay" function calls
		this function. */
		virtual const String& getFileExt() const;

		/** Returns true if the player can play 2D sound.
		Only the default implementation of the "canPlay" function calls
		this function. */
		virtual bool canPlay2D() const;

		/** Returns true if the player can play 3D sound.
		Only the default implementation of the "canPlay" function calls
		this function. */
		virtual bool canPlay3D() const;

	private:
		String                  mName;
	};



	//-----------------------------------------------------------------------
	/** An internal class which is used to help to declare class ResourceManagerEx. 
	Contains a list of all resource loaders which can be used
	to load a resource of the same specific type. */
	class GOTHOGRE_EXPORT SoundPlayerEnumerator : public Singleton<SoundPlayerEnumerator>
	{
	public:
		/** Constructor */
		SoundPlayerEnumerator();

		/** Destructor */
		~SoundPlayerEnumerator();

		/** Returns list of all registered resource loaders. */
		const StringVector& getAllSoundPlayers() const;

		/** Whether a specified resource can be played. */
		bool canPlay(bool _3D, const String& _filename, const String& _resourceGroup) const;

		/** Starts playing a specified sound. 
		The function calls the following functions: 
		SoundPlayer::canPlay, 
		then SoundPlayer::initialise (if the sound player is not initialised yet),
		then SoundPlayer::createPlayContext,
		then SoundPlayContext::play. */
		SoundPlayContextPtr play(bool _3D, const String& _filename, const String& _resourceGroup);

	private:
		friend class SoundSystem;

		/** Updates all initialised players and all playing contexts. 
		This function must be called from time to time. */
		void updateAll(const Vector3& _listenerPosition, 
			const Quaternion& _listenerOrientation);

	private:
		friend class SoundPlayer;

		/** Registers a sound player in this enumerator.
		The function is called from the constructor of the SoundPlayer class. */
		void addSoundPlayer(SoundPlayer* _soundPlayer, Priority _priority);

		/** Registers a sound player in this enumerator.
		The function also calls the SoundPlayer::shutdown function
		(if the sound player has been initialised before).
		The function is called from the destructor of the SoundPlayer class. */
		void removeSoundPlayer(SoundPlayer* _soundPlayer);

	private:
		SoundPlayer* findPlayerCanPlay(bool _3D, String& _filename, const String& _resourceGroup) const;
		SoundPlayer* getPlayerByName(const String& _playerName) const;

	private:
		typedef map<String, SoundPlayer*>::type   PlayersByName;
		typedef multimap<Priority, SoundPlayer*>::type PlayersByPriority;
		typedef list<SoundPlayContextPtr>::type   Contexts;

		struct SoundPlayerInfo
		{
			bool       initialised;
			Contexts   activeContexts;
			Contexts   freeContexts;
		};

		typedef map<SoundPlayer*, SoundPlayerInfo>::type  PlayerInfosByPlayer;

		StringVector          mAllPlayerNames;
		PlayersByName         mPlayersByName;
		PlayersByPriority     mPlayersByPriority;
		PlayerInfosByPlayer   mPlayerInfosByPlayer;

		enum {SOUND_PLAYER_ENUMERATOR_NUM_REFERENCE_COUNTS = 1};
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_PLAYER_H