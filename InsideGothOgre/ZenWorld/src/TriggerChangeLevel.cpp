#include "Precompiled.h"
#include "TriggerChangeLevel.h"

namespace GothOgre
{
	TriggerChangeLevel::TriggerChangeLevel(ZenFactory* _creator)
		: DerivedWorldObject("Trigger")
	{
	}
	//-------------------------------------------------------------------
	TriggerChangeLevel::~TriggerChangeLevel()
	{
	}
	//-------------------------------------------------------------------
	void TriggerChangeLevel::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void TriggerChangeLevel::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	TriggerChangeLevelFactory::TriggerChangeLevelFactory()
		: ZenFactory("TriggerChangeLevel")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& TriggerChangeLevelFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCTriggerChangeLevel:zCTrigger:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& TriggerChangeLevelFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& TriggerChangeLevelFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x6001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort TriggerChangeLevelFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr TriggerChangeLevelFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new TriggerChangeLevel(this);
	}

} // namespace GothOgre