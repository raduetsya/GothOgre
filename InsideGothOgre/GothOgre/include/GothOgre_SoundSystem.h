#ifndef GOTHOGRE_SOUND_SYSTEM_H
#define GOTHOGRE_SOUND_SYSTEM_H

#include "GothOgre_EnumIO.h"
#include "GothOgre_Priority.h"
#include "GothOgre_FrameListener.h"
#include "GothOgre_BoundingSphereTree.h"
#include "GothOgre_SceneManagerEx.h"
#include "GothOgre_SoundDef.h"
#include "GothOgre_SoundPlayer.h"
#include "GothOgre_SoundSettings.h"


namespace GothOgre
{
	class Sound;


	//-------------------------------------------------------------------------
	/** Internal class.
	Common definition for the "Sound" and "SoundSystem" classes. */
	class SoundCommonDefs
	{
	public:
		typedef vector<Real>::type RealVector;

	protected:
		typedef BoundingSphereTree<Sound*>  BST;
		
		struct SoundGroup;

		struct SoundGroupEntry
		{
			Sound*                                 sound;
			SoundGroup*                            soundGroup;
			list<SoundGroupEntry>::type::iterator  iterator;
		};

		typedef list<SoundGroupEntry>::type   SoundGroupEntries;

		struct SoundGroup
		{
			bool                                     enabled;
			Real                                     volume;
			SoundGroupEntries                        entries;
			map<String, SoundGroup*>::type::iterator iterator;
		};

		typedef map<String, SoundGroup*>::type  SoundGroupsByName;

		struct SoundScene
		{
			String    name;
			bool      enabled;
			BST       bst;
		};

		typedef map<SceneManagerEx*, SoundScene*>::type SoundScenesBySM;

		struct SoundPlayInfo
		{
			Sound*                               sound;
			size_t								 soundDefStateCount;
			size_t                               worldTimeIndex;
			SoundPlayContextPtr                  playContext;
			bool                                 is3D;
			bool                                 enabled;
			bool                                 enumerated;
			bool                                 allowedToPlay;
			bool                                 oldAllowedToPlay;
			Real                                 volume;
			Real                                 volumeTimeCoef;
			Real                                 volumeFromSoundDef;
			Real                                 finalVolume;
			size_t                               numPlayed;
			Real                                 waitTime;
			list<SoundPlayInfo>::type::iterator  playInfoIterator;
			list<String>::type::iterator         filenameIterator;
		};

		typedef list<SoundPlayInfo>::type  SoundPlayInfos;
	};



	//-------------------------------------------------------------------------
	/** Loop mode */
	struct GOTHOGRE_EXPORT SoundLoopMode
	{
		enum Enum
		{
			/** Play the sound once, when the listener 
			enters the sound's area at the first time. 
			The sound is never repeated. */
			PLAY_ONCE,

			/** Play the sound once, when the listener 
			enters the sound's area. If the listener will leave later,
			and enters it again, the sound will been repeated. */
			PLAY_ONCE_PER_ENTER,

			/** Play the sound repeately, in infinite loop. */
			INFINITE_LOOP,

			/** Play the sound repeately, with a random delay 
			(see Sound::setMinDelay and Sound::setMaxDelay)	*/
			INFINITE_LOOP_WITH_DELAY
		};

		SoundLoopMode(int _value = PLAY_ONCE) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
		GOTHOGRE_DECLARE_ENUM_IO(SoundLoopMode)
	private:
		Enum mValue;
	};



	//-------------------------------------------------------------------------
	/** Movable object to play sound. 
	This object designates a sound source, and can be attached to a scene node. */
	class GOTHOGRE_EXPORT Sound : public MovableObject, public SoundCommonDefs
	{
	public:
        /** Constructor */
        Sound();

		/** Named constructor */
		Sound(const String& _name);

		/** Virtual destructor */
		~Sound();

        /** Returns the type name of this object. */
        const String& getMovableType() const;

		/** Get the manager of this object. */
		SceneManagerEx* _getManager() const;

		/** Sets the filename of the sound resource linked to the object. */
		void setSound(bool _3D, const SoundDefPtr& _soundDef);

		/** Sets the filename of the sound resource linked to the object. (Simple version.) */
		void setSound2D(const SoundDefPtr& _soundDef);

		/** Sets the filename of the sound resource linked to the object. (Simple version.) */
		void setSound3D(const SoundDefPtr& _soundDef);

		/** Is the sound a 3D sound? 
		The volume of a 3D sound depends on the listener's position
		and orientation, and the sound's position. 
		The volume of a 2D sound doesn't depend on these values. 
		So, for example, background music is 2D. */
		bool is3D() const;

		/** Returns the filename of the sound. */
		const SoundDefPtr& getSoundDef() const;

		/** Enables or disables playing of the sound.
		The default value is true, i.e. sounds are enabled by default. */
		void setEnabled(bool _enable);

		/** Is the playing enabled? */
		bool isEnabled() const;


		/** Returns true if the sound is playing now. 
		The returned value may be used for information only. */
		bool isPlaying() const;

		/** Returns the length of the sound track, in seconds. */
		Real getPlayLength() const;

		/** Returns the playback position, in seconds. */
		Real getPlayPosition() const;


		/** Sets the priority of the sound. 
		The priority is used only when it's impossible to play all sounds,
		and it's necessary to select sounds to be played. 
		See SoundSystem::getMaxPlayingSounds for more information. 
		The default priority is 0. */
		void setPriority(Priority _priority);

		/** Returns the priority of the sound. */
		Priority getPriority() const;

		/** Sets the group of the sound.
		Groups are useful to change volume of the entire group easily,
		or stop playing of all sounds of the same group, and so on. 
		The default group is "" (empty string), i.e.
		a sound is not belong to any sound group by default. */
		void setSoundGroup(const String& _soundGroup);

		/** Returns the group of the sound. */
		const String& getSoundGroup() const;

		/** Sets the attenuation of the sound, in range [0..1]. 
		0 is silence, and 1 is the max volume.
		The default volume is 1, i.e. the maximum volume. */
		void setVolume(Real _volume);
		
		/** Returns the attenuation of the sound, in range [0..1]. */
		Real getVolume() const;


		/** Sets the loop mode of the sound. 
		The default loop mode is PLAY_ONCE, i.e. no loop. */
		void setLoopMode(SoundLoopMode _loopMode);

		/** Returns the loop mode of the sound. */
		SoundLoopMode getLoopMode() const;

		/** Resets the number of the sound's playing to zero.
		It can be useful to make a sound with loop mode "PLAY_ONCE_?"
		to be played again after it has been played. */
		void resetNumPlayed();


		/** Sets a minimal time for random delay, in seconds.
		The second parameter is multiplied by the length of the sound track (in seconds),
		and the product is added to the minimal delay.
		This value is only used if getLoopMode() == INFINITE_LOOP_WITH_DELAY. */
		void setMinDelay(Real _minDelay, Real _lengthMultiplier = 0.0f);

		/** Sets a maximal time for random delay, in seconds. 
		The second parameter is multiplied by the length of the sound track (in seconds),
		and the product is added to the maximal delay.
		This value is only used if getLoopMode() == INFINITE_LOOP_WITH_DELAY. */
		void setMaxDelay(Real _maxDelay, Real _lengthMultiplier = 0.0f);

		/** Returns a minimal time for random delay, in seconds. */
		Real getMinDelay() const;

		/** Returns the length multiplier for minimal delay. */
		Real getMinDelayLengthMultiplier() const;

		/** Returns a maximum time for random delay, in seconds. */
		Real getMaxDelay() const;

		/** Returns the length multiplier for minimal delay. */
		Real getMaxDelayLengthMultiplier() const;

		/** Sets whether the instance of the Sound class should be
		destroyed when the sound track is finished. 
		The default is false, i.e. keep the instance undestroyed. */
		void setDestroyOnEnd(bool _destroyOnEnd);

	public: // 3D sound:
		/** Sets the maximum distance, which is the distance from the listener 
		beyond which sound is no longer attenuated. 
		Only for 3D sounds. */
		void setSoundMaxDistance(Real _radius);

		/** Sets the maximum distance, which is the distance from the listener 
		beyond which sound is no longer attenuated. 
		Only for 3D sounds. */
		Real getSoundMaxDistance() const;

	public: // 2D sound:
		/** Sets a bounding box beyond which sound is no longer played.
		The sound is not attenuated while it's inside the bounding box.
		(Only for 2D sounds). */
		void setSoundBoundingBox(const AxisAlignedBox& _box, bool _localSpace = true);

		/** Returns a bounding box beyond which sound is no longer played.
		(Only for 2D sounds). */
		const AxisAlignedBox& getSoundBoundingBox() const;

		/** Returns true if a bounding box specified for local space. */
		bool isSoundBoundingBoxInLocalSpace() const;

		/** Sets the time in the course of which the sound should be played
		(with attenuation) after the listener has left the bounding box 
		set for the sound (see Sound::setSoundBoundingBox). 
		In another words, the sound doesn't stop immediately when the listener
		leaves the sound's bounding box, it's attenuated smoothly. 
		The default value is zero.
		(Only for 2D sounds). */
		void setOutVolumeSmoothingTime(Real _time);

		/** Returns the time in the cource of which the sound should be played
		(with attenuation) after the listener has left the bounding box 
		set for the sound (see Sound::setSoundBoundingBox).
		(Only for 2D sounds). */
		Real getOutVolumeSmoothingTime() const;

		/** Sets the time in the course of which the sound's volume
		should be increased from silence to the max volume after
		the listener has come to the bounding box 
		set for the sound (see Sound::setSoundBoundingBox). 
		The default value is zero.
		(Only for 2D sounds). */
		void setInVolumeSmoothingTime(Real _time);

		/** Returns the time in the course of which the sound's volume
		should be increased from silence to the max volume after
		the listener has come to the bounding box 
		set for the sound (see Sound::setSoundBoundingBox).
		(Only for 2D sounds). */
		Real getInVolumeSmoothingTime() const;

	protected:
		// Called to notify the object that it has been moved.
		void _notifyMoved();

		// Sounds are not rendered in the normal way, so we return 0
		Real getBoundingRadius() const {return 0;}
		const AxisAlignedBox& getBoundingBox() const {return AxisAlignedBox::BOX_NULL;}
		bool isVisible() const {return false;}
		void _updateRenderQueue(RenderQueue* _queue) {}
		void visitRenderables(Renderable::Visitor* _visitor, bool _debugRenderables = false) {}

	private:
		void init();
		void _notifyManager(SceneManager* _sceneMgr);

	private:
		friend class SoundSystem;

		bool                 m3D;
		SoundDefPtr          mSoundDef;
		Real                 mVolume;
		Real                 mSoundMaxDistance;
		AxisAlignedBox       mSoundBoundingBox;
		bool                 mSoundBoundingBoxInLocalSpace;
		bool                 mEnabled;
		Real                 mInVolumeSmoothingTime;
		Real                 mOutVolumeSmoothingTime;
		SoundLoopMode        mLoopMode;
		Real                 mMinDelay;
		Real                 mMinDelayLengthMultiplier;
		Real                 mMaxDelay;
		Real                 mMaxDelayLengthMultiplier;
		size_t               mNumPlayed;
		bool                 mDestroyOnEnd;
		Priority             mPriority;
		BST::ElementHandle   mBstElemHandle;
		SoundScene*          mSoundScene;
		SoundGroupEntry*     mSoundGroupEntry;
		SoundPlayInfo*       mSoundPlayInfo;
	};



	//-------------------------------------------------------------------------
	/** This is the main class to operate with sound. 
	The class is a factory which can create instances of the Sound class
	which are separate sound sources.
	The class also provides support for "sound groups".
	The sound group is a group of sounds, all sounds in the same groups
	can be changed at the same time. */
	class GOTHOGRE_EXPORT SoundSystem : public Singleton<SoundSystem>,
		public SoundCommonDefs, public MovableObjectFactory, 
		protected BoundingSphereTreeListener
	{
	public:
		SoundSystem();
		~SoundSystem();

		void initialise();
		void shutdown();

		/** Get the type of the object to be created. */
		const String& getType() const;
		static const String FACTORY_TYPE_NAME;

		/** Creates a sound (with an autogenerated name). */
		Sound* createSound(SceneManagerEx* _sceneManager);

		/** Destroys a sound. */
		void destroySound(Sound* _sound);

		/** Sets the listener. The listener is an object which can "listen" 
		sounds. For example, the listener can be camera. */
		void setSoundListener(Camera* _camera);

		/** Returns the listener. */
		Camera* getSoundListener() const;


		/** Sets a factor of volumes of a specified sound group. 
		Volume of each sound of the group is multiplied on this factor. 
		The result is not accumulated, i.e. several calls
		of SoundSystem::setGroupVolume don't use previous values. */
		void setGroupVolume(const String& _soundGroup, Real _volume);

		/** Returns a factor of volumes a specified sound group. */
		Real getGroupVolume(const String& _soundGroup) const;

		/** Enabled/disables playing all sounds in a specified sound group. */
		void setGroupEnabled(const String& _soundGroup, bool _enable);

		void enableGroup(const String& _soundGroup) {setGroupEnabled(_soundGroup, true);}
		void disableGroup(const String& _soundGroup) {setGroupEnabled(_soundGroup, false);}

		/** Is a specified sound group enabled? */
		bool isGroupEnabled(const String& _soundGroup) const;


		typedef list<String>::type PlayingSoundList;
		typedef ConstVectorIterator<PlayingSoundList> PlayingSoundIterator;

		/** Returns the list of sounds which are playing just now. 
		The returned value may be used for information only. */
		PlayingSoundIterator getPlayingSoundIterator(bool _3D) const;

	private:
		GOTHOGRE_DECLARE_FRAME_RENDERING_QUEUED( SoundSystem );

		void enumerateSphereIntersects( BST::ElementHandle _handle, Real _distance, void* _param );
		void enumerateAll(BST::ElementHandle _handle, void* _param);

		MovableObject* createInstanceImpl( const String& _name, const NameValuePairList* _params = 0 );
		void destroyInstance(MovableObject* _obj);

	private:
		friend class Sound;

		/** Returns name of sound group a specified sound belong to. */
		const String& getSoundGroupName(const Sound* _sound);

		/** Changes a sound group a specified sound belong to. */
		void changeSoundGroup(Sound* _sound, const String& _soundGroupName);
		
		/** Removes a sound from its sound group. */
		void removeFromSoundGroup(Sound* _sound);
		
		/** Returns a sound group by name.
		This function returns nullptr if not found. */
		const SoundGroup* findSoundGroup(const String& _soundGroupName) const;

		/** Returns a sound group by name.
		This function creates a new group if not found. */
		SoundGroup* getSoundGroup(const String& _soundGroupName);

		/** Returns an information about sounds in a scene.
		This function returns nullptr if not found. */
		const SoundScene* findSoundScene(SceneManagerEx* _sceneManager) const;

		/** Returns an information about sounds in a scene.
		This function creates a new instance of SoundScene if not found. */
		SoundScene* getSoundScene(SceneManagerEx* _sceneManager);

		/** Updates the current sound scene, i.e.
		searches listenable sounds, recalculates their volumes, and so on. */
		void updateCurrentSoundScene(const Vector3& _listenerPosition, const Quaternion& _listenerOrientation, Real _timeSinceLastUpdate);

		/** Checks number of sounds which are allowed to play,
		and, if the number is too many, disallows sounds 
		with minimal priority. */
		void restrictNumAllowedToPlay();

		/** Process a sound which area has been visited by the listener. */
		void processPlayAllowance(SoundPlayInfo* _cp, bool _justAllowed);

		/** Process a sound which area has been left by the listener. */
		void processPlayDisallowance(SoundPlayInfo* _cp, bool _justDisallowed);

		/** Starts the playing of the sound. */
		void play(SoundPlayInfo* _cp);

		/** Stops the playing of the sound. */
		void stop(SoundPlayInfo* _cp);

		/** Checks is the sound track is just finished. */
		bool isSoundEnded(SoundPlayInfo* _cp);

		/** Processes the ending of the sound track,
		i.e. loops it back if it's specified. */
		void processSoundEnd(SoundPlayInfo* _cp);

		/** Calculate parameters to increase the volume smoothly
		when the sound is just started. */
		void calculateVolumeSmoothingFactorIn(SoundPlayInfo* _cp);

		/** Calculate parameters to descrease the volume smoothly
		when the sound is almost ended. */
		void calculateVolumeSmoothingFactorOut(SoundPlayInfo* _cp);

		/** Calculate the final value for the sound volume,
		taking into account smoothing parameters and volume of the sound group. */
		void calculateVolume(SoundPlayInfo* _cp, Real _time);
		
		/** Removes an item from the list of currently playing sounds. */
		void removeSoundPlayInfo(SoundPlayInfo* _cp, bool _allowDestroySound = true);
		
		/** Removes an item from the list of currently playing sounds. */
		void removeSoundPlayInfo(Sound* _sound, bool _allowDestroySound = true);

		/** Inserts a sound to the bounding sphere tree. */
		void insertToBst(Sound* _sound);

		/** Removes a sound from the bounding sphere tree. */
		void removeFromBst(Sound* _sound);

		/** Writes changes in sound's radius to the bounding sphere tree. */
		void updateBstRadius(Sound* _sound);

		/** Writes changes in sound's position to the bounding sphere tree. */
		void updateBstCenter(Sound* _sound);

		/** Calculate center of a sound. */
		static Vector3 calculateSoundCenter(Sound* _sound);

		/** Calculate radius of a sound. */
		static Real calculateSoundRadius(Sound* _sound);

		GOTHOGRE_DECLARE_SOUND_SETTINGS_CHANGED( SoundSystem );

	private:
		Real                 mPadding;
		BST::RealList        mBSTMaxRadiusList;
		Camera*              mSoundListener;
		SoundGroupsByName    mSoundGroupsByName;
		SoundScenesBySM      mSoundScenesBySM;
		SoundScene*          mCurrentSoundScene;
		SoundPlayInfos       mSoundPlayInfos;
		size_t               mMaxPlayingSounds[2];
		PlayingSoundList     mPlayingSounds[2];

		typedef set<String>::type StringSet;
		StringSet            mErrorMessages;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_SYSTEM_H