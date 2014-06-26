#include "Precompiled.h"
#include "ZoneVobFarPlaneDefault.h"

namespace GothOgre
{
	ZoneVobFarPlaneDefault::ZoneVobFarPlaneDefault(ZenFactory* _creator)
		: DerivedWorldObject("ZoneVobFarPlane")
	{
	}
	//-------------------------------------------------------------------
	ZoneVobFarPlaneDefault::~ZoneVobFarPlaneDefault()
	{
	}
	//-------------------------------------------------------------------
	void ZoneVobFarPlaneDefault::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void ZoneVobFarPlaneDefault::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	ZoneVobFarPlaneDefaultFactory::ZoneVobFarPlaneDefaultFactory()
		: ZenFactory("ZoneVobFarPlaneDefault")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZoneVobFarPlaneDefaultFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCZoneVobFarPlaneDefault:zCZoneVobFarPlane:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZoneVobFarPlaneDefaultFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZoneVobFarPlaneDefaultFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZoneVobFarPlaneDefaultFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZoneVobFarPlaneDefaultFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZoneVobFarPlaneDefault(this);
	}

} // namespace GothOgre