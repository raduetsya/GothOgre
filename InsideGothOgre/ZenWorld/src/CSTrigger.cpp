#include "Precompiled.h"
#include "CSTrigger.h"

namespace GothOgre
{
	CSTrigger::CSTrigger(ZenFactory* _creator)
		: DerivedWorldObject("Trigger")
	{
	}
	//-------------------------------------------------------------------
	CSTrigger::~CSTrigger()
	{
	}
	//-------------------------------------------------------------------
	void CSTrigger::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void CSTrigger::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	CSTriggerFactory::CSTriggerFactory()
		: ZenFactory("CSTrigger")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& CSTriggerFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCCSTrigger:zCTrigger:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& CSTriggerFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& CSTriggerFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x6001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort CSTriggerFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr CSTriggerFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new CSTrigger(this);
	}

} // namespace GothOgre