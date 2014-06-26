#ifndef CODE_MASTER_H
#define CODE_MASTER_H

namespace GothOgre
{
	/***/
	class CodeMaster : public DerivedWorldObject
	{
	public:
		CodeMaster(ZenFactory* _creator);
		virtual ~CodeMaster();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class CodeMasterFactory : public ZenFactory
	{
	public:
		CodeMasterFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // CODE_MASTER_H
