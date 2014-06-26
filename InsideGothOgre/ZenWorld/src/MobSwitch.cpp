#include "Precompiled.h"
#include "MobSwitch.h"

namespace GothOgre
{
	MobSwitch::MobSwitch(ZenFactory* _creator)
		: DerivedWorldObject("MobInter")
	{
	}
	//-------------------------------------------------------------------
	MobSwitch::~MobSwitch()
	{
	}
	//-------------------------------------------------------------------
	void MobSwitch::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobSwitch::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobSwitchFactory::MobSwitchFactory()
		: ZenFactory("MobSwitch")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobSwitchFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobSwitch:oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobSwitchFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobSwitchFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x8B01);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobSwitchFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobSwitchFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobSwitch(this);
	}

} // namespace GothOgre