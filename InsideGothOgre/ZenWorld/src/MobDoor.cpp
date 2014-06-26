#include "Precompiled.h"
#include "MobDoor.h"

namespace GothOgre
{
	MobDoor::MobDoor(ZenFactory* _creator)
		: DerivedWorldObject("MobInter")
	{
	}
	//-------------------------------------------------------------------
	MobDoor::~MobDoor()
	{
	}
	//-------------------------------------------------------------------
	void MobDoor::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobDoor::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobDoorFactory::MobDoorFactory()
		: ZenFactory("MobDoor")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobDoorFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobDoor:oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobDoorFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobDoorFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xFC01);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobDoorFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobDoorFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobDoor(this);
	}

} // namespace GothOgre