#include "Precompiled.h"
#include "VobSoundDaytime.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	class VobSoundDaytime_StartTime : public ZenParamTime
	{
	public:
		const String& getName()
		{
			static const String s = "StartTime";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSoundDaytime";
			return s;
		}

		Time getValue(const void* _target)
		{
			const VobSoundDaytime* f = static_cast<const VobSoundDaytime*>(_target);
			return f->getStartTime();
		}

		void setValue(void* _target, const Time& _value)
		{
			VobSoundDaytime* f = static_cast<VobSoundDaytime*>(_target);
			f->setStartTime(_value);
		}
	};
	//---------------------------------------------------------------------------
	class VobSoundDaytime_EndTime : public ZenParamTime
	{
	public:
		const String& getName()
		{
			static const String s = "EndTime";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSoundDaytime";
			return s;
		}

		Time getValue(const void* _target)
		{
			const VobSoundDaytime* f = static_cast<const VobSoundDaytime*>(_target);
			return f->getEndTime();
		}

		void setValue(void* _target, const Time& _value)
		{
			VobSoundDaytime* f = static_cast<VobSoundDaytime*>(_target);
			f->setEndTime(_value);
		}
	};
	//---------------------------------------------------------------------------
	class VobSoundDaytime_SoundName : public ZenParamString
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
			static const String s = "VobSoundDaytime";
			return s;
		}

		const String& getSubType()
		{
			return SubType::SOUND_NAME;
		}

		String getValue(const void* _target)
		{
			const VobSoundDaytime* f = static_cast<const VobSoundDaytime*>(_target);
			return f->getSoundName();
		}

		void setValue(void* _target, const String& _value)
		{
			VobSoundDaytime* f = static_cast<VobSoundDaytime*>(_target);
			f->setSoundName(_value);
		}
	};
	//---------------------------------------------------------------------------
	class VobSoundDaytime_SoundName2 : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "SoundName2";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobSoundDaytime";
			return s;
		}

		const String& getSubType()
		{
			return SubType::SOUND_NAME;
		}

		String getValue(const void* _target)
		{
			const VobSoundDaytime* f = static_cast<const VobSoundDaytime*>(_target);
			return f->getSoundName2();
		}

		void setValue(void* _target, const String& _value)
		{
			VobSoundDaytime* f = static_cast<VobSoundDaytime*>(_target);
			f->setSoundName2(_value);
		}
	};





	//-------------------------------------------------------------------
	// VobSoundDaytime
	//-------------------------------------------------------------------
	VobSoundDaytime::VobSoundDaytime(ZenFactory* _creator)
		: DerivedWorldObject("VobSound")
	{
		if (createParamDictionary("VobSoundDaytime"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new VobSoundDaytime_SoundName );
			dict->addParamDef( new VobSoundDaytime_StartTime );
			dict->addParamDef( new VobSoundDaytime_EndTime );
			dict->addParamDef( new VobSoundDaytime_SoundName2 );
		}

		setParameter<String>("InvisibleVisual", "invisible_zCVobSoundTime.3ds");
		registerStartTimeListener();
		registerEndTimeListener();
	}
	//-------------------------------------------------------------------
	VobSoundDaytime::~VobSoundDaytime()
	{
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::setStartTime(const Time& _startTime)
	{
		mStartTime = _startTime;
		registerStartTimeListener();
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::registerStartTimeListener()
	{
		registerStartTimeListener(
			WorldTime::getSingleton().getYesterdayTime(mStartTime), 
			Time::ONE_DAY);
	}
	//-------------------------------------------------------------------
	const Time& VobSoundDaytime::getStartTime() const
	{
		return mStartTime;
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::setEndTime(const Time& _endTime)
	{
		mEndTime = _endTime;
		registerEndTimeListener();
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::registerEndTimeListener()
	{
		registerEndTimeListener(
			WorldTime::getSingleton().getYesterdayTime(mEndTime), 
			Time::ONE_DAY);
	}
	//-------------------------------------------------------------------
	const Time& VobSoundDaytime::getEndTime() const
	{
		return mEndTime;
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::setSoundName(const String& _soundName)
	{
		mSoundName = _soundName;
		registerStartTimeListener();
		registerEndTimeListener();
	}
	//-------------------------------------------------------------------
	const String& VobSoundDaytime::getSoundName() const
	{
		return mSoundName;
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::setSoundName2(const String& _soundName2)
	{
		mSoundName2 = _soundName2;
		registerStartTimeListener();
		registerEndTimeListener();
	}
	//-------------------------------------------------------------------
	const String& VobSoundDaytime::getSoundName2() const
	{
		return mSoundName2;
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::startTimeCame(WorldTimeEvent& _evt)
	{
		World* world = getWorld();
		world->freezeChangedState();

		getBase()->setParameter("SoundName", mSoundName);

		world->unfreezeChangedState();
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::endTimeCame(WorldTimeEvent& _evt)
	{
		World* world = getWorld();
		world->freezeChangedState();

		getBase()->setParameter("SoundName", mSoundName2);

		world->unfreezeChangedState();
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
		setSoundName(getBase()->getParameter<String>("SoundName"));
		
		float startHour = _zenArchive->readFloat("sndStartTime");
		startHour = Math::Clamp(startHour, 0.0f, 24.0f);
		setStartTime( startHour * Time::ONE_HOUR );

		float endHour = _zenArchive->readFloat("sndEndTime");
		endHour = Math::Clamp(endHour, 0.0f, 24.0f);
		setEndTime( endHour * Time::ONE_HOUR );

		setSoundName2( _zenArchive->readString("sndName2"));
	}
	//-------------------------------------------------------------------
	void VobSoundDaytime::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobSoundDaytimeFactory::VobSoundDaytimeFactory()
		: ZenFactory("VobSoundDaytime")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobSoundDaytimeFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobSoundDaytime:zCVobSound:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobSoundDaytimeFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobSoundDaytimeFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobSoundDaytimeFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobSoundDaytimeFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobSoundDaytime(this);
	}

} // namespace GothOgre