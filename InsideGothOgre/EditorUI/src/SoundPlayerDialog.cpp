#include "Precompiled.h"
#include "SoundPlayerDialog.h"


namespace GUISystem
{
	//---------------------------------------------------------------------------
	// SoundPlayerWorker - the worker class for the SoundPlayerDialog
	//---------------------------------------------------------------------------
	class SoundPlayerWorker
	{
	public:
		SoundPlayerWorker();
		~SoundPlayerWorker();

		void setFileName(const String& _filename, const String& _resourceGroup);
		const String& getFileName() const;
		const String& getResourceGroup() const;

		void play();
		void stop();
		bool isPlaying() const;

		void setLooping(bool _looping);
		bool isLooping() const;

		void setVolume(Real _volume);
		Real getVolume() const;

		Real getPlayLength() const;
		Real getPlayPosition() const;

	private:
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED(SoundPlayerWorker);

	private:
		String              mFileName;
		String              mResourceGroup;
		Real                mVolume;
		bool                mLooping;
		World*              mWorld;
		Sound*              mSound;
		SoundDefPtr         mSoundDef;
		SoundPlayContextPtr mSoundPlayContext;
	};
	//----------------------------------------------------------------------------
	SoundPlayerWorker::SoundPlayerWorker()
		: mVolume(1), mLooping(false), mWorld(nullptr), mSound(nullptr)
	{
		registerWorldListener();
	}
	//----------------------------------------------------------------------------
	SoundPlayerWorker::~SoundPlayerWorker()
	{
		stop();
	}
	//----------------------------------------------------------------------------
	void SoundPlayerWorker::setFileName(const String& _filename, const String& _resourceGroup)
	{
		mFileName = _filename;
		mResourceGroup = _resourceGroup;
		play();
	}
	//----------------------------------------------------------------------------
	const String& SoundPlayerWorker::getFileName() const
	{
		return mFileName;
	}
	//----------------------------------------------------------------------------
	const String& SoundPlayerWorker::getResourceGroup() const
	{
		return mResourceGroup;
	}
	//----------------------------------------------------------------------------
	void SoundPlayerWorker::play()
	{
		stop();
		mWorld = WorldManager::getSingleton().getActiveWorld();
		if(mWorld)
		{
			SceneManagerEx* sceneMgr = mWorld->getSceneManager();
			mSound = SoundSystem::getSingleton().createSound(sceneMgr);
			mSound->setPriority(Priority::HIGHEST);
			if(mSound)
			{
				mSoundDef = SoundDefManagerEx::getSingleton().createManual("$$$_SoundInSoundPlayer");
				SoundDef::TimeProperty timeProp;
				timeProp.playingStartTime = Time(0);
				timeProp.playingRepeatPeriod = Time(0);
				timeProp.fileName = mFileName;
				timeProp.resourceGroup = mResourceGroup;
				timeProp.volume = 1;
				mSoundDef->insertTimeProperty(timeProp);
				mSound->setSound2D(mSoundDef);
			}
		}
		else
		{
			mSoundPlayContext = SoundPlayerEnumerator::getSingleton().play(false, mFileName, mResourceGroup);
		}
		setLooping(mLooping);
		setVolume(mVolume);
	}
	//----------------------------------------------------------------------------
	void SoundPlayerWorker::stop()
	{
		if(mSound)
		{
			SceneManagerEx* sceneMgr = mWorld->getSceneManager();
			SoundSystem::getSingleton().destroySound(mSound);
			mSound = nullptr;
			SoundDefManagerEx::getSingleton().remove(mSoundDef);
		}
		if(!mSoundPlayContext.isNull())
		{
			mSoundPlayContext.setNull();
		}
	}
	//----------------------------------------------------------------------------
	bool SoundPlayerWorker::isPlaying() const
	{
		if(mSound)
		{
			return mSound->isPlaying();
		}
		else if(!mSoundPlayContext.isNull())
		{
			return mSoundPlayContext->isPlaying();
		}
		else
		{
			return false;
		}		
	}
	//----------------------------------------------------------------------------
	void SoundPlayerWorker::setLooping(bool _looping)
	{
		mLooping = _looping;
		if(mSound)
		{
			mSound->setLoopMode(_looping ? SoundLoopMode::INFINITE_LOOP : SoundLoopMode::PLAY_ONCE);
		}
		else if(!mSoundPlayContext.isNull())
		{
			mSoundPlayContext->setLooping(_looping);
		}
	}
	//----------------------------------------------------------------------------
	bool SoundPlayerWorker::isLooping() const
	{
		return mLooping;
	}
	//----------------------------------------------------------------------------
	void SoundPlayerWorker::setVolume(Real _volume)
	{
		mVolume = _volume;
		if(mSound)
		{
			mSound->setVolume(_volume);
		}
		if(!mSoundPlayContext.isNull())
		{
			mSoundPlayContext->setVolume(_volume);
		}
	}
	//----------------------------------------------------------------------------
	Real SoundPlayerWorker::getVolume() const
	{
		return mVolume;
	}
	//----------------------------------------------------------------------------
	Real SoundPlayerWorker::getPlayLength() const
	{
		if(mSound)
		{
			return mSound->getPlayLength();
		}
		else if(!mSoundPlayContext.isNull())
		{
			return mSoundPlayContext->getPlayLength();
		}
		else
		{
			return 0;
		}
	}
	//----------------------------------------------------------------------------
	Real SoundPlayerWorker::getPlayPosition() const
	{
		if(mSound)
		{
			return mSound->getPlayPosition();
		}
		else if(!mSoundPlayContext.isNull())
		{
			return mSoundPlayContext->getPlayPosition();
		}
		else
		{
			return 0;
		}
	}
	//----------------------------------------------------------------------------
	void SoundPlayerWorker::worldActiveChanged(const WorldEvent& _evt)
	{
		stop();
	}

		





	//----------------------------------------------------------------------------
	// SoundPlayerDialog
	//----------------------------------------------------------------------------
	SoundPlayerDialog::SoundPlayerDialog()
	{
		initialiseByAttributes(this);

		mButtonPlay->eventMouseButtonClick += MyGUI::newDelegate(this, &SoundPlayerDialog::notifyButtonPlay);
		mButtonStop->eventMouseButtonClick += MyGUI::newDelegate(this, &SoundPlayerDialog::notifyButtonStop);
		mCheckLooping->eventMouseButtonClick += MyGUI::newDelegate(this, &SoundPlayerDialog::notifyCheckLooping);
		mSliderVolume->eventScrollChangePosition += MyGUI::newDelegate(this, &SoundPlayerDialog::notifyVolumeChanged);

		mWorker = new SoundPlayerWorker;
		registerFrameListener();
	}
	//----------------------------------------------------------------------------
	SoundPlayerDialog::~SoundPlayerDialog()
	{
		delete mWorker;
	}
	//----------------------------------------------------------------------------
	void SoundPlayerDialog::setFileName(const String& _filename, const String& _resourceGroup)
	{
		mWorker->setFileName(_filename, _resourceGroup);
		mEditFileName->setCaption(_filename);
		mEditLength->setCaption(StringUtil::BLANK);
	}
	//----------------------------------------------------------------------------
	void SoundPlayerDialog::frameStarted(const FrameEvent& _evt)
	{
		if(mWorker->isPlaying())
		{
			mButtonPlay->setEnabled(false);
			mButtonStop->setEnabled(true);

			if(mEditLength->getCaption().empty())
			{
				Real playLength = mWorker->getPlayLength();
				mEditLength->setCaption(secondsToString(playLength, true));
				int range = Math::IFloor(playLength * 1000);
				mSliderPosition->setScrollRange(range != 0 ? range : 2);
			}

			Real playPosition = mWorker->getPlayPosition();
			mEditPosition->setCaption(secondsToString(playPosition, false));
			mSliderPosition->setScrollPosition(Math::IFloor(playPosition * 1000));
		}
		else
		{
			mButtonStop->setEnabled(false);
			mButtonPlay->setEnabled(!mWorker->getFileName().empty());
			mEditPosition->setCaption("00:00");
			mSliderPosition->setScrollPosition(0);
		}
	}
	//----------------------------------------------------------------------------
	void SoundPlayerDialog::notifyButtonPlay(MyGUI::Widget* _sender)
	{
		mWorker->play();
	}
	//----------------------------------------------------------------------------
	void SoundPlayerDialog::notifyButtonStop(MyGUI::Widget* _sender)
	{
		mWorker->stop();
	}
	//----------------------------------------------------------------------------
	void SoundPlayerDialog::notifyCheckLooping(MyGUI::Widget* _sender)
	{
		bool looping = mWorker->isLooping();
		looping = !looping;
		mWorker->setLooping(looping);
		mCheckLooping->setStateSelected(looping);
	}
	//----------------------------------------------------------------------------
	void SoundPlayerDialog::notifyVolumeChanged(MyGUI::ScrollBar* _sender, size_t _position)
	{
		Real volume = Real(_position + 1) / 100;
		mWorker->setVolume(volume);
	}
	//--------------------------------------------------------------------------
	void SoundPlayerDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if (_name == "close")
			eventEndDialog();
	}
	//----------------------------------------------------------------------------
	String SoundPlayerDialog::secondsToString(Real _time, bool _microseconds)
	{
		int seconds = Math::IFloor(_time);
		int minutes = seconds / 60;
		seconds %= 60;
		
		String smin = StrConv::toString(minutes);
		if(smin.length() < 2)
			smin.insert(0, 2 - smin.length(), '0');

		String ssec = StrConv::toString(seconds);
		if(ssec.length() < 2)
			ssec.insert(0, 2 - ssec.length(), '0');

		String str = smin + ':' + ssec;
		
		if(_microseconds)
		{
			int ms = Math::IFloor(1000 * (_time - Math::IFloor(_time)));
			String sms = StrConv::toString(ms);
			if(sms.length() < 3)
				sms.insert(0, 3 - smin.length(), '0');
			str += ':' + sms;
		}
		return str;
	}

} // namespace GUISystem