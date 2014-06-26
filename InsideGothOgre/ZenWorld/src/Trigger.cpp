#include "Precompiled.h"
#include "Trigger.h"

namespace GothOgre
{
	Trigger::Trigger(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	Trigger::~Trigger()
	{
	}
	//-------------------------------------------------------------------
	void Trigger::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void Trigger::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	TriggerFactory::TriggerFactory()
		: ZenFactory("Trigger")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& TriggerFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCTrigger:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& TriggerFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& TriggerFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xB801);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort TriggerFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr TriggerFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new Trigger(this);
	}

} // namespace GothOgre