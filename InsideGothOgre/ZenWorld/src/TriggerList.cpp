#include "Precompiled.h"
#include "TriggerList.h"

namespace GothOgre
{
	TriggerList::TriggerList(ZenFactory* _creator)
		: DerivedWorldObject("Trigger")
	{
	}
	//-------------------------------------------------------------------
	TriggerList::~TriggerList()
	{
	}
	//-------------------------------------------------------------------
	void TriggerList::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void TriggerList::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	TriggerListFactory::TriggerListFactory()
		: ZenFactory("TriggerList")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& TriggerListFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCTriggerList:zCTrigger:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& TriggerListFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& TriggerListFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xE980);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort TriggerListFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr TriggerListFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new TriggerList(this);
	}

} // namespace GothOgre