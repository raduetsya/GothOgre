#ifndef ZONE_ZFOG_DEFAULT_H
#define ZONE_ZFOG_DEFAULT_H

namespace GothOgre
{
	/***/
	class ZoneZFogDefault : public DerivedWorldObject
	{
	public:
		ZoneZFogDefault(ZenFactory* _creator);
		virtual ~ZoneZFogDefault();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class ZoneZFogDefaultFactory : public ZenFactory
	{
	public:
		ZoneZFogDefaultFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ZONE_ZFOG_DEFAULT_H
