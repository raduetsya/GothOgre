#ifndef ICU_UNICODE_SUPPORT_H
#define ICU_UNICODE_SUPPORT_H

#include "StringHolder.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	class ICUUnicodeSupport : public UnicodeSupport
	{
	public:
		ICUUnicodeSupport();
		~ICUUnicodeSupport();

		const String& getName() const;

		/** Converts a unicode string to upper-case. */
		void toUpperCase(String& _str);

		/** Converts a unicode string to lower-case. */
		void toLowerCase(String& _str);

		/** Compares two strings case-insensitive. The strings is treated as in UTF-8. 
		The function returns 0 if the strings are equal,
		-1 if the first string is lesser than the second string,
		and 1 if the first string is greater than the second string. */
		int compareNoCase(const String& _first, const String& _second);

		/** Returns a list of all available encodings. */
		const StringVector& getAllEncodings();

		/** Opens a converter to convert strings after. */
		CodepageConverter* openConverterImpl(const String& _encoding);

	private:
		template<int CharSize>
		void _toUpperCase(StringHolder<CharSize> _str);

		template<>
		void _toUpperCase<1>(StringHolder<1> _str);

		template<>
		void _toUpperCase<2>(StringHolder<2> _str);


		template<int CharSize>
		void _toLowerCase(StringHolder<CharSize> _str);

		template<>
		void _toLowerCase<1>(StringHolder<1> _str);

		template<>
		void _toLowerCase<2>(StringHolder<2> _str);

		
		template<int CharSize>
		int _compareNoCase(ConstStringHolder<CharSize> _first, ConstStringHolder<CharSize> _second);

		template<>
		int _compareNoCase<1>(ConstStringHolder<1> _first, ConstStringHolder<1> _second);

		template<>
		int _compareNoCase<2>(ConstStringHolder<2> _first, ConstStringHolder<2> _second);

	private:
		void buildListEncodings();

		template<int CharSize>
		void _addEncoding(const char* _encoding);

		template<>
		void _addEncoding<1>(const char* _encoding);

		template<>
		void _addEncoding<2>(const char* _encoding);

		template<int CharSize>
		UConverter* _openConverter(ConstStringHolder<CharSize> _encoding);

		template<>
		UConverter* _openConverter<1>(ConstStringHolder<1> _encoding);

		template<>
		UConverter* _openConverter<2>(ConstStringHolder<2> _encoding);
	};

} // namespace GothOgre

#endif // ICU_UNICODE_SUPPORT_H