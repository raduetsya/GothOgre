#ifndef EARTHQUAKE_H
#define EARTHQUAKE_H

namespace GothOgre
{
	/***/
	class Earthquake : public DerivedWorldObject
	{
	public:
		Earthquake(ZenFactory* _creator);
		virtual ~Earthquake();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class EarthquakeFactory : public ZenFactory
	{
	public:
		EarthquakeFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // EARTHQUAKE_H
