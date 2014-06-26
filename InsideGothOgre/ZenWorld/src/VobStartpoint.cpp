#include "Precompiled.h"
#include "VobStartpoint.h"

namespace GothOgre
{
	VobStartpoint::VobStartpoint(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	VobStartpoint::~VobStartpoint()
	{
	}
	//-------------------------------------------------------------------
	void VobStartpoint::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobStartpoint::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobStartpointFactory::VobStartpointFactory()
		: ZenFactory("VobStartpoint")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobStartpointFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobStartpoint:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobStartpointFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobStartpointFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobStartpointFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobStartpointFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobStartpoint(this);
	}

} // namespace GothOgre