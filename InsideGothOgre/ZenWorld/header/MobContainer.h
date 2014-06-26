#ifndef MOB_CONTAINER_H
#define MOB_CONTAINER_H

namespace GothOgre
{
	/***/
	class MobContainer : public DerivedWorldObject
	{
	public:
		MobContainer(ZenFactory* _creator);
		virtual ~MobContainer();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobContainerFactory : public ZenFactory
	{
	public:
		MobContainerFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_CONTAINER_H
