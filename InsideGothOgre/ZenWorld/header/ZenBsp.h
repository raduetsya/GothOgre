#ifndef ZEN_BSP_H
#define ZEN_BSP_H

namespace GothOgre
{
	/***/
	class ZenBsp : public DerivedWorldObject
	{
	public:
		ZenBsp();
		virtual ~ZenBsp();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);

	private:
		void destroyVisual();
		void updateVisual();

	private:
		MeshPtr	                    mBspMesh;
		SceneManagerEx::EntityList  mEntityList;
		PortalSystem::PortalList    mPortalList;
	};

	/***/
	class ZenBspFactory : public ZenFactory
	{
	public:
		ZenBspFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ZEN_BSP_H