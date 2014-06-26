#include "Precompiled.h"
#include "VobLensFlare.h"

namespace GothOgre
{
	VobLensFlare::VobLensFlare(ZenFactory* _creator)
		: DerivedWorldObject("VobAnimate")
	{
	}
	//-------------------------------------------------------------------
	VobLensFlare::~VobLensFlare()
	{
	}
	//-------------------------------------------------------------------
	void VobLensFlare::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobLensFlare::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobLensFlareFactory::VobLensFlareFactory()
		: ZenFactory("VobLensFlare")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobLensFlareFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobLensFlare:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobLensFlareFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobLensFlareFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xFCC0);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobLensFlareFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobLensFlareFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobLensFlare(this);
	}

} // namespace GothOgre