#ifndef UTF_CODEPAGE_ID_H
#define UTF_CODEPAGE_ID_H

namespace GothOgre
{
	// UTF Code pages (CP_UTF8 = 65001 is already defined in Windows SDK)
	enum CodePages
	{
		CP_UTF16LE           = 1200,
		CP_UTF16BE           = 1201,
		CP_UTF32LE           = 12000,
		CP_UTF32BE           = 12001,
	};

}

#endif // UTF_CODEPAGE_ID_H