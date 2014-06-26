#ifndef ZONE_VOB_FAR_PLANE_DEFAULT_H
#define ZONE_VOB_FAR_PLANE_DEFAULT_H

namespace GothOgre
{
	/***/
	class ZoneVobFarPlaneDefault : public DerivedWorldObject
	{
	public:
		ZoneVobFarPlaneDefault(ZenFactory* _creator);
		virtual ~ZoneVobFarPlaneDefault();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class ZoneVobFarPlaneDefaultFactory : public ZenFactory
	{
	public:
		ZoneVobFarPlaneDefaultFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ZONE_VOB_FAR_PLANE_DEFAULT_H
