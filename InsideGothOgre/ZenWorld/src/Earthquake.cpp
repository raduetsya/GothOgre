#include "Precompiled.h"
#include "Earthquake.h"

namespace GothOgre
{
	Earthquake::Earthquake(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	Earthquake::~Earthquake()
	{
	}
	//-------------------------------------------------------------------
	void Earthquake::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void Earthquake::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	EarthquakeFactory::EarthquakeFactory()
		: ZenFactory("Earthquake")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& EarthquakeFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCEarthquake:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& EarthquakeFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& EarthquakeFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x0000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort EarthquakeFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr EarthquakeFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new Earthquake(this);
	}

} // namespace GothOgre