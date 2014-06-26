#ifndef UTF32_ENDIAN_H
#define UTF32_ENDIAN_H

#include "MachineEndian.h"


namespace GothOgre
{
	union ByteOrderExchanger32
	{
		struct
		{
			uint8 byte0:8;
			uint8 byte1:8;
			uint8 byte2:8;
			uint8 byte3:8;
		};
		uint32 dw;
	};

	template<bool REVERSED_BYTE_ORDER>
	inline void orderBytes(uint32& _data)
	{
	}

	template<>
	inline void orderBytes<true>(uint32& _data)
	{
		ByteOrderExchanger32& xch = reinterpret_cast<ByteOrderExchanger32&>(_data);
		uint8 tmp = xch.byte0;
		xch.byte0 = xch.byte3;
		xch.byte3 = tmp;
		tmp = xch.byte1;
		xch.byte1 = xch.byte2;
		xch.byte2 = tmp;
	}
	//---------------------------------------------------------------------------
	template<bool REVERSED_BYTE_ORDER>
	struct Utf32Base
	{
		// Returns "UTF-32LE" or "UTF-32BE"
		static const String& getName()
		{
			if(MACHINE_LITTLE_ENDIAN != REVERSED_BYTE_ORDER)
			{
				static const String s = "UTF-32LE";
				return s;
			}
			else
			{
				static const String s = "UTF-32BE";
				return s;
			}
		}

		// How many double words are used for one code point?
		// This function always returns 1.
		static uint8 length(const uint8* _buf, size_t _offset, size_t _bufSize)
		{
			return 4;
		}

		// Get a code point from a string at a code point boundary offset,
		// and advance the offset to the next code point boundary.
		// (Post-incrementing forward iteration.)
		static void next(const uint8* _buf, size_t& _offset, size_t _bufSize, uint32& _c)
		{
			if(_offset + 4 > _bufSize)
			{
				_c = -1;
				return;
			}
			_c = *(const uint32*)(_buf + _offset);
			orderBytes<REVERSED_BYTE_ORDER>(_c);
			if(_c > 0x10FFFF)
			{
				_c = -1;
				UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, _bufSize - _offset);
				return;
			}
			_offset += 4;
		}

		// Get a code point from a string at a code point boundary offset,
		// and advance the offset to the next code point boundary.
		// (Post-incrementing forward iteration.)
		// This is a fast version without any additional checks.
		static void next_unsafe(const uint8* _buf, size_t& _offset, uint32& _c)
		{
			_c = *(const uint32*)(_buf + _offset);
			orderBytes<REVERSED_BYTE_ORDER>(_c);
			_offset += 4;
		}

		// Append a code point to a string, overwriting 1 to 2 words.
		// The offset points to the current end of the string contents
		// and is advanced (post-increment).
		static void append(uint8* _buf, size_t& _offset, size_t _bufSize, uint32 _c)
		{
			if(_offset + 4 > _bufSize)
			{
				Error::notEnoughSpace();
				return;
			}

			if(_c > 0x10FFFF)
			{
				Error::couldNotConvert(getName(), "UTF-32", _buf + _offset, _bufSize - _offset);
				return;
			}
			
			orderBytes<REVERSED_BYTE_ORDER>(_c);
			*(uint32*)(_buf + _offset) = _c;
			_offset += 4;
		}

		// Append a code point to a string, overwriting 1 to 2 words.
		// The offset points to the current end of the string contents
		// and is advanced (post-increment).
		// This is a fast version without any additional checks.
		static void append_unsafe(uint8* _buf, size_t& _offset, uint32 _c)
		{
			orderBytes<REVERSED_BYTE_ORDER>(_c);
			*(uint32*)(_buf + _offset) = _c;
			_offset += 4;
		}
	};

	typedef Utf32Base<!MACHINE_LITTLE_ENDIAN> Utf32LE;
	typedef Utf32Base<MACHINE_LITTLE_ENDIAN>  Utf32BE;
}

#endif // UTF32_ENDIAN_H