#ifndef GOTHOGRE_STR_UTIL_H
#define GOTHOGRE_STR_UTIL_H


namespace GothOgre
{
	struct GOTHOGRE_EXPORT StrUtil : public Ogre::StringUtil
	{
		// Makes the specified string upper-case. Strings are treated as 7-bit ASCII.
		static String& toUpperCase(String& _inOutString);
			
		// Makes the specified string low-case. Strings are treated as 7-bit ASCII.
		static String& toLowerCase(String& _inOutString);

		// Compares two strings case-insensivite. Strings are treated as 7-bit ASCII.
		static int compareNoCase(const String& _text1, const String& _text2);
		static int compareNoCase(const String& _text1, const char* _text2);
		static int compareNoCase(const char* _text1, const String& _text2);
		static int compareNoCase(const char* _text1, const char* _text2);

		static bool equalsNoCase(const String& _text1, const String& _text2);
		static bool equalsNoCase(const String& _text1, const char* _text2);
		static bool equalsNoCase(const char* _text1, const String& _text2);
		static bool equalsNoCase(const char* _text1, const char* _text2);
	};

} // namespace GothOgre

#endif // GOTHOGRE_STR_UTIL_H