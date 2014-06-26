#ifndef VOB_LIGHT_H
#define VOB_LIGHT_H

namespace GothOgre
{
	/***/
	class VobLight : public DerivedWorldObject
	{
	public:
		VobLight(ZenFactory* _creator);
		virtual ~VobLight();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class VobLightFactory : public ZenFactory
	{
	public:
		VobLightFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_LIGHT_H
