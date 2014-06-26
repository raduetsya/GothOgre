#include "Precompiled.h"
#include "TriggerWorldStart.h"

namespace GothOgre
{
	TriggerWorldStart::TriggerWorldStart(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	TriggerWorldStart::~TriggerWorldStart()
	{
	}
	//-------------------------------------------------------------------
	void TriggerWorldStart::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void TriggerWorldStart::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	TriggerWorldStartFactory::TriggerWorldStartFactory()
		: ZenFactory("TriggerWorldStart")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& TriggerWorldStartFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCTriggerWorldStart:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& TriggerWorldStartFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& TriggerWorldStartFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort TriggerWorldStartFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr TriggerWorldStartFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new TriggerWorldStart(this);
	}

} // namespace GothOgre