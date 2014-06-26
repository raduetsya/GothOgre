#ifndef WIN_UNICODE_SUPPORT_H
#define WIN_UNICODE_SUPPORT_H

#include "StringHolder.h"


namespace GothOgre
{
	//
	class WinUnicodeSupport : public UnicodeSupport
	{
	public:
		WinUnicodeSupport();
		virtual ~WinUnicodeSupport();
		
		const String& getName() const;

	public:
		void toUpperCase(String& _str);
		void toLowerCase(String& _str);
		int compareNoCase(const String& _first, const String& _second);

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

	protected:
		CodepageConverter* openConverterImpl(const String& _encoding);
		void buildListEncodings();

	private:
		static BOOL CALLBACK codePageEnumProc(LPSTR _lpCodePageString);
		void addEncoding2(const String& _encoding, UINT _codePage);
		String normalizeName(const String& _name);

	private:
		typedef map<String, UINT>::type                CodePageByEncoding;
		CodePageByEncoding    mCodepageByEncoding;
	};


} // namespace GothOgre

#endif // WIN_UNICODE_SUPPORT_H