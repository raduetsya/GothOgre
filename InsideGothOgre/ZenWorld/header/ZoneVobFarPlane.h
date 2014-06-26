#ifndef ZONE_VOB_FAR_PLANE_H
#define ZONE_VOB_FAR_PLANE_H

namespace GothOgre
{
	/***/
	class ZoneVobFarPlane : public DerivedWorldObject
	{
	public:
		ZoneVobFarPlane(ZenFactory* _creator);
		virtual ~ZoneVobFarPlane();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class ZoneVobFarPlaneFactory : public ZenFactory
	{
	public:
		ZoneVobFarPlaneFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ZONE_VOB_FAR_PLANE_H
