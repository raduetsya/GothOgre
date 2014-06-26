#ifndef TRIGGER_WORLD_START_H
#define TRIGGER_WORLD_START_H

namespace GothOgre
{
	/***/
	class TriggerWorldStart : public DerivedWorldObject
	{
	public:
		TriggerWorldStart(ZenFactory* _creator);
		virtual ~TriggerWorldStart();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class TriggerWorldStartFactory : public ZenFactory
	{
	public:
		TriggerWorldStartFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // TRIGGER_WORLD_START_H
