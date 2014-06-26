#ifndef MESSAGE_FILTER_H
#define MESSAGE_FILTER_H

namespace GothOgre
{
	/***/
	class MessageFilter : public DerivedWorldObject
	{
	public:
		MessageFilter(ZenFactory* _creator);
		virtual ~MessageFilter();

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);
	};

	/***/
	class MessageFilterFactory : public ZenFactory
	{
	public:
		MessageFilterFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // MESSAGE_FILTER_H
