#include "Precompiled.h"
#include "ZoneZFogDefault.h"

namespace GothOgre
{
	ZoneZFogDefault::ZoneZFogDefault(ZenFactory* _creator)
		: DerivedWorldObject("ZoneZFog")
	{
	}
	//-------------------------------------------------------------------
	ZoneZFogDefault::~ZoneZFogDefault()
	{
	}
	//-------------------------------------------------------------------
	void ZoneZFogDefault::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void ZoneZFogDefault::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	ZoneZFogDefaultFactory::ZoneZFogDefaultFactory()
		: ZenFactory("ZoneZFogDefault")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZoneZFogDefaultFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCZoneZFogDefault:zCZoneZFog:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZoneZFogDefaultFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZoneZFogDefaultFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xFCC0);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZoneZFogDefaultFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZoneZFogDefaultFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZoneZFogDefault(this);
	}

} // namespace GothOgre