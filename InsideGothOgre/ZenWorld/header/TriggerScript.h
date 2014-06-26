#ifndef TRIGGER_SCRIPT_H
#define TRIGGER_SCRIPT_H

namespace GothOgre
{
	/***/
	class TriggerScript : public DerivedWorldObject
	{
	public:
		TriggerScript(ZenFactory* _creator);
		virtual ~TriggerScript();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class TriggerScriptFactory : public ZenFactory
	{
	public:
		TriggerScriptFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // TRIGGER_SCRIPT_H
