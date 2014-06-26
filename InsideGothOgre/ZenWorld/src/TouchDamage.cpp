#include "Precompiled.h"
#include "TouchDamage.h"

namespace GothOgre
{
	TouchDamage::TouchDamage(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	TouchDamage::~TouchDamage()
	{
	}
	//-------------------------------------------------------------------
	void TouchDamage::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void TouchDamage::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	TouchDamageFactory::TouchDamageFactory()
		: ZenFactory("TouchDamage")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& TouchDamageFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCTouchDamage:zCTouchDamage:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& TouchDamageFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& TouchDamageFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x9001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort TouchDamageFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr TouchDamageFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new TouchDamage(this);
	}

} // namespace GothOgre