#ifndef VOB_STARTPOINT_H
#define VOB_STARTPOINT_H

namespace GothOgre
{
	/***/
	class VobStartpoint : public DerivedWorldObject
	{
	public:
		VobStartpoint(ZenFactory* _creator);
		virtual ~VobStartpoint();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class VobStartpointFactory : public ZenFactory
	{
	public:
		VobStartpointFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_STARTPOINT_H
