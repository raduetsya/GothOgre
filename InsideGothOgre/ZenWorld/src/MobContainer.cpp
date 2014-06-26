#include "Precompiled.h"
#include "MobContainer.h"

namespace GothOgre
{
	MobContainer::MobContainer(ZenFactory* _creator)
		: DerivedWorldObject("MobInter")
	{
	}
	//-------------------------------------------------------------------
	MobContainer::~MobContainer()
	{
	}
	//-------------------------------------------------------------------
	void MobContainer::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MobContainer::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MobContainerFactory::MobContainerFactory()
		: ZenFactory("MobContainer")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MobContainerFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCMobContainer:oCMobInter:oCMOB:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MobContainerFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MobContainerFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xFC01);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MobContainerFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MobContainerFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MobContainer(this);
	}

} // namespace GothOgre