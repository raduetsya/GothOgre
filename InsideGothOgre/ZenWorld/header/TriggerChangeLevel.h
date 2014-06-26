#ifndef TRIGGER_CHANGE_LEVEL_H
#define TRIGGER_CHANGE_LEVEL_H

namespace GothOgre
{
	/***/
	class TriggerChangeLevel : public DerivedWorldObject
	{
	public:
		TriggerChangeLevel(ZenFactory* _creator);
		virtual ~TriggerChangeLevel();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class TriggerChangeLevelFactory : public ZenFactory
	{
	public:
		TriggerChangeLevelFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // TRIGGER_CHANGE_LEVEL_H
