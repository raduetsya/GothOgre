#include "Precompiled.h"
#include "VobStair.h"

namespace GothOgre
{
	VobStair::VobStair(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	VobStair::~VobStair()
	{
	}
	//-------------------------------------------------------------------
	void VobStair::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobStair::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobStairFactory::VobStairFactory()
		: ZenFactory("VobStair")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobStairFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobStair:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobStairFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobStairFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobStairFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobStairFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobStair(this);
	}

} // namespace GothOgre