#include "Precompiled.h"
#include "Utf16.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------
	uint8 Utf16::length(const uint16* _buf, size_t _offset, size_t _bufSize)
	{
		if(_offset >= _bufSize)
			return 0;

		uint16 lead = _buf[_offset];
		if((lead & 0xF800) == 0xD800)
		{
			if(_offset + 1 >= _bufSize)
				return 0;

			return 2;
		}
		return 1;
	}
	//----------------------------------------------------------------------------
	static const uint32 UTF16_SURROGATE_OFFSET = (0xD800 << 10UL) + 0xDC00 - 0x10000;
	//---------------------------------------------------------------------------
	void Utf16::next(const uint16* _buf, size_t& _offset, size_t _bufSize, uint32& _c)
	{
		if(_offset >= _bufSize)
		{
			_c = -1;
			return;
		}

		uint16 lead = _buf[_offset++];
		_c = lead;
		if((_c & 0xF800) == 0xD800)
		{
			if(_offset >= _bufSize)
			{
				--_offset;
				_c = -1;
				return;
			}

			if((_c & 0xFC00) != 0xD800)
			{
				--_offset;
				_c = -1;
				UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, (_bufSize - _offset) * 2);
				return;
			}

			uint16 trail = _buf[_offset++];
			if((trail & 0xFC00) != 0xDC00)
			{
				_offset -= 2;
				_c = -1;
				UnicodeSupport::_throwCouldNotConvert("UTF-32", getName(), _buf + _offset, (_bufSize - _offset) * 2);
				return;
			}

			_c = (_c << 10) + (trail - UTF16_SURROGATE_OFFSET);
        }
	}
	//----------------------------------------------------------------------------
	void Utf16::next_unsafe(const uint16* _buf, size_t& _offset, uint32& _c)
	{
		_c = _buf[_offset++];
		if((_c & 0xF800) == 0xD800)
		{
			uint16 trail = _buf[_offset++];
			_c = (_c << 10) + (trail - UTF16_SURROGATE_OFFSET);
        }
	}
	//----------------------------------------------------------------------------
	void Utf16::append(uint16* _buf, size_t& _offset, size_t _bufSize, uint32 _c)
	{
		if(_c <= 0xFFFF) 
		{
			if(_offset >= _bufSize)
			{
				GOTHOGRE_EXCEPT("Not enough space in codepage buffer.");
				return;
			}

			if((_c & 0xF800) == 0xD800)
			{
				UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-32", &_c, sizeof(_c));
				return;
			}

			_buf[_offset++] = _c;
		} 
		else if(_c <= 0x10FFFF) 
		{
			if(_offset + 1 >= _bufSize)
			{
				GOTHOGRE_EXCEPT("Not enough space in codepage buffer.");
				return;
			}

			_buf[_offset++] = (_c >> 10) + 0xD7C0;
			_buf[_offset++] = (_c & 0x3FF) | 0xDC00;
		}
		else 
			UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-32", &_c, sizeof(_c));
	}
	//----------------------------------------------------------------------------
	void Utf16::append_unsafe(uint16* _buf, size_t& _offset, uint32 _c)
	{
		if(_c <= 0xFFFF) 
		{
			_buf[_offset++] = _c;
		} 
		else
		{
			_buf[_offset++] = (_c >> 10) + 0xD7C0;
			_buf[_offset++] = (_c & 0x3FF) | 0xDC00;
		}
	}
	//----------------------------------------------------------------------------
	const String& Utf16::getName()
	{
		static const String s = "UTF-16";
		return s;
	}
}