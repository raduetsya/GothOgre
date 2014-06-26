#ifndef MOB_LADDER_H
#define MOB_LADDER_H

namespace GothOgre
{
	/***/
	class MobLadder : public DerivedWorldObject
	{
	public:
		MobLadder(ZenFactory* _creator);
		virtual ~MobLadder();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobLadderFactory : public ZenFactory
	{
	public:
		MobLadderFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_LADDER_H
