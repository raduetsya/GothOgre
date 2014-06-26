#include "GothOgre_Precompiled.h"
#include "GothOgre_ResourceGroup.h"
#include "GothOgre_SceneQueryMask.h"
#include "GothOgre_SoundSystem.h"
#include "GothOgre_SoundSettings.h"
#include "GothOgre_WorldTime.h"
#include "GothOgre_Interpolation.h"
#include "GothOgre_StdIO.h"


//-------------------------------------------------------------------------
GothOgre::SoundSystem* 
	Ogre::Singleton<GothOgre::SoundSystem>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// SoundLoopMode
	//-------------------------------------------------------------------------
	GOTHOGRE_IMPLEMENT_ENUM_IO(SoundLoopMode)
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT( PLAY_ONCE )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT( PLAY_ONCE_PER_ENTER )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT( INFINITE_LOOP )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT( INFINITE_LOOP_WITH_DELAY )
	GOTHOGRE_IMPLEMENT_ENUM_IO_END




	//-------------------------------------------------------------------------
	// Sound
	//-------------------------------------------------------------------------
	Sound::Sound()
	{
		init();
	}
	//-------------------------------------------------------------------------
	Sound::Sound(const String& _name)
		: MovableObject(_name)
	{
		init();
	}
	//-------------------------------------------------------------------------
	void Sound::init()
	{
		mBstElemHandle = BST::NULL_HANDLE;
		m3D = false;
		mVolume = 1;
		mEnabled = true;
		mSoundMaxDistance = Math::POS_INFINITY;
		mSoundBoundingBox.setInfinite();
		mSoundBoundingBoxInLocalSpace = true;
		mInVolumeSmoothingTime = 0;
		mOutVolumeSmoothingTime = 0;
		mNumPlayed = 0;
		mPriority = Priority::LOWEST;
		mDestroyOnEnd = false;
		mLoopMode = SoundLoopMode::PLAY_ONCE;
		mMinDelay = 0;
		mMinDelayLengthMultiplier = 0;
		mMaxDelay = 0;
		mMaxDelayLengthMultiplier = 0;
		mSoundGroupEntry = nullptr;
		mSoundPlayInfo = nullptr;
		mSoundScene = nullptr;

		setCastShadows(false);
		setQueryFlags(0);
		setVisibilityFlags(0);
	}
	//-------------------------------------------------------------------------
	Sound::~Sound()
	{
		SoundSystem::getSingleton().removeSoundPlayInfo(this, false);
		SoundSystem::getSingleton().removeFromBst(this);
		SoundSystem::getSingleton().removeFromSoundGroup(this);
	}
	//-------------------------------------------------------------------------
	void Sound::_notifyManager(SceneManager* _sceneMgr)
	{
		MovableObject::_notifyManager(_sceneMgr);
		mSoundScene = SoundSystem::getSingleton().getSoundScene( (SceneManagerEx*) _sceneMgr );
		SoundSystem::getSingleton().insertToBst(this);
	}
	//-------------------------------------------------------------------------
	const String& Sound::getMovableType() const
	{
		return SoundSystem::FACTORY_TYPE_NAME;
	}
	//---------------------------------------------------------------------------
	SceneManagerEx* Sound::_getManager() const 
	{
		return (SceneManagerEx*) MovableObject::_getManager();
	}
	//-------------------------------------------------------------------------
	void Sound::setSound(bool _3D, const SoundDefPtr& _soundDef)
	{
		// We need reset playing of the sound if it's playing
		if(mSoundPlayInfo)
			mSoundPlayInfo->enabled = false;

		m3D = _3D;
		mSoundDef = _soundDef;
		
		// The new sound requires new statistics
		resetNumPlayed();
	}
	//-------------------------------------------------------------------------
	void Sound::resetNumPlayed()
	{
		mNumPlayed = 0;
		if(mSoundPlayInfo)
			mSoundPlayInfo->numPlayed = 0;
	}
	//-------------------------------------------------------------------------
	void Sound::setSound2D(const SoundDefPtr& _soundDef)
	{
		setSound(false, _soundDef);
	}
	//-------------------------------------------------------------------------
	void Sound::setSound3D(const SoundDefPtr& _soundDef)
	{
		setSound(true, _soundDef);
	}
	//-------------------------------------------------------------------------
	bool Sound::is3D() const
	{
		return m3D;
	}
	//-------------------------------------------------------------------------
	const SoundDefPtr& Sound::getSoundDef() const
	{
		return mSoundDef;
	}
	//-------------------------------------------------------------------------
	void Sound::setPriority(Priority _priority)
	{
		mPriority = _priority;
	}
	//-------------------------------------------------------------------------
	Priority Sound::getPriority() const
	{
		return mPriority;
	}
	//-------------------------------------------------------------------------
	bool Sound::isPlaying() const
	{
		return mSoundPlayInfo && !mSoundPlayInfo->playContext.isNull();
	}
	//-------------------------------------------------------------------------
	Real Sound::getPlayLength() const
	{
		if(mSoundPlayInfo && !mSoundPlayInfo->playContext.isNull())
			return mSoundPlayInfo->playContext->getPlayLength();
		else
			return 0;
	}
	//-------------------------------------------------------------------------
	Real Sound::getPlayPosition() const
	{
		if(mSoundPlayInfo && !mSoundPlayInfo->playContext.isNull())
			return mSoundPlayInfo->playContext->getPlayPosition();
		else
			return 0;
	}
	//-------------------------------------------------------------------------
	void Sound::setEnabled(bool _enable)
	{
		mEnabled = _enable;
	}
	//-------------------------------------------------------------------------
	bool Sound::isEnabled() const
	{
		return mEnabled;
	}
	//-------------------------------------------------------------------------
	void Sound::setSoundGroup(const String& _soundGroup)
	{
		SoundSystem::getSingleton().changeSoundGroup(this, _soundGroup);
	}
	//-------------------------------------------------------------------------
	const String& Sound::getSoundGroup() const
	{
		return SoundSystem::getSingleton().getSoundGroupName(this);
	}
	//-------------------------------------------------------------------------
	void Sound::setVolume(Real _volume)
	{
		mVolume = _volume;
		// The new volume will send to the player at the next frame
		// by the SoundSystem::frameRenderingQueued function.
	}
	//-------------------------------------------------------------------------
	Real Sound::getVolume() const
	{
		return mVolume;
	}
	//-------------------------------------------------------------------------
	void Sound::setLoopMode(SoundLoopMode _loopMode)
	{
		mLoopMode = _loopMode;

		if(mSoundPlayInfo && !mSoundPlayInfo->playContext.isNull())
			mSoundPlayInfo->playContext->setLooping(_loopMode == SoundLoopMode::INFINITE_LOOP);
	}
	//-------------------------------------------------------------------------
	SoundLoopMode Sound::getLoopMode() const
	{
		return mLoopMode;
	}
	//-------------------------------------------------------------------------
	void Sound::setMinDelay(Real _minDelay, Real _lengthMultiplier)
	{
		mMinDelay = _minDelay;
		mMinDelayLengthMultiplier = _lengthMultiplier;
	}
	//-------------------------------------------------------------------------
	Real Sound::getMinDelay() const
	{
		return mMinDelay;
	}
	//-------------------------------------------------------------------------
	Real Sound::getMinDelayLengthMultiplier() const
	{
		return mMinDelayLengthMultiplier;
	}
	//-------------------------------------------------------------------------
	void Sound::setMaxDelay(Real _maxDelay, Real _lengthMultiplier)
	{
		mMaxDelay = _maxDelay;
		mMaxDelayLengthMultiplier = _lengthMultiplier;
	}
	//-------------------------------------------------------------------------
	Real Sound::getMaxDelay() const
	{
		return mMaxDelay;
	}
	//-------------------------------------------------------------------------
	Real Sound::getMaxDelayLengthMultiplier() const
	{
		return mMaxDelayLengthMultiplier;
	}
	//-------------------------------------------------------------------------
	void Sound::setSoundMaxDistance(Real _radius)
	{
		mSoundMaxDistance = _radius;

		// Send the new radius to the bounding sphere tree in the SoundSystem.
		SoundSystem::getSingleton().updateBstRadius(this);
		
		// Send the new radius to the player.
		if(m3D && _radius != Math::POS_INFINITY && mSoundPlayInfo && !mSoundPlayInfo->playContext.isNull())
			mSoundPlayInfo->playContext->setMaxDistance(_radius);
	}
	//-------------------------------------------------------------------------
	Real Sound::getSoundMaxDistance() const
	{
		return mSoundMaxDistance;
	}
	//-------------------------------------------------------------------------
	void Sound::setSoundBoundingBox(const AxisAlignedBox& _box, bool _localSpace)
	{
		mSoundBoundingBox = _box;
		mSoundBoundingBoxInLocalSpace = _localSpace;

		// Send the new radius to the bounding sphere tree in the SoundSystem.
		SoundSystem::getSingleton().updateBstRadius(this);
	}
	//-------------------------------------------------------------------------
	const AxisAlignedBox& Sound::getSoundBoundingBox() const
	{
		return mSoundBoundingBox;
	}
	//-------------------------------------------------------------------------
	bool Sound::isSoundBoundingBoxInLocalSpace() const
	{
		return mSoundBoundingBoxInLocalSpace;
	}
	//-------------------------------------------------------------------------
	void Sound::setOutVolumeSmoothingTime(Real _time)
	{
		mOutVolumeSmoothingTime = _time;
	}
	//-------------------------------------------------------------------------
	Real Sound::getOutVolumeSmoothingTime() const
	{
		return mOutVolumeSmoothingTime;
	}
	//-------------------------------------------------------------------------
	void Sound::setInVolumeSmoothingTime(Real _time)
	{
		mInVolumeSmoothingTime = _time;
	}
	//-------------------------------------------------------------------------
	Real Sound::getInVolumeSmoothingTime() const
	{
		return mInVolumeSmoothingTime;
	}
	//-------------------------------------------------------------------------
	void Sound::setDestroyOnEnd(bool _destroyOnEnd)
	{
		mDestroyOnEnd = _destroyOnEnd;
	}
	//-------------------------------------------------------------------------
	void Sound::_notifyMoved()
	{
		MovableObject::_notifyMoved();
		SoundSystem::getSingleton().updateBstCenter(this);
		if(mSoundPlayInfo && !mSoundPlayInfo->playContext.isNull()
			&& m3D)
		{
			mSoundPlayInfo->playContext->setPosition(
				getParentNode()->_getDerivedPosition());
		}		
	}



	

	//-------------------------------------------------------------------------
	// SoundSystem
	//-------------------------------------------------------------------------
	SoundSystem::SoundSystem()
		: mSoundListener(nullptr),
		  mCurrentSoundScene(nullptr)
	{
	}
	//-------------------------------------------------------------------------
	SoundSystem::~SoundSystem()
	{
	}
	//-------------------------------------------------------------------------
	void SoundSystem::initialise()
	{
		// Register movable object factory
		Root::getSingleton().addMovableObjectFactory(this);

		// Register as listener
		registerSoundSettingsListener(); // read sound settings
		registerFrameListener(); // SoundSystem need be updated every frame
	}
	//-------------------------------------------------------------------------
	void SoundSystem::shutdown()
	{
		// Stop and destroy all sounds
		for(SoundScenesBySM::iterator it = mSoundScenesBySM.begin();
			it != mSoundScenesBySM.end(); ++it)
		{	
			mCurrentSoundScene = it->second;
			mCurrentSoundScene->bst.findAll(this);
		}
		GOTHOGRE_ASSERT(mSoundPlayInfos.empty(), "Cannot shutdown SoundSystem, because some sounds are being playing.");

		// Delete all the sound scenes
		for(SoundScenesBySM::iterator it2 = mSoundScenesBySM.begin();
			it2 != mSoundScenesBySM.end(); ++it2)
		{
			SoundScene* soundScene = it2->second;
			soundScene->bst.logStatistics("SoundSystem BST (" + soundScene->name + ")");
			delete soundScene;
		}
		mSoundScenesBySM.clear();
		mCurrentSoundScene = nullptr;

		// Delete all the sound groups
		for(SoundGroupsByName::iterator it3 = mSoundGroupsByName.begin();
			it3 != mSoundGroupsByName.end(); ++it3)
		{
			SoundGroup* soundGroup = it3->second;
			delete soundGroup;
		}
		mSoundGroupsByName.clear();
		mSoundListener = nullptr;

		// Unregister movable object factory
		if(Root::getSingletonPtr())
			Root::getSingleton().removeMovableObjectFactory(this);

		// Unregister as listener
		unregisterFrameListener();
		unregisterSoundSettingsListener();
	}
	//-------------------------------------------------------------------------
	void SoundSystem::enumerateAll(BST::ElementHandle _handle, void* _param)
	{
		// Destroy sound
		Sound* sound = mCurrentSoundScene->bst.getData(_handle);
		sound->_getManager()->destroyMovableObject(sound);
	}
	//-------------------------------------------------------------------------
	MovableObject* SoundSystem::createInstanceImpl(const String& _name, const NameValuePairList* _params)
	{
		return OGRE_NEW Sound(_name);
	}
	//-------------------------------------------------------------------------
	void SoundSystem::destroyInstance(MovableObject* _obj)
	{
		OGRE_DELETE _obj;
	}
	//-------------------------------------------------------------------------
	const String SoundSystem::FACTORY_TYPE_NAME = "Sound";
	//-------------------------------------------------------------------------
	const String& SoundSystem::getType() const
	{
		return FACTORY_TYPE_NAME;
	}
	//-------------------------------------------------------------------------
	Sound* SoundSystem::createSound(SceneManagerEx* _sceneManager)
	{
		return (Sound*) _sceneManager->createMovableObject(FACTORY_TYPE_NAME);
	}
	//-------------------------------------------------------------------------
	void SoundSystem::destroySound(Sound* _sound)
	{
		_sound->_getManager()->destroyMovableObject(_sound);
	}
	//-------------------------------------------------------------------------
	void SoundSystem::setSoundListener(Camera* _newSoundListener)
	{
		if(mSoundListener != _newSoundListener)
		{
			SceneManagerEx* sceneManager = mSoundListener ? ((SceneManagerEx*) mSoundListener->getSceneManager()) : nullptr;
			SceneManagerEx* newSceneManager = _newSoundListener ? ((SceneManagerEx*) _newSoundListener->getSceneManager()) : nullptr;
			if(sceneManager != newSceneManager)
			{
				if(sceneManager)
				{
					SoundScene* soundScene = getSoundScene(sceneManager);
					soundScene->enabled = false;
					while(!mSoundPlayInfos.empty())
					{
						Sound* sound = mSoundPlayInfos.front().sound;
						removeSoundPlayInfo(sound);
					}
				}
				if(newSceneManager)
				{
					SoundScene* newSoundScene = getSoundScene(newSceneManager);
					newSoundScene->enabled = true;
					mCurrentSoundScene = newSoundScene;
				}
			}
			mSoundListener = _newSoundListener;
		}
	}
	//-------------------------------------------------------------------------
	Camera* SoundSystem::getSoundListener() const
	{
		return mSoundListener;
	}
	//-------------------------------------------------------------------------
	const String& SoundSystem::getSoundGroupName(const Sound* _sound)
	{
		if(!_sound->mSoundGroupEntry)
			return StringUtil::BLANK;

		SoundGroup* soundGroup = _sound->mSoundGroupEntry->soundGroup;
		const String& soundGroupName = soundGroup->iterator->first;
		return soundGroupName;
	}
	//-------------------------------------------------------------------------
	void SoundSystem::changeSoundGroup(Sound* _sound, const String& _soundGroupName)
	{
		removeFromSoundGroup(_sound);

		SoundGroup* soundGroup = getSoundGroup(_soundGroupName);
		soundGroup->entries.push_front(SoundGroupEntry());
		SoundGroupEntry* entry = &(soundGroup->entries.front());
		entry->iterator = soundGroup->entries.begin();
		entry->sound = _sound;
		entry->soundGroup = soundGroup;
		_sound->mSoundGroupEntry = entry;
	}
	//-------------------------------------------------------------------------
	void SoundSystem::removeFromSoundGroup(Sound* _sound)
	{
		SoundGroupEntry* entry = _sound->mSoundGroupEntry;
		if(entry)
		{
			SoundGroup* soundGroup = entry->soundGroup;
			soundGroup->entries.erase(entry->iterator);
			_sound->mSoundGroupEntry = nullptr;
		}
	}
	//-------------------------------------------------------------------------
	SoundSystem::SoundGroup* SoundSystem::getSoundGroup(const String& _soundGroupName)
	{
		SoundGroup* soundGroup = (SoundGroup*) findSoundGroup(_soundGroupName);
		if(!soundGroup)
		{
			SoundGroupsByName::iterator it = mSoundGroupsByName.insert(std::make_pair(_soundGroupName, new SoundGroup)).first;
			soundGroup = it->second;
			soundGroup->iterator = it;
			soundGroup->volume = SoundSettings::getSingleton().getSetting(_soundGroupName, "Volumes", 1.0f);
			soundGroup->enabled = true;
		}
		return soundGroup;
	}
	//-------------------------------------------------------------------------
	const SoundSystem::SoundGroup* SoundSystem::findSoundGroup(const String& _soundGroupName) const
	{
		SoundGroupsByName::const_iterator it = mSoundGroupsByName.find(_soundGroupName);
		if(it != mSoundGroupsByName.end())
		{
			return it->second;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------------
	SoundSystem::SoundScene* SoundSystem::getSoundScene(SceneManagerEx* _sceneManager)
	{
		SoundScene* soundScene = (SoundScene*) findSoundScene(_sceneManager);
		if(!soundScene)
		{
			SoundScenesBySM::iterator it = mSoundScenesBySM.insert(std::make_pair(_sceneManager, new SoundScene)).first;
			soundScene = it->second;
			soundScene->enabled = true;
			soundScene->name = _sceneManager->getName();
			soundScene->bst.setMaxRadiusList(mBSTMaxRadiusList);
		}
		return soundScene;
	}
	//-------------------------------------------------------------------------
	const SoundSystem::SoundScene* SoundSystem::findSoundScene(SceneManagerEx* _sceneManager) const
	{
		SoundScenesBySM::const_iterator it = mSoundScenesBySM.find(_sceneManager);
		if(it != mSoundScenesBySM.end())
		{
			return it->second;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------------
	Real SoundSystem::getGroupVolume(const String& _soundGroupName) const
	{
		const SoundGroup* soundGroup = findSoundGroup(_soundGroupName);
		if(soundGroup)
			return soundGroup->volume;
		else
			return SoundSettings::getSingleton().getSetting(_soundGroupName, "Volumes", 1.0f);
	}
	//-------------------------------------------------------------------------
	void SoundSystem::setGroupVolume(const String& _soundGroup, Real _volume)
	{
		SoundSettings::getSingleton().setSetting(_soundGroup, "Volumes", _volume);
	}
	//-------------------------------------------------------------------------
	void SoundSystem::setGroupEnabled(const String& _soundGroup, bool _enable)
	{
		SoundGroup* soundGroup = getSoundGroup(_soundGroup);
		soundGroup->enabled = _enable;
	}
	//-------------------------------------------------------------------------
	bool SoundSystem::isGroupEnabled(const String& _soundGroup) const
	{
		const SoundGroup* soundGroup = findSoundGroup(_soundGroup);
		return soundGroup ? soundGroup->enabled : true;
	}
	//-------------------------------------------------------------------------
	SoundSystem::PlayingSoundIterator SoundSystem::getPlayingSoundIterator(bool _3D) const
	{
		return (SoundSystem::PlayingSoundIterator) mPlayingSounds[_3D];
	}
	//-------------------------------------------------------------------------
	void SoundSystem::frameRenderingQueued(const FrameEvent& _evt)
	{
		// Get the listener's position
		Vector3 listenerPosition = Vector3::ZERO;
		Quaternion listenerOrientation = Quaternion::IDENTITY;
		if(mSoundListener)
		{
			listenerPosition = mSoundListener->getPositionForViewUpdate();
			listenerOrientation = mSoundListener->getOrientationForViewUpdate();
		}

		if(mCurrentSoundScene)
			updateCurrentSoundScene(listenerPosition, listenerOrientation, _evt.timeSinceLastEvent);

		// Updates all initialised players and all playing contexts.
		SoundPlayerEnumerator::getSingleton().updateAll(listenerPosition, listenerOrientation);
	}
	//------------------------------------------------------------------------------
	void SoundSystem::updateCurrentSoundScene(const Vector3& _listenerPosition, const Quaternion& _listenerOrientation, Real _timeSinceLastUpdate)
	{
		// Suppose all the sounds' spheres are outside the listener's position
		SoundPlayInfos::iterator it;
		for(it = mSoundPlayInfos.begin(); it != mSoundPlayInfos.end(); ++it)
		{
			SoundPlayInfo* cp = &*it;
			cp->oldAllowedToPlay = cp->allowedToPlay;
			cp->allowedToPlay = false; // the enumerateSphereIntersects function
			cp->enumerated = false;    // will set them to true for all sounds which can be listen
		}

		// Search sounds' spheres which are enabled and contain the listener's position;
		// i.e. search sounds which can be listened
		mCurrentSoundScene->bst.findSphereIntersects(
			Sphere(_listenerPosition, mPadding), 
			this, 
			const_cast<Vector3*>(&_listenerPosition));

		// Checks number of sounds which are allowed to play,
		// and, if the number is too many, disallows sounds 
		// with minimal priority.
		restrictNumAllowedToPlay();

		// For each sound near the listener...
		it = mSoundPlayInfos.begin(); 
		while(it != mSoundPlayInfos.end())
		{
			SoundPlayInfo* cp = &(*it++);
			Sound* sound = cp->sound;
			cp->waitTime -= _timeSinceLastUpdate;

			// Processing moving of the sound relative to the listener
			bool allowChanged = (cp->allowedToPlay != cp->oldAllowedToPlay);
			if(cp->allowedToPlay)
				processPlayAllowance(cp, allowChanged);
			else
				processPlayDisallowance(cp, allowChanged);

			// Recalculate the volume of the sound
			calculateVolume(cp, _timeSinceLastUpdate);

			// The end of the sound track could be reached, process
			if(isSoundEnded(cp))
				processSoundEnd(cp);

			// We can stop unlistenable sounds.
			if(cp->finalVolume == 0)
				stop(cp);

			// We can remove information about unlistenable sounds.
			if(cp->playContext.isNull() && !cp->allowedToPlay)
			{
				bool canRemovePI = false;
				if(!cp->enabled || !sound->mEnabled)
					canRemovePI = true;
				else if(!cp->enumerated)
					canRemovePI = true;
				else if(sound->mLoopMode == SoundLoopMode::PLAY_ONCE && sound->mNumPlayed != 0)
					canRemovePI = true;

				if(canRemovePI)
					removeSoundPlayInfo(cp);
			}
		}
	}
	//------------------------------------------------------------------------------
	void SoundSystem::enumerateSphereIntersects(BST::ElementHandle _handle, Real _distance, void* _param) 
	{
		Sound* sound = mCurrentSoundScene->bst.getData(_handle);
		SoundPlayInfo* cp = sound->mSoundPlayInfo;
		const SoundDefPtr& soundDef = sound->mSoundDef;

		if(cp)
			cp->enumerated = true;

		// We don't consider disabled sounds.
		if(!sound->mEnabled)
			return;

		if(!sound->mSoundScene->enabled)
			return;

		if(sound->mSoundGroupEntry)
		{
			if(!sound->mSoundGroupEntry->soundGroup->enabled)
				return;
		}

		// Need check the bounding box if it's not infinite.
		if(sound->mSoundBoundingBox.isFinite())
		{
			const Vector3& listenerWorldPosition = *(const Vector3*) _param;
			if(sound->mSoundBoundingBoxInLocalSpace && sound->getParentNode())
			{
				Vector3 listenerLocalPosition = sound->getParentNode()->convertWorldToLocalPosition(listenerWorldPosition);
				if(!sound->mSoundBoundingBox.contains(listenerLocalPosition))
					return;
			}
			else
			{
				if(!sound->mSoundBoundingBox.contains(listenerWorldPosition))
					return;
			}
		}
		else if(sound->mSoundBoundingBox.isNull())
			return;

		// The sound must be played once and it has been done already.
		if(sound->mNumPlayed != 0 && sound->mLoopMode == SoundLoopMode::PLAY_ONCE)
			return;

		// If nothing to play
		if(soundDef.isNull())
			return;

		if(!cp)
		{
			// The sound is playing for the first time, add it to the list.
			mSoundPlayInfos.push_front(SoundPlayInfo());
			cp = sound->mSoundPlayInfo = &mSoundPlayInfos.front();
			cp->playInfoIterator = mSoundPlayInfos.begin();
			cp->sound = sound;
			cp->soundDefStateCount = soundDef->getStateCount();
			cp->volumeFromSoundDef = 0;
			cp->worldTimeIndex = -1;
			cp->enabled = true;
			cp->oldAllowedToPlay = false;
			cp->allowedToPlay = false;
			cp->enumerated = true;
			cp->numPlayed = 0;
			cp->waitTime = 0;
			cp->volume = 0;
			cp->finalVolume = 0;
		}

		if(!cp->enabled)
			return;

		// The sound must be played once and it has been done already.
		if(cp->numPlayed != 0 && sound->mLoopMode == SoundLoopMode::PLAY_ONCE_PER_ENTER)
			return;

		// If the sound definition was changed, the playing must be restarted.
		if(cp->soundDefStateCount != sound->mSoundDef->getStateCount())
			return;

		// Process changing of the world time.
		Time curTime = WorldTime::getSingleton().getCurrentTime();
		Interpolation::FindClosestPointsResult<Time> 
			fcpr = Interpolation::findClosestPoints(
			curTime, soundDef->getTimePropertyIterator(),
			&SoundDef::TimeProperty::playingStartTime, 
			&SoundDef::TimeProperty::playingRepeatPeriod );

		if(!fcpr.success)
			return;

		size_t worldTimeIndex = fcpr.lowerBound;
		if(cp->worldTimeIndex == -1 && worldTimeIndex != -1)
		{
			cp->worldTimeIndex = worldTimeIndex;
			cp->volumeFromSoundDef = soundDef->getTimeProperty(worldTimeIndex).volume;
		}

		if(cp->worldTimeIndex != worldTimeIndex)
			return;

		// The sound is allowed to start/continue the playing.
		cp->allowedToPlay = true;
	}
	//------------------------------------------------------------------------------
	void SoundSystem::restrictNumAllowedToPlay()
	{
		// Calculate number of sounds which are allowed to play.
		size_t numAllowedToPlay[2] = {0, 0}; // 2D and 3D calculated separately
		SoundPlayInfos::iterator it = mSoundPlayInfos.begin(); 
		while(it != mSoundPlayInfos.end())
		{
			SoundPlayInfo* cp = &(*it++);
			if(cp->allowedToPlay)
			{
				Sound* sound = cp->sound;
				++numAllowedToPlay[sound->m3D];
			}
		}

		// Compare number of sounds which are allowed to play
		// with the maximum number of sounds which can be played simultaneosly.
		for(int u = 0; u != 2; ++u)
			if(numAllowedToPlay[u] > mMaxPlayingSounds[u])
			{
				// Too many allowed-to-play sounds, disallowing:
				size_t numToDisallow = numAllowedToPlay[u] - mMaxPlayingSounds[u];
				for(size_t k = 0; k != numToDisallow; ++k)
				{
					// Search a sound with a minimal priority.
					Priority minPriority = std::numeric_limits<int>::max();
					SoundPlayInfo* minPriorityCP = nullptr;
					SoundPlayInfos::iterator it = mSoundPlayInfos.begin(); 
					while(it != mSoundPlayInfos.end())
					{
						SoundPlayInfo* cp = &(*it++);
						if(cp->allowedToPlay)
						{
							Sound* sound = cp->sound;
							if(sound->m3D == (u!=0) && minPriority > sound->mPriority)
							{
								minPriority = sound->mPriority;
								minPriorityCP = cp;
							}
						}
					}
					// Disallowing a sound with a minimal priority.
					minPriorityCP->allowedToPlay = false;
				}
			}
	}
	//------------------------------------------------------------------------------
	void SoundSystem::processPlayAllowance(SoundPlayInfo* _cp, bool _justAllowed)
	{
		Sound* sound = _cp->sound;
		if(_cp->playContext.isNull())
		{
			// The sound is not playing now.
			if(mPlayingSounds[sound->m3D].size() < mMaxPlayingSounds[sound->m3D])
			{
				// Start playing (or wait some time).
				if(_cp->waitTime <= 0)
				{
					play(_cp);
					if(!_cp->playContext.isNull())
						calculateVolumeSmoothingFactorIn(_cp);
				}
			}
		}
		else // !_cp->playContext.isNull()
		{
			// The sound is playing now, 
			// so we should only recalculate the parameters for the volume smoothing.
			if(_justAllowed)
				calculateVolumeSmoothingFactorIn(_cp);
		}
	}
	//------------------------------------------------------------------------------
	void SoundSystem::processPlayDisallowance(SoundPlayInfo* _cp, bool _justDisallowed)
	{
		if(!_cp->playContext.isNull())
		{
			// The sound is playing now, 
			// calculate the parameters for the volume smoothing
			calculateVolumeSmoothingFactorOut(_cp);
		}
	}
	//------------------------------------------------------------------------------
	void SoundSystem::play(SoundPlayInfo* _cp)
	{
		Sound* sound = _cp->sound;
		const SoundDefPtr& soundDef = sound->getSoundDef();
		size_t worldTimeIndex = _cp->worldTimeIndex;
		if(worldTimeIndex >= soundDef->getNumTimeProperties())
			return;

		const SoundDef::TimeProperty& timeProperty = soundDef->getTimeProperty(worldTimeIndex);
		const String& fileName = timeProperty.fileName;
		const String& resourceGroup = timeProperty.resourceGroup;
		if(fileName.empty())
			return;

		// Use SoundPlayerEnumerator to start playing
		SoundPlayContextPtr playContext;
		try
		{
			playContext = SoundPlayerEnumerator::getSingleton().play(
				sound->m3D, fileName, resourceGroup);
		}
		catch(Exception& _exception)
		{
			// Transform exceptions to errors
			// (the application should not be stopped for reason 
			// that some sound cannot be played).
			const String& desc = _exception.getDescription();

			// Suppress duplication messages.
			StringSet::iterator it = mErrorMessages.find(desc);
			if(it == mErrorMessages.end())
			{
				mErrorMessages.insert(desc);
				GOTHOGRE_ERROR(desc);
			}

			// Can't play, set null
			playContext.setNull();
		}

		_cp->playContext = playContext;
		if(!playContext.isNull())
		{
			// Setup play context
			playContext->setLooping(sound->mLoopMode == SoundLoopMode::INFINITE_LOOP);
			if(sound->m3D)
			{
				if(sound->mSoundMaxDistance != Math::POS_INFINITY)
					playContext->setMaxDistance(sound->mSoundMaxDistance);
				Node* node = sound->getParentNode();
				const Vector3& pos = node ? node->_getDerivedPosition() : Vector3::ZERO;
				playContext->setPosition(pos);
			}
			_cp->is3D = sound->m3D;
			mPlayingSounds[_cp->is3D].push_back(fileName);
			_cp->filenameIterator = mPlayingSounds[_cp->is3D].end();
			--_cp->filenameIterator;
		}
	}
	//------------------------------------------------------------------------------
	void SoundSystem::stop(SoundPlayInfo* _cp)
	{
		if(!_cp->playContext.isNull())
		{
			Sound* sound = _cp->sound;
			_cp->playContext.setNull();
			++_cp->numPlayed;
			++sound->mNumPlayed;
			mPlayingSounds[_cp->is3D].erase(_cp->filenameIterator);
		}
	}
	//-------------------------------------------------------------------------
	bool SoundSystem::isSoundEnded(SoundPlayInfo* _cp)
	{
		// Return true if the sound track just finished
		return !_cp->playContext.isNull() && !_cp->playContext->isPlaying();
	}
	//-------------------------------------------------------------------------
	void SoundSystem::processSoundEnd(SoundPlayInfo* _cp)
	{
		// It's necessary to calculate length of the sound BEFORE the sound will be stopped
		Real length = 0;
		if(!_cp->playContext.isNull())
			length = _cp->playContext->getPlayLength();
		
		stop(_cp);
		
		Sound* sound = _cp->sound;
		switch(sound->mLoopMode)
		{
			case SoundLoopMode::PLAY_ONCE: 
			case SoundLoopMode::PLAY_ONCE_PER_ENTER:
			{
				_cp->waitTime = Math::POS_INFINITY; // no play again
				break;
			}
			case SoundLoopMode::INFINITE_LOOP: 
			{
				_cp->waitTime = 0;
				break;
			}
			case SoundLoopMode::INFINITE_LOOP_WITH_DELAY: 
			{
				Real finalMinDelay = sound->mMinDelay + length * sound->mMinDelayLengthMultiplier;
				Real finalMaxDelay = sound->mMaxDelay + length * sound->mMaxDelayLengthMultiplier;
				_cp->waitTime = Math::RangeRandom(finalMinDelay, finalMaxDelay);
				break;
			}
		}
	}
	//------------------------------------------------------------------------------
	void SoundSystem::calculateVolumeSmoothingFactorIn(SoundPlayInfo* _cp)
	{
		Sound* sound = _cp->sound;
		if(sound->mInVolumeSmoothingTime == 0)
			_cp->volumeTimeCoef = Math::POS_INFINITY;
		else
			_cp->volumeTimeCoef = 1 / sound->mInVolumeSmoothingTime;
	}
	//------------------------------------------------------------------------------
	void SoundSystem::calculateVolumeSmoothingFactorOut(SoundPlayInfo* _cp)
	{
		Sound* sound = _cp->sound;
		if(sound->mOutVolumeSmoothingTime == 0)
			_cp->volumeTimeCoef = Math::NEG_INFINITY;
		else
			_cp->volumeTimeCoef = -1 / sound->mOutVolumeSmoothingTime;
	}
	//------------------------------------------------------------------------------
	void SoundSystem::calculateVolume(SoundPlayInfo* _cp, Real _time)
	{
		_cp->volume += _time * _cp->volumeTimeCoef;
		if(_cp->volume > 1)
		{
			_cp->volume = 1;
			_cp->volumeTimeCoef = 0;
		}
		else if(_cp->volume < 0)
		{
			_cp->volume = 0;
			_cp->volumeTimeCoef = 0;
		}

		if(!_cp->playContext.isNull())
		{
			Sound* sound = _cp->sound;
			Real groupVolume = sound->mSoundGroupEntry ? sound->mSoundGroupEntry->soundGroup->volume : 1;
			Real newFinalVolume = _cp->volume * groupVolume * sound->mVolume * _cp->volumeFromSoundDef;
			if(_cp->finalVolume != newFinalVolume)
			{
				_cp->finalVolume = newFinalVolume;
				if(_cp->playContext->isPlaying())
					_cp->playContext->setVolume(_cp->finalVolume);
			}
		}
	}
	//-------------------------------------------------------------------------
	void SoundSystem::removeSoundPlayInfo(SoundPlayInfo* _cp, bool _allowDestroySound)
	{
		// Stop the playing
		stop(_cp);

		// Remove information from the list of currently playing sounds.
		Sound* sound = _cp->sound;
		sound->mSoundPlayInfo = nullptr;
		mSoundPlayInfos.erase(_cp->playInfoIterator);

		// Destroy the sound if it's needed and if it's allowed
		if(_allowDestroySound && sound->mDestroyOnEnd
			&& sound->mLoopMode == SoundLoopMode::PLAY_ONCE && sound->mNumPlayed != 0)
		{
			destroySound(sound);
		}
	}
	//-------------------------------------------------------------------------
	void SoundSystem::removeSoundPlayInfo(Sound* _sound, bool _allowDestroySound)
	{
		if(_sound->mSoundPlayInfo)
			removeSoundPlayInfo(_sound->mSoundPlayInfo, _allowDestroySound);
	}
	//------------------------------------------------------------------------------
	Vector3 SoundSystem::calculateSoundCenter(Sound* _sound)
	{
		Vector3 pos;
		if(_sound->mSoundBoundingBox.isInfinite())
		{
			// infinite bounding box; use parent node's position
			if(_sound->getParentNode())
				pos = _sound->getParentNode()->_getDerivedPosition();
			else
				pos = Vector3::ZERO;
		}
		else if(_sound->mSoundBoundingBox.isFinite())
		{
			// finite bounding box; calculate center
			pos = _sound->mSoundBoundingBox.getCenter();
			if(_sound->mSoundBoundingBoxInLocalSpace && _sound->getParentNode())
				pos = _sound->getParentNode()->convertLocalToWorldPosition(pos);
		}
		else 
		{
			// null bounding box
			pos = Vector3::ZERO;
		}
		return pos;
	}
	//------------------------------------------------------------------------------
	Real SoundSystem::calculateSoundRadius(Sound* _sound)
	{
		Real radius;
		if(_sound->mSoundBoundingBox.isInfinite())
		{
			// infinite bounding box; use max distance
			radius = _sound->mSoundMaxDistance;
		}
		else if(_sound->mSoundBoundingBox.isFinite())
		{
			// finite bounding box; calculate radius
			radius = _sound->mSoundBoundingBox.getHalfSize().length();
		}
		else
		{
			// null bounding box
			radius = 0;
		}
		return radius;
	}
	//------------------------------------------------------------------------------
	void SoundSystem::insertToBst(Sound* _sound)
	{
		if(_sound->mBstElemHandle == BST::NULL_HANDLE)
		{
			SoundScene* soundScene = _sound->mSoundScene;
			Vector3 pos = calculateSoundCenter(_sound);
			Real radius = calculateSoundRadius(_sound);
			_sound->mBstElemHandle = soundScene->bst.add(pos, radius, _sound);
		}
	}
	//------------------------------------------------------------------------------
	void SoundSystem::removeFromBst(Sound* _sound)
	{
		if(_sound->mBstElemHandle != BST::NULL_HANDLE)
		{
			SoundScene* soundScene = _sound->mSoundScene;
			soundScene->bst.remove(_sound->mBstElemHandle);
			_sound->mBstElemHandle = BST::NULL_HANDLE;
		}
	}
	//-------------------------------------------------------------------------
	void SoundSystem::updateBstRadius(Sound* _sound)
	{
		if(_sound->mBstElemHandle != BST::NULL_HANDLE)
		{
			SoundScene* soundScene = _sound->mSoundScene;
			Real radius = calculateSoundRadius(_sound);
			soundScene->bst.setRadius(_sound->mBstElemHandle, radius);
		}
	}
	//-------------------------------------------------------------------------
	void SoundSystem::updateBstCenter(Sound* _sound)
	{
		if(_sound->mBstElemHandle != BST::NULL_HANDLE)
		{
			SoundScene* soundScene = _sound->mSoundScene;
			Vector3 pos = calculateSoundCenter(_sound);
			soundScene->bst.setCenter(_sound->mBstElemHandle, pos);
		}
	}





	//-------------------------------------------------------------------------
	// Sound settings
	//-------------------------------------------------------------------------
	void SoundSystem::soundSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		Real defaultPadding = 300.f;
		mPadding = SoundSettings::getSingleton().getSetting("Padding", "SoundSystem", defaultPadding);

		size_t defaultMaxPlaying2D = 1;
		size_t defaultMaxPlaying3D = 0x7FFF;
		mMaxPlayingSounds[false] = SoundSettings::getSingleton().getSetting("MaxPlayingSounds2D", "SoundSystem", defaultMaxPlaying2D);
		mMaxPlayingSounds[true] = SoundSettings::getSingleton().getSetting("MaxPlayingSounds3D", "SoundSystem", defaultMaxPlaying3D);
		
		BST::RealList defaultList;
		defaultList.push_back(5000); // 50m
		mBSTMaxRadiusList = SoundSettings::getSingleton().getSetting("BSTMaxRadiusList", "SoundSystem", defaultList);
		for(SoundScenesBySM::iterator it = mSoundScenesBySM.begin();
			it != mSoundScenesBySM.end(); ++it)
		{
			SoundScene* soundScene = it->second;
			soundScene->bst.setMaxRadiusList(mBSTMaxRadiusList);
		}

		for(SoundGroupsByName::iterator it = mSoundGroupsByName.begin(); it != mSoundGroupsByName.end(); ++it)
		{
			SoundGroup* soundGroup = it->second;
			const String& groupName = it->first;
			soundGroup->volume = SoundSettings::getSingleton().getSetting(groupName, "Volumes", 1.f);
		}
	}
}