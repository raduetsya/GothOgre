#include "Precompiled.h"
#include "MoverControler.h"

namespace GothOgre
{
	MoverControler::MoverControler(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	MoverControler::~MoverControler()
	{
	}
	//-------------------------------------------------------------------
	void MoverControler::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MoverControler::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MoverControlerFactory::MoverControlerFactory()
		: ZenFactory("MoverControler")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MoverControlerFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCMoverControler:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MoverControlerFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MoverControlerFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x0000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MoverControlerFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MoverControlerFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MoverControler(this);
	}

} // namespace GothOgre