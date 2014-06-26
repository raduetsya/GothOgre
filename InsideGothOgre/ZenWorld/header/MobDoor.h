#ifndef MOB_DOOR_H
#define MOB_DOOR_H

namespace GothOgre
{
	/***/
	class MobDoor : public DerivedWorldObject
	{
	public:
		MobDoor(ZenFactory* _creator);
		virtual ~MobDoor();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobDoorFactory : public ZenFactory
	{
	public:
		MobDoorFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_DOOR_H
