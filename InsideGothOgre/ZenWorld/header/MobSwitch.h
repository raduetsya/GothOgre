#ifndef MOB_SWITCH_H
#define MOB_SWITCH_H

namespace GothOgre
{
	/***/
	class MobSwitch : public DerivedWorldObject
	{
	public:
		MobSwitch(ZenFactory* _creator);
		virtual ~MobSwitch();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobSwitchFactory : public ZenFactory
	{
	public:
		MobSwitchFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_SWITCH_H
