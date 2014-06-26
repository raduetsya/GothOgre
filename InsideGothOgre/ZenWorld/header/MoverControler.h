#ifndef MOVER_CONTROLER_H
#define MOVER_CONTROLER_H

namespace GothOgre
{
	/***/
	class MoverControler : public DerivedWorldObject
	{
	public:
		MoverControler(ZenFactory* _creator);
		virtual ~MoverControler();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MoverControlerFactory : public ZenFactory
	{
	public:
		MoverControlerFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOVER_CONTROLER_H
