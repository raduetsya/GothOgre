#ifndef UTF8_H
#define UTF8_H

namespace GothOgre
{
	struct Utf8
	{
		// Always returns "UTF-8"
		static const String& getName();

		// How many bytes are used for one code point?
		// The function returns 1 to 4.
		static uint8 length(const uint8* _buf, size_t _offset, size_t _bufSize);

		// Get a code point from a string at a code point boundary offset,
		// and advance the offset to the next code point boundary.
		// (Post-incrementing forward iteration.)
		static void next(const uint8* _buf, size_t& _offset, size_t _bufSize, uint32& _c);

		// Get a code point from a string at a code point boundary offset,
		// and advance the offset to the next code point boundary.
		// (Post-incrementing forward iteration.)
		// This is a fast version without any additional checks.
		static void next_unsafe(const uint8* _buf, size_t& _offset, uint32& _c);

		// Append a code point to a string, overwriting 1 to 4 bytes.
		// The offset points to the current end of the string contents
		// and is advanced (post-increment).
		static void append(uint8* _buf, size_t& _offset, size_t _bufSize, uint32 _c);

		// Append a code point to a string, overwriting 1 to 4 bytes.
		// The offset points to the current end of the string contents
		// and is advanced (post-increment).
		// This is a fast version without any additional checks.
		static void append_unsafe(uint8* _buf, size_t& _offset, uint32 _c);
	};

}

#endif // UTF8_H