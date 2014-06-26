#ifndef MOB_FIRE_H
#define MOB_FIRE_H

namespace GothOgre
{
	/***/
	class MobFire : public DerivedWorldObject
	{
	public:
		MobFire(ZenFactory* _creator);
		virtual ~MobFire();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobFireFactory : public ZenFactory
	{
	public:
		MobFireFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_FIRE_H
