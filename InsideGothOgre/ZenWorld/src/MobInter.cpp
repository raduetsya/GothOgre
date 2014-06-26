#include "Precompiled.h"
#include "MobInter.h"

namespace GothOgre
{
	MobInter::MobInter(ZenFactory* _creator)
		: DerivedWorldObject("Mob")
	{
	}
	//-------------------------------------------------------------------
	MobInter::~MobInter()
	{
	}
	//-------------------------------------------------------------------
	void MobInter::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobInter::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobInterFactory::MobInterFactory()
		: ZenFactory("MobInter")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobInterFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobInterFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobInterFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x8B01);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobInterFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobInterFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobInter(this);
	}

} // namespace GothOgre