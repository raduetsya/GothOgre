#ifndef PFX_CONTROLER_H
#define PFX_CONTROLER_H

namespace GothOgre
{
	/***/
	class PFXControler : public DerivedWorldObject
	{
	public:
		PFXControler(ZenFactory* _creator);
		virtual ~PFXControler();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class PFXControlerFactory : public ZenFactory
	{
	public:
		PFXControlerFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // PFX_CONTROLER_H
