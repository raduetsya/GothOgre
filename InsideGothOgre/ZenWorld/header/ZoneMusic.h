#ifndef ZONE_MUSIC_H
#define ZONE_MUSIC_H

namespace GothOgre
{
	/***/
	class ZoneMusic : public DerivedWorldObject
	{
	public:
		ZoneMusic(ZenFactory* _creator);
		virtual ~ZoneMusic();

		void setEnabled(bool _enable);
		bool isEnabled() const;

		void setPriority(int _priority);
		int getPriority() const;

		void setVolume(Real _volume);
		Real getVolume() const;

		void setLoopMode(SoundLoopMode _loopMode);
		SoundLoopMode getLoopMode() const;

		void read(ZenArchive* _zenArchive, ushort _version);
		void write(ZenArchive* _zenArchive, ushort _version);

	public:
		void _nameChanged();
		void _worldBoundsChanged();
		
		void createSound();
		void destroySound();
		void updateSound();

		// GOTHOGRE_DECLARE_HERO_STATUS_CHANGED

	private:
		Sound*  mSound;
	};

	/***/
	class ZoneMusicFactory : public ZenFactory
	{
	public:
		ZoneMusicFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ZONE_MUSIC_H
