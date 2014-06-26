#include "Precompiled.h"
#include "ZoneMusic.h"

namespace GothOgre
{
	//-------------------------------------------------------------------
	// Parameters
	//-------------------------------------------------------------------
	class ZoneMusic_Name : public ZenParamString //  overrides Vob_Name
	{
	public:
		const String& getName()
		{
			static const String s = "Name";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		String getValue(const void* _target)
		{
			const ZoneMusic* f = static_cast<const ZoneMusic*>(_target);
			return f->getBase()->getParameter<String>("Name");
		}

		void setValue(void* _target, const String& _value)
		{
			ZoneMusic* f = static_cast<ZoneMusic*>(_target);
			f->getBase()->setParameter<String>("Name", _value);
			f->_nameChanged();
		}
	};
	//---------------------------------------------------------------------------
	class ZoneMusic_Enabled : public ZenParamBool
	{
	public:
		const String& getName()
		{
			static const String s = "Enabled";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "ZoneMusic";
			return s;
		}

		bool getValue(const void* _target)
		{
			const ZoneMusic* f = static_cast<const ZoneMusic*>(_target);
			return f->isEnabled();
		}

		void setValue(void* _target, bool _value)
		{
			ZoneMusic* f = static_cast<ZoneMusic*>(_target);
			f->setEnabled(_value);
		}
	};	
	//---------------------------------------------------------------------------
	class ZoneMusic_Priority : public ZenParamInt
	{
	public:
		const String& getName()
		{
			static const String s = "Priority";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "ZoneMusic";
			return s;
		}

		int getValue(const void* _target)
		{
			const ZoneMusic* f = static_cast<const ZoneMusic*>(_target);
			return f->getPriority();
		}

		void setValue(void* _target, int _value)
		{
			ZoneMusic* f = static_cast<ZoneMusic*>(_target);
			f->setPriority(_value);
		}
	};	
	//---------------------------------------------------------------------------
	class ZoneMusic_Volume : public ZenParamReal
	{
	public:
		const String& getName()
		{
			static const String s = "VolumeLevel";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "ZoneMusic";
			return s;
		}

		Real getValue(const void* _target)
		{
			const ZoneMusic* f = static_cast<const ZoneMusic*>(_target);
			return f->getVolume();
		}

		void setValue(void* _target, Real _value)
		{
			ZoneMusic* f = static_cast<ZoneMusic*>(_target);
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
	class ZoneMusic_LoopMode : public ZenParamEnumT<SoundLoopMode>
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
			static const String s = "ZoneMusic";
			return s;
		}

		int getValue(const void* _target)
		{
			const ZoneMusic* f = static_cast<const ZoneMusic*>(_target);
			return f->getLoopMode();
		}

		void setValue(void* _target, int _value)
		{
			ZoneMusic* f = static_cast<ZoneMusic*>(_target);
			f->setLoopMode(_value);
		}
	};
	//--------------------------------------------------------------------------
	class ZoneMusic_WorldBounds : public ZenParamAxisAlignedBox // overrides Vob_WorldBounds to make it non-internal
	{
	public:
		const String& getName()
		{
			static const String s = "WorldBounds";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "ZoneMusic";
			return s;
		}

		AxisAlignedBox getValue(const void* _target)
		{
			const ZoneMusic* f = static_cast<const ZoneMusic*>(_target);
			return f->getBase()->getParameter<AxisAlignedBox>("WorldBounds");
		}

		void setValue(void* _target, const AxisAlignedBox& _value)
		{
			ZoneMusic* f = static_cast<ZoneMusic*>(_target);
			f->getBase()->setParameter<AxisAlignedBox>("WorldBounds", _value);
			f->_worldBoundsChanged();
		}
	};



	//-------------------------------------------------------------------
	// ZoneMusic
	//-------------------------------------------------------------------
	ZoneMusic::ZoneMusic(ZenFactory* _creator)
		: DerivedWorldObject("Vob"), mSound(nullptr)
	{
		if (createParamDictionary("ZoneMusic"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new ZoneMusic_Name );
			dict->addParamDef( new ZoneMusic_Enabled );
			dict->addParamDef( new ZoneMusic_Priority );
			dict->addParamDef( new ZoneMusic_Volume );
			dict->addParamDef( new ZoneMusic_LoopMode );
			dict->addParamDef( new ZoneMusic_WorldBounds );
		}

		setParameter<String>("InvisibleVisual", "invisible_zCZoneMusic.3ds");
		createSound();
	}
	//-------------------------------------------------------------------
	ZoneMusic::~ZoneMusic()
	{
		destroySound();
	}
	//-------------------------------------------------------------------
	void ZoneMusic::setEnabled(bool _enable)
	{
		mSound->setEnabled(_enable);
	}
	//-------------------------------------------------------------------
	bool ZoneMusic::isEnabled() const
	{
		return mSound->isEnabled();
	}
	//-------------------------------------------------------------------
	void ZoneMusic::setPriority(int _priority)
	{
		mSound->setPriority(_priority);
	}
	//-------------------------------------------------------------------
	int ZoneMusic::getPriority() const
	{
		return mSound->getPriority();
	}
	//-------------------------------------------------------------------
	void ZoneMusic::setVolume(Real _volume)
	{
		Real vol = _volume / 100;
		mSound->setVolume(vol);
	}
	//-------------------------------------------------------------------
	Real ZoneMusic::getVolume() const
	{
		return mSound->getVolume() * 100;
	}
	//-------------------------------------------------------------------
	void ZoneMusic::setLoopMode(SoundLoopMode _loopMode)
	{
		mSound->setLoopMode(_loopMode);
	}
	//-------------------------------------------------------------------
	SoundLoopMode ZoneMusic::getLoopMode() const
	{
		return mSound->getLoopMode();
	}
	//-------------------------------------------------------------------
	void ZoneMusic::_nameChanged()
	{
		// Search a new sound def based on the last three characters of this vob's name
		SoundDefPtr soundDef;

		const String& name = getName(); // for example, vob's name = "KHORINIS_KHO"

		// Get the last three characters - the characters after the last underline
		size_t underlinePos = name.rfind('_');
		if(underlinePos != String::npos)
		{
			// The name of the music theme
			String theme = name.substr(underlinePos + 1); // for example, theme = "KHO"
			soundDef = SoundDefManagerEx::getSingleton().findByName("MUSIC_" + theme);
		}

		// Store the new sound def
		mSound->setSound2D(soundDef);		
	}
	//-------------------------------------------------------------------
	void ZoneMusic::_worldBoundsChanged()
	{
		AxisAlignedBox aab = getParameter<AxisAlignedBox>("WorldBounds");
		mSound->setSoundBoundingBox(aab, false);
	}
	//-------------------------------------------------------------------
	void ZoneMusic::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
		
		_nameChanged();
		_worldBoundsChanged();

		setEnabled(_zenArchive->readBool("enabled"));
		setPriority( _zenArchive->readInt("priority"));
		bool ellipsoid = _zenArchive->readBool("ellipsoid");
		float reverbLevel = _zenArchive->readFloat("reverbLevel");
		setVolume( _zenArchive->readFloat("volumeLevel") * 100 );
		bool loop = _zenArchive->readBool("loop");
		setLoopMode(loop ? SoundLoopMode::INFINITE_LOOP : SoundLoopMode::PLAY_ONCE);
	}
	//-------------------------------------------------------------------
	void ZoneMusic::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void ZoneMusic::createSound()
	{
		if(!mSound)
		{
			SceneNode* sceneNode = getSceneNode();
			SceneManagerEx* sm = (SceneManagerEx*) sceneNode->getCreator();
			mSound = SoundSystem::getSingleton().createSound(sm);
			sceneNode->attachObject(mSound);
			mSound->setSoundGroup( SoundGroup::ZONE_MUSIC );
			mSound->setInVolumeSmoothingTime(1.0f);  // 1 second
			mSound->setOutVolumeSmoothingTime(1.0f); // 1 second
		}
	}
	//-------------------------------------------------------------------
	void ZoneMusic::destroySound()
	{
		if(mSound)
		{
			SoundSystem::getSingleton().destroySound(mSound);
			mSound = nullptr;
		}
	}






	//-------------------------------------------------------------------
	// ZoneMusicFactory
	//-------------------------------------------------------------------
	ZoneMusicFactory::ZoneMusicFactory()
		: ZenFactory("ZoneMusic")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZoneMusicFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCZoneMusic:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZoneMusicFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZoneMusicFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x0000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZoneMusicFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZoneMusicFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZoneMusic(this);
	}

} // namespace GothOgre