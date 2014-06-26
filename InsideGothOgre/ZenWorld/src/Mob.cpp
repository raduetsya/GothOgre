#include "Precompiled.h"
#include "Mob.h"

namespace GothOgre
{
	Mob::Mob(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	Mob::~Mob()
	{
	}
	//-------------------------------------------------------------------
	void Mob::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void Mob::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobFactory::MobFactory()
		: ZenFactory("Mob")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xB801);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new Mob(this);
	}

} // namespace GothOgre