#ifndef VOB_ANIMATE_H
#define VOB_ANIMATE_H

namespace GothOgre
{
	/***/
	class VobAnimate : public DerivedWorldObject
	{
	public:
		VobAnimate(ZenFactory* _creator);
		virtual ~VobAnimate();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class VobAnimateFactory : public ZenFactory
	{
	public:
		VobAnimateFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_ANIMATE_H
