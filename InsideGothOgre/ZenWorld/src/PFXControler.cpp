#include "Precompiled.h"
#include "PFXControler.h"

namespace GothOgre
{
	PFXControler::PFXControler(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	PFXControler::~PFXControler()
	{
	}
	//-------------------------------------------------------------------
	void PFXControler::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void PFXControler::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	PFXControlerFactory::PFXControlerFactory()
		: ZenFactory("PFXControler")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& PFXControlerFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCPFXControler:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& PFXControlerFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& PFXControlerFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort PFXControlerFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr PFXControlerFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new PFXControler(this);
	}

} // namespace GothOgre