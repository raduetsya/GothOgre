#ifndef MOB_H
#define MOB_H

namespace GothOgre
{
	/***/
	class Mob : public DerivedWorldObject
	{
	public:
		Mob(ZenFactory* _creator);
		virtual ~Mob();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobFactory : public ZenFactory
	{
	public:
		MobFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_H
