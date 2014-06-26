#ifndef ZONE_ZFOG_H
#define ZONE_ZFOG_H

namespace GothOgre
{
	/***/
	class ZoneZFog : public DerivedWorldObject
	{
	public:
		ZoneZFog(ZenFactory* _creator);
		virtual ~ZoneZFog();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class ZoneZFogFactory : public ZenFactory
	{
	public:
		ZoneZFogFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ZONE_ZFOG_H
