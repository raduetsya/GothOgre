#ifndef CAM_TRJ_KEY_FRAME_H
#define CAM_TRJ_KEY_FRAME_H

namespace GothOgre
{
	/***/
	class CamTrjKeyFrame : public DerivedWorldObject
	{
	public:
		CamTrjKeyFrame(ZenFactory* _creator);
		virtual ~CamTrjKeyFrame();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class CamTrjKeyFrameFactory : public ZenFactory
	{
	public:
		CamTrjKeyFrameFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // CAM_TRJ_KEY_FRAME_H