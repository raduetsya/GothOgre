#ifndef ZENWORLD_H
#define ZENWORLD_H

namespace GothOgre
{
	/***/
	class ZenWorld : public World
	{
	public:
		ZenWorld();
		virtual ~ZenWorld();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);

	private:
		void readVobTree(ZenArchive* _zenArchive);
		void _readVobTree(ZenArchive* _zenArchive, const WorldObjectPtr& _root);

	private:
		int mChildID;
	};

	/***/
	class ZenWorldFactory : public ZenFactory
	{
	public:
		ZenWorldFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
		virtual ZenObjectPtr createZenObject();
	};

} // namespace GothOgre

#endif // ZENWORLD_H