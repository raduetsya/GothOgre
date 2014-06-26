#include "Precompiled.h"
#include "VobScreenFX.h"

namespace GothOgre
{
	VobScreenFX::VobScreenFX(ZenFactory* _creator)
		: DerivedWorldObject("VobAnimate")
	{
	}
	//-------------------------------------------------------------------
	VobScreenFX::~VobScreenFX()
	{
	}
	//-------------------------------------------------------------------
	void VobScreenFX::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobScreenFX::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobScreenFXFactory::VobScreenFXFactory()
		: ZenFactory("VobScreenFX")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobScreenFXFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobScreenFX:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobScreenFXFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobScreenFXFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobScreenFXFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobScreenFXFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobScreenFX(this);
	}

} // namespace GothOgre