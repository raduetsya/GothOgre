#ifndef MOB_WHEEL_H
#define MOB_WHEEL_H

namespace GothOgre
{
	/***/
	class MobWheel : public DerivedWorldObject
	{
	public:
		MobWheel(ZenFactory* _creator);
		virtual ~MobWheel();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MobWheelFactory : public ZenFactory
	{
	public:
		MobWheelFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOB_WHEEL_H
