#ifndef MOVER_H
#define MOVER_H

namespace GothOgre
{
	/***/
	class Mover : public DerivedWorldObject
	{
	public:
		Mover(ZenFactory* _creator);
		virtual ~Mover();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MoverFactory : public ZenFactory
	{
	public:
		MoverFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MOVER_H
