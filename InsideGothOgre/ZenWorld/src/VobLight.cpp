#include "Precompiled.h"
#include "VobLight.h"

namespace GothOgre
{
	VobLight::VobLight(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	VobLight::~VobLight()
	{
	}
	//-------------------------------------------------------------------
	void VobLight::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobLight::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobLightFactory::VobLightFactory()
		: ZenFactory("VobLight")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobLightFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobLight:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobLightFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobLightFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xB400);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobLightFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobLightFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobLight(this);
	}

} // namespace GothOgre