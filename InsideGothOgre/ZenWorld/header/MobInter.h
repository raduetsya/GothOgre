#ifndef MOB_INTER_H
#define MOB_INTER_H

namespace GothOgre
{
	/***/
	class MobInter : public DerivedWorldObject
	{
	public:
		MobInter(ZenFactory* _creator);
		virtual ~MobInter();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobInterFactory : public ZenFactory
	{
	public:
		MobInterFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_INTER_H
