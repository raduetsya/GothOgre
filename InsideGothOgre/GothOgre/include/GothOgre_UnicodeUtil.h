#ifndef GOTHOGRE_UNICODE_UTIL_H
#define GOTHOGRE_UNICODE_UTIL_H

#include "GothOgre_CodepageConverter.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	// Front-end for all functions working with unicode
	class GOTHOGRE_EXPORT UnicodeUtil
	{
	public:
		// Makes the specified string upper-case. Strings are treated as UTF-8.
		static String& toUpperCase(String& _inOutString);
			
		// Makes the specified string low-case. Strings are treated as UTF-8.
		static String& toLowerCase(String& _inOutString);

		// Compares two strings case-insensivite. Strings are treated as UTF-8.
		static int compareNoCase(const String& _text1, const String& _text2);
		static bool equalsNoCase(const String& _text1, const String& _text2);

		/** Returns a list of all available encodings. */
		static const StringVector& getAllEncodings();

		/** Opens a converter to convert strings after. */
		static CodepageConverter* openConverter(const String& _encoding);
	};

} // namespace GothOgre

#endif // GOTHOGRE_UNICODE_UTIL_H