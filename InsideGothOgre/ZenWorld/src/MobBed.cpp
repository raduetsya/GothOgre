#include "Precompiled.h"
#include "MobBed.h"

namespace GothOgre
{
	MobBed::MobBed(ZenFactory* _creator)
		: DerivedWorldObject("MobInter")
	{
	}
	//-------------------------------------------------------------------
	MobBed::~MobBed()
	{
	}
	//-------------------------------------------------------------------
	void MobBed::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobBed::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobBedFactory::MobBedFactory()
		: ZenFactory("MobBed")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobBedFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobBed:oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobBedFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobBedFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xF000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobBedFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobBedFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobBed(this);
	}

} // namespace GothOgre