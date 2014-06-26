#ifndef ITEM_H
#define ITEM_H

namespace GothOgre
{
	/***/
	class Item : public DerivedWorldObject
	{
	public:
		Item(ZenFactory* _creator);
		virtual ~Item();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class ItemFactory : public ZenFactory
	{
	public:
		ItemFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // ITEM_H
