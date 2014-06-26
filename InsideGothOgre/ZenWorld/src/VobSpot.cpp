#include "Precompiled.h"
#include "VobSpot.h"

namespace GothOgre
{
	VobSpot::VobSpot(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	VobSpot::~VobSpot()
	{
	}
	//-------------------------------------------------------------------
	void VobSpot::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobSpot::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobSpotFactory::VobSpotFactory()
		: ZenFactory("VobSpot")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobSpotFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobSpot:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobSpotFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobSpotFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobSpotFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobSpotFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobSpot(this);
	}

} // namespace GothOgre