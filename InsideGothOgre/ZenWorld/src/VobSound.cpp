#include "Precompiled.h"
#include "VobSound.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	// Parameters
	//---------------------------------------------------------------------------
	class VobSound_SoundName : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "SoundName";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		const String& getSubType()
		{
			return SubType::SOUND_NAME;
		}

		String getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->getSoundName();
		}

		void setValue(void* _target, const String& _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setSoundName(_value);
		}
	};
	//---------------------------------------------------------------------------
	class VobSound_MaxDistance : public ZenParamReal
	{
	public:
		const String& getName()
		{
			static const String s = "MaxDistance";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		Real getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->getMaxDistance();
		}

		void setValue(void* _target, Real _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setMaxDistance(_value);
		}

		Real getMin()
		{
			return 0;
		}
	};
	//---------------------------------------------------------------------------
	class VobSound_MinDelay : public ZenParamReal
	{
	public:
		const String& getName()
		{
			static const String s = "MinDelay";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		Real getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->getMinDelay();
		}

		void setValue(void* _target, Real _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setMinDelay(_value);
		}

		Real getMin()
		{
			return 0;
		}
	};
	//---------------------------------------------------------------------------
	class VobSound_MaxDelay : public ZenParamReal
	{
	public:
		const String& getName()
		{
			static const String s = "MaxDelay";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		Real getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->getMaxDelay();
		}

		void setValue(void* _target, Real _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setMaxDelay(_value);
		}

		Real getMin()
		{
			return 0;
		}
	};
	//---------------------------------------------------------------------------
	class VobSound_LoopMode : public ZenParamEnumT<SoundLoopMode>
	{
	public:
		const String& getName()
		{
			static const String s = "LoopMode";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		int getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->getLoopMode();
		}

		void setValue(void* _target, int _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setLoopMode(_value);
		}
	};
	//---------------------------------------------------------------------------
	class VobSound_Volume : public ZenParamReal
	{
	public:
		const String& getName()
		{
			static const String s = "Volume";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		Real getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->getVolume();
		}

		void setValue(void* _target, Real _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setVolume(_value);
		}

		Real getMin()
		{
			return 0;
		}

		Real getMax()
		{
			return 100;
		}
	};
	//---------------------------------------------------------------------------
	class VobSound_EnabledOnStart : public ZenParamBool
	{
	public:
		const String& getName()
		{
			static const String s = "EnabledOnStart";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		bool getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->isEnabledOnStart();
		}

		void setValue(void* _target, bool _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setEnabledOnStart(_value);
		}
	};
	//---------------------------------------------------------------------------
	class VobSound_EnabledNow : public ZenParamBool
	{
	public:
		const String& getName()
		{
			static const String s = "EnabledNow";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSound";
			return s;
		}

		bool getValue(const void* _target)
		{
			const VobSound* f = static_cast<const VobSound*>(_target);
			return f->isEnabledNow();
		}

		void setValue(void* _target, bool _value)
		{
			VobSound* f = static_cast<VobSound*>(_target);
			f->setEnabledNow(_value);
		}

		bool isInternal()
		{
			return true;
		}
	};



	//-------------------------------------------------------------------
	// VobSound
	//-------------------------------------------------------------------
	VobSound::VobSound(ZenFactory* _creator)
		: DerivedWorldObject("Vob"), mSound(nullptr)
	{
		if (createParamDictionary("VobSound"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new VobSound_SoundName );
			dict->addParamDef( new VobSound_LoopMode );
			dict->addParamDef( new VobSound_MinDelay );
			dict->addParamDef( new VobSound_MaxDelay );
			dict->addParamDef( new VobSound_MaxDistance );
			dict->addParamDef( new VobSound_Volume );
			dict->addParamDef( new VobSound_EnabledOnStart );
			dict->addParamDef( new VobSound_EnabledNow );
		}

		createSound();
		setParameter<String>("InvisibleVisual", "invisible_zCVobSound.3ds");
	}
	//-------------------------------------------------------------------
	VobSound::~VobSound()
	{
		destroySound();
	}
	//-------------------------------------------------------------------
	void VobSound::setSoundName(const String& _name)
	{
		mSoundName = _name;
		SoundDefPtr soundDef = SoundDefManagerEx::getSingleton().findByName(mSoundName);
		mSound->setSound3D(soundDef);
	}
	//-------------------------------------------------------------------
	const String& VobSound::getSoundName() const
	{
		return mSoundName;
	}
	//-------------------------------------------------------------------
	void VobSound::setMaxDistance(Real _radius)
	{
		mSound->setSoundMaxDistance(_radius);
	}
	//-------------------------------------------------------------------
	Real VobSound::getMaxDistance() const
	{
		return mSound->getSoundMaxDistance();
	}
	//-------------------------------------------------------------------
	void VobSound::setMinDelay(Real _delay)
	{
		mMinDelay = _delay;
		mSound->setMinDelay(_delay, -1);
	}
	//-------------------------------------------------------------------
	Real VobSound::getMinDelay() const
	{
		return mMinDelay;
	}
	//-------------------------------------------------------------------
	void VobSound::setMaxDelay(Real _delay)
	{
		mMaxDelay = _delay;
		mSound->setMaxDelay(_delay, -1);
	}
	//-------------------------------------------------------------------
	Real VobSound::getMaxDelay() const
	{
		return mMaxDelay;
	}
	//-------------------------------------------------------------------
	void VobSound::setLoopMode(SoundLoopMode _loopMode)
	{
		mSound->setLoopMode(_loopMode);
	}
	//-------------------------------------------------------------------
	SoundLoopMode VobSound::getLoopMode() const
	{
		return mSound->getLoopMode();
	}
	//-------------------------------------------------------------------
	void VobSound::setVolume(Real _volume)
	{
		mVolume = _volume / 100;
		mSound->setVolume(mVolume);
	}
	//-------------------------------------------------------------------
	Real VobSound::getVolume() const
	{
		return mVolume * 100;
	}
	//-------------------------------------------------------------------
	void VobSound::setEnabledOnStart(bool _enabledOnStart)
	{
		mEnabledOnStart = _enabledOnStart;
	}
	//-------------------------------------------------------------------
	bool VobSound::isEnabledOnStart() const
	{
		return mEnabledOnStart;
	}
	//-------------------------------------------------------------------
	void VobSound::setEnabledNow(bool _enable)
	{
		mSound->setEnabled(_enable);
	}
	//-------------------------------------------------------------------
	bool VobSound::isEnabledNow() const
	{
		return mSound->isEnabled();
	}
	//-------------------------------------------------------------------
	String VobSound::getCombinedName() const
	{
		String str = getBase()->getCombinedName();
		if(!mSoundName.empty())
		{
			if(!str.empty())
				str += ' ';
			str += '\"';
			str += mSoundName;
			str += '\"';
		}
		return str;
	}
	//-------------------------------------------------------------------
	void VobSound::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);

		// volume in range [0..100]
		float sndVolume = _zenArchive->readFloat("sndVolume", 100);
		setVolume(sndVolume);

		SndMode sndMode = (SndMode) _zenArchive->readEnum("sndMode", LOOPING);
		SoundLoopMode soundLoopMode;
		switch(sndMode)
		{
			case LOOPING: soundLoopMode = SoundLoopMode::INFINITE_LOOP; break;
			case ONCE:    soundLoopMode = SoundLoopMode::PLAY_ONCE; break;
			case RANDOM:  soundLoopMode = SoundLoopMode::INFINITE_LOOP_WITH_DELAY; break;
		}
		setLoopMode(soundLoopMode);

		float sndRandDelay = _zenArchive->readFloat("sndRandDelay", 5.0f);
		float sndRandDelayVar = _zenArchive->readFloat("sndRandDelayVar", 2.0f);
		setMinDelay(sndRandDelay - sndRandDelayVar);
		setMaxDelay(sndRandDelay + sndRandDelayVar);
		
		bool sndStartOn = _zenArchive->readBool("sndStartOn", true);
		setEnabledOnStart(sndStartOn);
		setEnabledNow(sndStartOn);

		bool sndAmbient3D = _zenArchive->readBool("sndAmbient3D", false);
		bool sndObstruction = _zenArchive->readBool("sndObstruction", false);
		float sndConeAngle = _zenArchive->readFloat("sndConeAngle", 0);
		SndVolType sndVolType = (SndVolType) _zenArchive->readEnum("sndVolType");
		float sndRadius = _zenArchive->readFloat("sndRadius", 1500);
		setMaxDistance(sndRadius);
		
		String sndName = _zenArchive->readString("sndName");
		setSoundName(sndName);
	}
	//-------------------------------------------------------------------
	void VobSound::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobSound::createSound()
	{
		if(!mSound)
		{
			SceneNode* sceneNode = getSceneNode();
			SceneManagerEx* sm = (SceneManagerEx*) sceneNode->getCreator();
			mSound = SoundSystem::getSingleton().createSound(sm);
			sceneNode->attachObject(mSound);
			mSound->setSoundGroup(SoundGroup::SOUND_EFFECT);
		}
	}
	//-------------------------------------------------------------------
	void VobSound::destroySound()
	{
		if(mSound)
		{
			SoundSystem::getSingleton().destroySound(mSound);
			mSound = nullptr;
		}
	}



	//-------------------------------------------------------------------
	// VobSoundFactory
	//-------------------------------------------------------------------
	VobSoundFactory::VobSoundFactory()
		: ZenFactory("VobSound")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobSoundFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobSound:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobSoundFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobSoundFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobSoundFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobSoundFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobSound(this);
	}

} // namespace GothOgre