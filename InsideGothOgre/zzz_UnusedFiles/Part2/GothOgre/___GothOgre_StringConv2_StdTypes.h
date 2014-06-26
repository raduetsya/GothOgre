#ifndef GOTHOGRE_STRING_CONV2_STD_TYPES_H
#define GOTHOGRE_STRING_CONV2_STD_TYPES_H


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// char
	template<>
	struct StringConvOutput<char>
	{
		static String toString(const char& _val)
		{
			return StringConvOutputP<char>::toString(_val, 10);
		}
	};

	template<>
	struct GOTHOGRE_EXPORT StringConvOutputP<char>
	{
		static String toString(const char& _val, size_t _radix);
	};

	template<>
	struct StringConvInput
	{
		static char fromString(const String& _str, size_t _start, size_t& _readcount)
		{
			return StringConvInputP<char>::fromString(_str, _start, _readcount, 10);
		}
	};

	template<>
	struct GOTHOGRE_EXPORT StringConvInputP
	{
		static char fromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix);
	};

} // namespace GothOgre

#endif // GOTHOGRE_STRING_CONV2_STD_TYPES_H