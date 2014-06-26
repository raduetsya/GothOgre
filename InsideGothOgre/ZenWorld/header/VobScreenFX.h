#ifndef VOB_SCREEN_FX_H
#define VOB_SCREEN_FX_H

namespace GothOgre
{
	/***/
	class VobScreenFX : public DerivedWorldObject
	{
	public:
		VobScreenFX(ZenFactory* _creator);
		virtual ~VobScreenFX();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class VobScreenFXFactory : public ZenFactory
	{
	public:
		VobScreenFXFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_SCREEN_FX_H
