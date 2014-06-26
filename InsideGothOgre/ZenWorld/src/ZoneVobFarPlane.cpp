#include "Precompiled.h"
#include "ZoneVobFarPlane.h"

namespace GothOgre
{
	ZoneVobFarPlane::ZoneVobFarPlane(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	ZoneVobFarPlane::~ZoneVobFarPlane()
	{
	}
	//-------------------------------------------------------------------
	void ZoneVobFarPlane::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void ZoneVobFarPlane::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	ZoneVobFarPlaneFactory::ZoneVobFarPlaneFactory()
		: ZenFactory("ZoneVobFarPlane")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZoneVobFarPlaneFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCZoneVobFarPlane:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZoneVobFarPlaneFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZoneVobFarPlaneFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZoneVobFarPlaneFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZoneVobFarPlaneFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZoneVobFarPlane(this);
	}

} // namespace GothOgre