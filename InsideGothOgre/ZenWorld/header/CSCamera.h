#ifndef CSCAMERA_H
#define CSCAMERA_H

namespace GothOgre
{
	/***/
	class CSCamera : public DerivedWorldObject
	{
	public:
		CSCamera(ZenFactory* _creator);
		virtual ~CSCamera();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class CSCameraFactory : public ZenFactory
	{
	public:
		CSCameraFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // CSCAMERA_H
