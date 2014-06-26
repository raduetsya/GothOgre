#ifndef TRIGGER_LIST_H
#define TRIGGER_LIST_H

namespace GothOgre
{
	/***/
	class TriggerList : public DerivedWorldObject
	{
	public:
		TriggerList(ZenFactory* _creator);
		virtual ~TriggerList();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class TriggerListFactory : public ZenFactory
	{
	public:
		TriggerListFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // TRIGGER_LIST_H
