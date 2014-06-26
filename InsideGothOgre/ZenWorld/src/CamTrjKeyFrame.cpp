#include "Precompiled.h"
#include "CamTrjKeyFrame.h"

namespace GothOgre
{
	CamTrjKeyFrame::CamTrjKeyFrame(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
	}
	//-------------------------------------------------------------------
	CamTrjKeyFrame::~CamTrjKeyFrame()
	{
	}
	//-------------------------------------------------------------------
	void CamTrjKeyFrame::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	void CamTrjKeyFrame::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	CamTrjKeyFrameFactory::CamTrjKeyFrameFactory()
		: ZenFactory("CamTrjKeyFrame")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& CamTrjKeyFrameFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCCamTrj_KeyFrame:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& CamTrjKeyFrameFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& CamTrjKeyFrameFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x8700);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort CamTrjKeyFrameFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr CamTrjKeyFrameFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new CamTrjKeyFrame(this);
	}

} // namespace GothOgre