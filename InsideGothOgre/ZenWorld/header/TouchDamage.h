#ifndef TOUCH_DAMAGE_H
#define TOUCH_DAMAGE_H

namespace GothOgre
{
	/***/
	class TouchDamage : public DerivedWorldObject
	{
	public:
		TouchDamage(ZenFactory* _creator);
		virtual ~TouchDamage();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class TouchDamageFactory : public ZenFactory
	{
	public:
		TouchDamageFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // TOUCH_DAMAGE_H
