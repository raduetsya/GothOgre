#ifndef CSTRIGGER_H
#define CSTRIGGER_H

namespace GothOgre
{
	/***/
	class CSTrigger : public DerivedWorldObject
	{
	public:
		CSTrigger(ZenFactory* _creator);
		virtual ~CSTrigger();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class CSTriggerFactory : public ZenFactory
	{
	public:
		CSTriggerFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // CSTRIGGER_H
