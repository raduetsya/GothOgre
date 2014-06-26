#include "Precompiled.h"
#include "TriggerScript.h"

namespace GothOgre
{
	TriggerScript::TriggerScript(ZenFactory* _creator)
		: DerivedWorldObject("Trigger")
	{
	}
	//-------------------------------------------------------------------
	TriggerScript::~TriggerScript()
	{
	}
	//-------------------------------------------------------------------
	void TriggerScript::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void TriggerScript::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	TriggerScriptFactory::TriggerScriptFactory()
		: ZenFactory("TriggerScript")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& TriggerScriptFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCTriggerScript:zCTrigger:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& TriggerScriptFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& TriggerScriptFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x6001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort TriggerScriptFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr TriggerScriptFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new TriggerScript(this);
	}

} // namespace GothOgre