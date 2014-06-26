#ifndef UNICODE_ICU_UTF_UTIL_H
#define UNICODE_ICU_UTF_UTIL_H

#include "UtfBuffers.h"

namespace GothOgre
{
	// UTF-8 -> UTF-16
	void utf8_to_utf16(Utf16Buffer& _buf16, const uint8_t* _buf8, int32_t _sz8);
	void utf8_to_utf16_unsafe(Utf16Buffer& _buf16, const uint8_t* _buf8, int32_t _sz8);

	// UTF-16 -> UTF-8
	void utf16_to_utf8(Utf8Buffer& _buf8, const uint16_t* _buf16, int32_t _sz16);
	void utf16_to_utf8_unsafe(Utf8Buffer& _buf8, const uint16_t* _buf16, int32_t _sz16);
}

#endif // UNICODE_ICU_UTF_UTIL_H