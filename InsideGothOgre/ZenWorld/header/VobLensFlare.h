#ifndef VOB_LENS_FLARE_H
#define VOB_LENS_FLARE_H

namespace GothOgre
{
	/***/
	class VobLensFlare : public DerivedWorldObject
	{
	public:
		VobLensFlare(ZenFactory* _creator);
		virtual ~VobLensFlare();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class VobLensFlareFactory : public ZenFactory
	{
	public:
		VobLensFlareFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_LENS_FLARE_H
