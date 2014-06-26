#include "Precompiled.h"
#include "Item.h"

namespace GothOgre
{
	Item::Item(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	Item::~Item()
	{
	}
	//-------------------------------------------------------------------
	void Item::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void Item::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	ItemFactory::ItemFactory()
		: ZenFactory("Item")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ItemFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCItem:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ItemFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ItemFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x0000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ItemFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ItemFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new Item(this);
	}

} // namespace GothOgre