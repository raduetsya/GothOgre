#ifndef ZEN_MTL_H
#define ZEN_MTL_H

namespace GothOgre
{
	/***/
	class ZenMtl : public ZenObject
	{
	public:
		ZenMtl();
		virtual ~ZenMtl();

		/** Sets a material which must be written with following call 
		of function ZenMtl::write. */
		virtual void setName(const String& _name);

		/** Returns the name of material which has been read
		with previous call of function ZenMtl::read. */
		virtual const String& getName() const;

		/** Sets a material which must be written with following call 
		of function ZenMtl::write. */
		virtual void setSectorName(const String& _name);

		/** Returns the name of material which has been read
		with previous call of function ZenMtl::read. */
		virtual const String& getSectorName() const;

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);

	private:
		MtlPtr mMtl;
		String mSectorName;
	};

	/***/
	class ZenMtlFactory : public ZenFactory
	{
	public:
		ZenMtlFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};	

} // namespace GothOgre

#endif // ZEN_MTL_H
