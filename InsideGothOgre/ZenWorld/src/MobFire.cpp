#include "Precompiled.h"
#include "MobFire.h"

namespace GothOgre
{
	MobFire::MobFire(ZenFactory* _creator)
		: DerivedWorldObject("MobInter")
	{
	}
	//-------------------------------------------------------------------
	MobFire::~MobFire()
	{
	}
	//-------------------------------------------------------------------
	void MobFire::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobFire::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobFireFactory::MobFireFactory()
		: ZenFactory("MobFire")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobFireFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobFire:oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobFireFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobFireFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x4801);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobFireFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobFireFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobFire(this);
	}

} // namespace GothOgre