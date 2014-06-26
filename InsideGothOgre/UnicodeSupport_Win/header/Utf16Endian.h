#ifndef UTF16_ENDIAN_H
#define UTF16_ENDIAN_H

#include "MachineEndian.h"


namespace GothOgre
{
	union ByteOrderExchanger16
	{
		struct
		{
			uint8 byte0:8;
			uint8 byte1:8;
		};
		uint16 w;
	};

	template<bool REVERSED_BYTE_ORDER>
	inline void orderBytes(uint16& _data)
	{
	}

	template<>
	inline void orderBytes<true>(uint16& _data)
	{
		ByteOrderExchanger16& xch = reinterpret_cast<ByteOrderExchanger16&>(_data);
		uint8 tmp = xch.byte0;
		xch.byte0 = xch.byte1;
		xch.byte1 = tmp;
	}
	//----------------------------------------------------------------------------
	template<bool REVERSED_BYTE_ORDER>
	struct Utf16Base
	{
		// Returns "UTF-16LE" or "UTF-16BE"
		static const String& getName()
		{
			if(MACHINE_LITTLE_ENDIAN != REVERSED_BYTE_ORDER)
			{
				static const String s = "UTF-16LE";
				return s;
			}
			else
			{
				static const String s = "UTF-16BE";
				return s;
			}
		}

		// How many words are used for one code point?
		// The function returns 1 to 2.
		static uint8 length(const uint8* _buf, size_t _offset, size_t _bufSize)
		{
			if(_offset + 2 > _bufSize)
				return 0;

			uint16 lead = *(const uint16*) (_buf + _offset);
			orderBytes<REVERSED_BYTE_ORDER>(lead);
			if((lead & 0xF800) == 0xD800)
			{
				if(_offset + 4 > _bufSize)
					return 0;

				return 4;
			}
			return 2;
		}

		static const uint32 UTF16_SURROGATE_OFFSET = (0xD800 << 10UL) + 0xDC00 - 0x10000;

		// Get a code point from a string at a code point boundary offset,
		// and advance the offset to the next code point boundary.
		// (Post-incrementing forward iteration.)
		static void next(const uint8* _buf, size_t& _offset, size_t _bufSize, uint32& _c)
		{
			if(_offset + 2 > _bufSize)
			{
				_c = -1;
				return;
			}

			uint16 lead = *(const uint16*) (_buf + _offset);
			orderBytes<REVERSED_BYTE_ORDER>(lead);
			_c = lead;
			if((_c & 0xF800) == 0xD800)
			{
				if(_offset + 4 > _bufSize)
				{
					_c = -1;
					return;
				}

				if((_c & 0xFC00) != 0xD800)
				{
					_c = -1;
					UnicodeSupport::_throwCouldNotConvert("UTF32", getName(), _buf + _offset, _bufSize - _offset);
					return;
				}

				_offset += 2;
				uint16 trail = *(const uint16*) (_buf + _offset);
				orderBytes<REVERSED_BYTE_ORDER>(lead);

				if((trail & 0xFC00) != 0xDC00)
				{
					_c = -1;
					_offset -= 2;
					UnicodeSupport::_throwCouldNotConvert("UTF32", getName(), _buf + _offset, _bufSize - _offset);
					return;
				}

				_c = (_c << 10) + (trail - UTF16_SURROGATE_OFFSET);
			}
			_offset += 2;
		}

		// Get a code point from a string at a code point boundary offset,
		// and advance the offset to the next code point boundary.
		// (Post-incrementing forward iteration.)
		// This is a fast version without any additional checks.
		static void next_unsafe(const uint8* _buf, size_t& _offset, uint32& _c)
		{
			uint16 lead = *(const uint16*) (_buf + _offset);
			orderBytes<REVERSED_BYTE_ORDER>(lead);
			_c = lead;
			if((_c & 0xF800) == 0xD800)
			{
				_offset += 2;
				uint16 trail = *(const uint16*) (_buf + _offset);
				orderBytes<REVERSED_BYTE_ORDER>(trail);
				_c = (_c << 10) + (trail - UTF16_SURROGATE_OFFSET);
			}
			_offset += 2;
		}

		// Append a code point to a string, overwriting 1 to 2 words.
		// The offset points to the current end of the string contents
		// and is advanced (post-increment).
		static void append(uint8* _buf, size_t& _offset, size_t _bufSize, uint32 _c)
		{
			if(_c <= 0xFFFF) 
			{
				if(_offset + 2 > _bufSize)
				{
					Error::notEnoughSpace();
					return;
				}

				if((_c & 0xF800) == 0xD800)
				{
					UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-32", _buf + _offset, _bufSize - _offset);
					return;
				}

				uint16 lead = (uint16) _c;
				orderBytes<REVERSED_BYTE_ORDER>(lead);
				*(uint16*) (_buf + _offset) = lead;
				_offset += 2;
			} 
			else if(_c <= 0x10FFFF) 
			{
				if(_offset + 4 > _bufSize)
				{
					Error::notEnoughSpace();
					return;
				}
				uint16 lead = (uint16) ((_c >> 10) + 0xD7C0);
				uint16 trail = (uint16) ((_c & 0x3FF) | 0xDC00);
				orderBytes<REVERSED_BYTE_ORDER>(lead);
				orderBytes<REVERSED_BYTE_ORDER>(trail);
				*(uint16*) (_buf + _offset) = lead;
				_offset += 2;
				*(uint16*) (_buf + _offset) = trail;
				_offset += 2;
			}
			else 
			{
				UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-32", _buf + _offset, _bufSize - _offset);
				return;
			}
		}

		// Append a code point to a string, overwriting 1 to 2 words.
		// The offset points to the current end of the string contents
		// and is advanced (post-increment).
		// This is a fast version without any additional checks.
		static void append_unsafe(uint8* _buf, size_t& _offset, uint32 _c)
		{
			if(_c <= 0xFFFF) 
			{
				uint16 lead = (uint16) _c;
				orderBytes<REVERSED_BYTE_ORDER>(lead);
				*(uint16*) (_buf + _offset) = lead;
				_offset += 2;
			} 
			else
			{
				uint16 lead = (uint16) ((_c >> 10) + 0xD7C0);
				uint16 trail = (uint16) ((_c & 0x3FF) | 0xDC00);
				orderBytes<REVERSED_BYTE_ORDER>(lead);
				orderBytes<REVERSED_BYTE_ORDER>(trail);
				*(uint16*) (_buf + _offset) = lead;
				_offset += 2;
				*(uint16*) (_buf + _offset) = trail;
				_offset += 2;
			}
		}
	};

	typedef Utf16Base<!MACHINE_LITTLE_ENDIAN> Utf16LE;
	typedef Utf16Base<MACHINE_LITTLE_ENDIAN>  Utf16BE;

}

#endif // UTF16_ENDIAN_H