#ifndef VOB_STAIR_H
#define VOB_STAIR_H

namespace GothOgre
{
	/***/
	class VobStair : public DerivedWorldObject
	{
	public:
		VobStair(ZenFactory* _creator);
		virtual ~VobStair();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class VobStairFactory : public ZenFactory
	{
	public:
		VobStairFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_STAIR_H
