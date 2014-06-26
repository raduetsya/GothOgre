#include "Precompiled.h"
#include "MessageFilter.h"

namespace GothOgre
{
	MessageFilter::MessageFilter(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	MessageFilter::~MessageFilter()
	{
	}
	//-------------------------------------------------------------------
	void MessageFilter::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void MessageFilter::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	MessageFilterFactory::MessageFilterFactory()
		: ZenFactory("MessageFilter")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& MessageFilterFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCMessageFilter:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& MessageFilterFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& MessageFilterFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x0000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort MessageFilterFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr MessageFilterFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new MessageFilter(this);
	}

} // namespace GothOgre