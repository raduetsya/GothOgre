#include "Precompiled.h"
#include "Utf8.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	// In Unicode, all UTF-8 byte sequences with more than 4 bytes are illegal;
	// lead bytes above 0xf4 are illegal.
	// We keep them in this table for skipping long ISO 10646-UTF-8 sequences.
	static const uint8 sTableCountTrailBytes[256] = 
	{
		// 00-7F: Single-byte encoding (compatible with US-ASCII)
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		// 80-BF: Second, third, or fourth byte of a multi-byte sequence
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

		// C0-DF: Start of 2-byte sequence
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

		// E0-EF: Start of 3-byte sequence
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

		// F0-F4: Start of 4-byte sequence
		3, 3, 3, 3, 3,

		// F5-F7: Restricted by RFC 3629: start of 4-byte sequence for codepoint above 10FFFF
		3, 3, 3,

		// F8-FB: Restricted by RFC 3629: start of 5-byte sequence
		4, 4, 4, 4,

		// FC-FD: Restricted by RFC 3629: start of 6-byte sequence
		5, 5,

		// FE-FF: Invalid: not defined by original UTF-8 specification
		-1, -1
	};
	//------------------------------------------------------------------------------
	// Count the trail bytes for a UTF-8 lead byte.
	static uint8 countTrailBytes(uint8 _leadByte)
	{
		return sTableCountTrailBytes[ _leadByte ];
	}
	//------------------------------------------------------------------------------
	// Mask a UTF-8 lead byte, leave only the lower bits 
	// that form part of the code point value.
	static void maskLeadByte(uint8& _leadByte, uint8 _countTrailBytes) 
	{
		if(_countTrailBytes != 0)
			_leadByte &= (1 << (6 - _countTrailBytes)) - 1;
	}
	//---------------------------------------------------------------------------
	uint8 Utf8::length(const uint8* _buf, size_t _offset, size_t _bufSize)
	{
		if(_offset >= _bufSize)
			return 0;

		uint8 leadByte = _buf[_offset];
		size_t len = 1 + countTrailBytes(leadByte);
		if(_offset + len > _bufSize)
			return 0;

		return len;
	}
	//---------------------------------------------------------------------------
	void Utf8::next(const uint8* _buf, size_t& _offset, size_t _bufSize, uint32& _c)
	{
		if(_offset >= _bufSize)
		{
			_c = -1;
			return;
		}

		size_t oldOffset = _offset;
		uint8 c0 = _buf[_offset++];

		size_t count = countTrailBytes(c0);
		if(_offset + count > _bufSize)
		{
			_offset = oldOffset;
			_c = -1;
			return;
		}

		maskLeadByte(c0, count);
		_c = c0;

		switch(count) 
		{
			/* each branch falls through to the next one */
			case 5:
			case 4:
			{
				/* count>=4 is always illegal: no more than 3 trail bytes in Unicode's UTF-8 */
				_offset = oldOffset;
				_c = -1;
				UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, _bufSize - _offset);
				return;
			}
			case 3:
			{
				uint8 trail = _buf[_offset++];
				if((trail & 0xC0) != 0x80)
				{
					_offset = oldOffset;
					_c = -1;
					UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, _bufSize - _offset);
					return;
				}

				_c = (_c << 6) | (trail & 0x3F);
				if(_c >= 0x110) 
				{
					/* code point>0x10ffff, outside Unicode */
					_offset = oldOffset;
					_c = -1;
					UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, _bufSize - _offset);
					return;
				}
			}
			case 2:
			{
				uint8 trail = _buf[_offset++];
				if((trail & 0xC0) != 0x80)
				{
					_offset = oldOffset;
					_c = -1;
					UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, _bufSize - _offset);
					return;
				}

				_c = (_c << 6) | (trail & 0x3F);
			}
			case 1:
			{
				uint8 trail = _buf[_offset++];
				if((trail & 0xC0) != 0x80)
				{
					_offset = oldOffset;
					_c = -1;
					UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, _bufSize - _offset);
					return;
				}

				_c = (_c << 6) | (trail & 0x3F);
			}
			case 0:
			{
				break;
			}
			case -1:
			{
				_offset = oldOffset;
				_c = -1;
				UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, _bufSize - _offset);
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	void Utf8::next_unsafe(const uint8* _buf, size_t& _offset, uint32& _c)
	{
		uint8 c0 = _buf[_offset++];
		
		size_t count = countTrailBytes(c0);

		maskLeadByte(c0, count);
		_c = c0;

		switch(count) 
		{
			/* each branch falls through to the next one */
			case 3:
			{
				uint8 trail = _buf[_offset++];
				_c = (_c << 6) | (trail & 0x3F);
			}
			case 2:
			{
				uint8 trail = _buf[_offset++];
				_c = (_c << 6) | (trail & 0x3F);
			}
			case 1:
			{
				uint8 trail = _buf[_offset++];
				_c = (_c << 6) | (trail & 0x3F);
			}
			case 0:
			{
				break;
			}
		}
	}
	//---------------------------------------------------------------------------
	void Utf8::append(uint8* _buf, size_t& _offset, size_t _bufSize, uint32 _c)
	{
		if(_c <= 0x7F) 
		{
			if(_offset >= _bufSize)
			{
				GOTHOGRE_EXCEPT("Not enough space in codepage buffer.");
				return;
			}
			_buf[_offset++] = (uint8) _c;
		} 
		else if(_c <= 0x7FF) 
		{
			if(_offset + 1 >= _bufSize)
			{
				GOTHOGRE_EXCEPT("Not enough space in codepage buffer.");
				return;
			}
			_buf[_offset++] = (uint8) ((_c >> 6) | 0xc0);
			_buf[_offset++] = (uint8) ((_c & 0x3f) | 0x80);
		} 
		else if(_c <= 0xFFFF)
		{
			if(0xD800 <= _c && _c <= 0xDFFF)
			{
				UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-32", &_c, sizeof(_c));
				return;
			}
			if(_offset + 2 >= _bufSize)
			{
				GOTHOGRE_EXCEPT("Not enough space in codepage buffer.");
				return;
			}
			_buf[_offset++] = (uint8) ((_c >> 12) | 0xE0);
			_buf[_offset++] = (uint8) (((_c >> 6) & 0x3f) | 0x80);
			_buf[_offset++] = (uint8) ((_c & 0x3f) | 0x80);
		} 
		else if(_c <= 0x10FFFF)
		{
  			if(_offset + 3 >= _bufSize)
			{
				GOTHOGRE_EXCEPT("Not enough space in codepage buffer.");
				return;
			}
			_buf[_offset++] = (uint8) ((_c >> 18) | 0xf0);
            _buf[_offset++] = (uint8) (((_c >> 12) & 0x3f) | 0x80);
            _buf[_offset++] = (uint8) (((_c >> 6) & 0x3f) | 0x80);
            _buf[_offset++] = (uint8) ((_c & 0x3f) | 0x80);
		}
		else
		{
			UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-32", &_c, sizeof(_c));
		}
	}
	//---------------------------------------------------------------------------
	void Utf8::append_unsafe(uint8* _buf, size_t& _offset, uint32 _c)
	{
		if(_c <= 0x7F) 
		{
			_buf[_offset++] = (uint8) _c;
		} 
		else if(_c <= 0x7FF) 
		{
			_buf[_offset++] = (uint8) ((_c >> 6) | 0xc0);
			_buf[_offset++] = (uint8) ((_c & 0x3f) | 0x80);
		} 
		else if(_c <= 0xFFFF)
		{
			_buf[_offset++] = (uint8) ((_c >> 12) | 0xE0);
			_buf[_offset++] = (uint8) (((_c >> 6) & 0x3f) | 0x80);
			_buf[_offset++] = (uint8) ((_c & 0x3f) | 0x80);
		} 
		else
		{
			_buf[_offset++] = (uint8) ((_c >> 18) | 0xf0);
            _buf[_offset++] = (uint8) (((_c >> 12) & 0x3f) | 0x80);
            _buf[_offset++] = (uint8) (((_c >> 6) & 0x3f) | 0x80);
            _buf[_offset++] = (uint8) ((_c & 0x3f) | 0x80);
		}
	}
	//---------------------------------------------------------------------------
	const String& Utf8::getName()
	{
		static const String s = "UTF-8";
		return s;
	}
}