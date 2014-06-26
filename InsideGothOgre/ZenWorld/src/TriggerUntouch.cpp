#include "Precompiled.h"
#include "TriggerUntouch.h"

namespace GothOgre
{
	TriggerUntouch::TriggerUntouch(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	TriggerUntouch::~TriggerUntouch()
	{
	}
	//-------------------------------------------------------------------
	void TriggerUntouch::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void TriggerUntouch::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	TriggerUntouchFactory::TriggerUntouchFactory()
		: ZenFactory("TriggerUntouch")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& TriggerUntouchFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCTriggerUntouch:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& TriggerUntouchFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& TriggerUntouchFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xFCC0);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort TriggerUntouchFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr TriggerUntouchFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new TriggerUntouch(this);
	}

} // namespace GothOgre