#ifndef ZONE_MUSIC_DEFAULT_H
#define ZONE_MUSIC_DEFAULT_H

namespace GothOgre
{
	/***/
	class ZoneMusicDefault : public DerivedWorldObject
	{
	public:
		ZoneMusicDefault(ZenFactory* _creator);
		virtual ~ZoneMusicDefault();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class ZoneMusicDefaultFactory : public ZenFactory
	{
	public:
		ZoneMusicDefaultFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ZONE_MUSIC_DEFAULT_H
