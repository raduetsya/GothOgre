#ifndef TRIGGER_H
#define TRIGGER_H

namespace GothOgre
{
	/***/
	class Trigger : public DerivedWorldObject
	{
	public:
		Trigger(ZenFactory* _creator);
		virtual ~Trigger();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class TriggerFactory : public ZenFactory
	{
	public:
		TriggerFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // TRIGGER_H
