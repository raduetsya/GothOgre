#include "Precompiled.h"
#include "CSCamera.h"

namespace GothOgre
{
	CSCamera::CSCamera(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	CSCamera::~CSCamera()
	{
	}
	//-------------------------------------------------------------------
	void CSCamera::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void CSCamera::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	CSCameraFactory::CSCameraFactory()
		: ZenFactory("CSCamera")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& CSCameraFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCCSCamera:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& CSCameraFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& CSCameraFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x7800);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort CSCameraFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr CSCameraFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new CSCamera(this);
	}

} // namespace GothOgre