#include "Precompiled.h"
#include "MobWheel.h"

namespace GothOgre
{
	MobWheel::MobWheel(ZenFactory* _creator)
		: DerivedWorldObject("MobInter")
	{
	}
	//-------------------------------------------------------------------
	MobWheel::~MobWheel()
	{
	}
	//-------------------------------------------------------------------
	void MobWheel::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobWheel::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobWheelFactory::MobWheelFactory()
		: ZenFactory("MobWheel")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobWheelFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobWheel:oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobWheelFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobWheelFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x8B01);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobWheelFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobWheelFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobWheel(this);
	}

} // namespace GothOgre