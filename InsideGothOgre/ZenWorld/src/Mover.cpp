#include "Precompiled.h"
#include "Mover.h"

namespace GothOgre
{
	Mover::Mover(ZenFactory* _creator)
		: DerivedWorldObject("Trigger")
	{
	}
	//-------------------------------------------------------------------
	Mover::~Mover()
	{
	}
	//-------------------------------------------------------------------
	void Mover::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void Mover::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MoverFactory::MoverFactory()
		: ZenFactory("Mover")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MoverFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCMover:zCTrigger:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MoverFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MoverFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x9C00);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MoverFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MoverFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new Mover(this);
	}

} // namespace GothOgre