#ifndef VOB_SPOT_H
#define VOB_SPOT_H

namespace GothOgre
{
	/***/
	class VobSpot : public DerivedWorldObject
	{
	public:
		VobSpot(ZenFactory* _creator);
		virtual ~VobSpot();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class VobSpotFactory : public ZenFactory
	{
	public:
		VobSpotFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_SPOT_H
