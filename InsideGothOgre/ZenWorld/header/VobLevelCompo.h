#ifndef VOB_LEVEL_COMPO_H
#define VOB_LEVEL_COMPO_H

namespace GothOgre
{
	/***/
	class VobLevelCompo : public DerivedWorldObject
	{
	public:
		VobLevelCompo(ZenFactory* _creator);
		virtual ~VobLevelCompo();

		void setZenBspName(const String& _zenBspName);
		const String& getZenBspName() const;
		bool isCompiled() const;

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);

	private:
		WorldObjectPtr  mZenBsp;
	};

	/***/
	class VobLevelCompoFactory : public ZenFactory
	{
	public:
		VobLevelCompoFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_LEVEL_COMPO_H
