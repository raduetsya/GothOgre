#include "Precompiled.h"
#include "MobLadder.h"

namespace GothOgre
{
	MobLadder::MobLadder(ZenFactory* _creator)
		: DerivedWorldObject("MobInter")
	{
	}
	//-------------------------------------------------------------------
	MobLadder::~MobLadder()
	{
	}
	//-------------------------------------------------------------------
	void MobLadder::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobLadder::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobLadderFactory::MobLadderFactory()
		: ZenFactory("MobLadder")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobLadderFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobLadder:oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobLadderFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobLadderFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x8B01);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobLadderFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobLadderFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobLadder(this);
	}

} // namespace GothOgre