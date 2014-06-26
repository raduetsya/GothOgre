#include "Precompiled.h"
#include "CodeMaster.h"

namespace GothOgre
{
	CodeMaster::CodeMaster(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	CodeMaster::~CodeMaster()
	{
	}
	//-------------------------------------------------------------------
	void CodeMaster::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void CodeMaster::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	CodeMasterFactory::CodeMasterFactory()
		: ZenFactory("CodeMaster")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& CodeMasterFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCCodeMaster:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& CodeMasterFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& CodeMasterFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x0000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort CodeMasterFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr CodeMasterFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new CodeMaster(this);
	}

} // namespace GothOgre