#include "Precompiled.h"
#include "VobAnimate.h"

namespace GothOgre
{
	VobAnimate::VobAnimate(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	VobAnimate::~VobAnimate()
	{
	}
	//-------------------------------------------------------------------
	void VobAnimate::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void VobAnimate::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobAnimateFactory::VobAnimateFactory()
		: ZenFactory("VobAnimate")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobAnimateFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobAnimate:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobAnimateFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobAnimateFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobAnimateFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobAnimateFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobAnimate(this);
	}

} // namespace GothOgre