#ifndef TRIGGER_UNTOUCH_H
#define TRIGGER_UNTOUCH_H

namespace GothOgre
{
	/***/
	class TriggerUntouch : public DerivedWorldObject
	{
	public:
		TriggerUntouch(ZenFactory* _creator);
		virtual ~TriggerUntouch();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class TriggerUntouchFactory : public ZenFactory
	{
	public:
		TriggerUntouchFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // TRIGGER_UNTOUCH_H
