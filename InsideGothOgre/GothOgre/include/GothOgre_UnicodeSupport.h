#ifndef GOTHOGRE_UNICODE_SUPPORT_H
#define GOTHOGRE_UNICODE_SUPPORT_H


namespace GothOgre
{
	class CodepageConverter;


	//-----------------------------------------------------------------------
	// Auxiliary class.
	// This class is designed to implement unicode support.
	// Don't use it directly until you create a new implementation of unicode support.
	class GOTHOGRE_EXPORT UnicodeSupport
	{
	public:
		UnicodeSupport();
		virtual ~UnicodeSupport();

		virtual const String& getName() const = 0;

		/** Converts an unicode string to upper-case. */
		virtual void toUpperCase(String& _str) = 0;

		/** Converts an unicode string to lower-case. */
		virtual void toLowerCase(String& _str) = 0;

		/** Compares two strings case-insensitive. The strings is treated as in UTF-8. 
		The function returns 0 if the strings are equal,
		-1 if the first string is lesser than the second string,
		and 1 if the first string is greater than the second string. */
		virtual int compareNoCase(const String& _first, const String& _second) = 0;

		/** Returns a list of all available encodings. */
		const StringVector& getAllEncodings();

		/** Opens a converter to convert strings after. */
		CodepageConverter* openConverter(const String& _encoding);

	public:
		// Privately used by implementations of UnicodeSupport
		static void _throwEncodingNotFound(const String& _encoding);
		static void _throwCouldNotConvert(const String& _encodingTo, const String& _encodingFrom, const void* _data, size_t _size);

	protected:
		virtual CodepageConverter* openConverterImpl(const String& _encoding) = 0;
		virtual void buildListEncodings() = 0;
		void addEncoding(const String& _encoding);

	private:
		void deleteSharedConverters();
		static bool lessEncoding(const String& _first, const String& _second);

	private:
		typedef map<String, CodepageConverter*>::type  ConverterByEncoding;

		ConverterByEncoding     mConverterByEncoding;
		StringVector            mAllEncodings;
		OGRE_RW_MUTEX(		    mAllEncodingsMutex);
		OGRE_RW_MUTEX(		    mConverterByEncodingMutex);
	};



	//-----------------------------------------------------------------------
	// Auxiliary class.
	// This class is designed to implement unicode support.
	// Don't use it directly until you create a new implementation of unicode support.
	class GOTHOGRE_EXPORT UnicodeSupportRegistration : public Singleton<UnicodeSupportRegistration>
	{
	public:
		UnicodeSupportRegistration();
		~UnicodeSupportRegistration();

		// Returns a pointer to the registered implementation of unicode support
		UnicodeSupport* getSupport() const;

	private:
		friend class UnicodeSupport;
		void registerSupport(UnicodeSupport* _support);
		void unregisterSupport(UnicodeSupport* _support);

	private:
		UnicodeSupport* mSupport;
	};


} // namespace GothOgre

#endif // GOTHOGRE_UNICODE_SUPPORT_H