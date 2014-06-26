#ifndef MOB_BED_H
#define MOB_BED_H

namespace GothOgre
{
	/***/
	class MobBed : public DerivedWorldObject
	{
	public:
		MobBed(ZenFactory* _creator);
		virtual ~MobBed();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobBedFactory : public ZenFactory
	{
	public:
		MobBedFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_BED_H
