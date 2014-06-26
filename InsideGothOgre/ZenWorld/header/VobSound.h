#ifndef VOB_SOUND_H
#define VOB_SOUND_H

namespace GothOgre
{
	//-----------------------------------------------------------------------
	/***/
	class VobSound : public DerivedWorldObject
	{
	public:
		VobSound(ZenFactory* _creator);
		virtual ~VobSound();

		void setSoundName(const String& _name);
		const String& getSoundName() const;

		String getCombinedName() const;

		void setMaxDistance(Real _radius);
		Real getMaxDistance() const;

		void setMinDelay(Real _delay);
		Real getMinDelay() const;
		
		void setMaxDelay(Real _delay);
		Real getMaxDelay() const;
		
		void setLoopMode(SoundLoopMode _loopMode);
		SoundLoopMode getLoopMode() const;

		void setVolume(Real _volume);
		Real getVolume() const;

		void setEnabledOnStart(bool _enabledOnStart);
		bool isEnabledOnStart() const;

		void setEnabledNow(bool _enabled);
		bool isEnabledNow() const;

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);

	private:
		enum SndMode
		{
			LOOPING,
			ONCE,
			RANDOM
		};

		enum SndVolType
		{
			SPHERE,
			ELLIPSOID
		};

		void updateSoundVolume();
		void createSound();
		void destroySound();

	private:
		Sound*  mSound;
		String  mSoundName;
		Real    mMinDelay;
		Real    mMaxDelay;
		Real    mVolume;
		Real    mVolumeFactor;
		bool    mEnabledOnStart;
	};



	//-------------------------------------------------------------------------
	/***/
	class VobSoundFactory : public ZenFactory
	{
	public:
		VobSoundFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_SOUND_H
