#include "Precompiled.h"
#include "ZoneZFog.h"

namespace GothOgre
{
	ZoneZFog::ZoneZFog(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	ZoneZFog::~ZoneZFog()
	{
	}
	//-------------------------------------------------------------------
	void ZoneZFog::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void ZoneZFog::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	ZoneZFogFactory::ZoneZFogFactory()
		: ZenFactory("ZoneZFog")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZoneZFogFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCZoneZFog:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZoneZFogFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZoneZFogFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xFCC0);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZoneZFogFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZoneZFogFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZoneZFog(this);
	}

} // namespace GothOgre