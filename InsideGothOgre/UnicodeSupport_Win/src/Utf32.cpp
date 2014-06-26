#include "Precompiled.h"
#include "Utf32.h"

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
	//---------------------------------------------------------------------------
	void Utf32Base<true>::reverseByteOrder(uint32& _data)
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
	void Utf32Base<false>::reverseByteOrder(uint32& _data)
	{
	}
	//---------------------------------------------------------------------------
	uint8 Utf32<T>::length(const uint32* _buf, size_t _offset, size_t _bufSize)
	{
		return 1;
	}
	//---------------------------------------------------------------------------
	void Utf32<RBO>::next(const uint8* _buf, size_t& _offset, size_t _bufSize, uint32& _c)
	{
		if(_offset + 4 > _bufSize)
		{
			_c = 0;
			return;
		}
		
		_c = *(const uint32*)(_buf + _offset);
		reverseByteOrder(_c);
		if(_c > 0x10FFFF)
			GOTHOGRE_EXCEPT("Illegal UTF-32 code point.");

		_offset += 4;
	}
	//---------------------------------------------------------------------------
	void Utf32Base<RBO>::next_unsafe(const uint8* _buf, size_t& _offset, uint32& _c)
	{
		_c = *(const uint32*)(_buf + _offset);
		reverseByteOrder(_c);
		_offset += 4;
	}
	//---------------------------------------------------------------------------
	void Utf32Base<RBO>::append(uint8* _buf, size_t& _offset, size_t _bufSize, uint32 _c)
	{
		if(_offset + 4 > _bufSize)
			GOTHOGRE_EXCEPT("Not enough space in buffer.");

		if(_c > 0x10FFFF)
			GOTHOGRE_EXCEPT("Illegal UTF-32 code point.");
		
		reverseByteOrder(_c);
		*(const uint32*)(_buf + _offset) = _c;
		_offset += 4;
	}
	//---------------------------------------------------------------------------
	void Utf32<false>::append_unsafe(uint32* _buf, size_t& _offset, uint32 _c)
	{
		reverseByteOrder(_c);
		*(const uint32*)(_buf + _offset) = _c;
		_offset += 4;
	}
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	uint8 Utf32::NonPlatformByteOrder::length(const uint32* _buf, size_t _offset, size_t _bufSize)
	{
		return 1;
	}
	//---------------------------------------------------------------------------
	void Utf32::NonPlatformByteOrder::next(const uint32* _buf, size_t& _offset, size_t _bufSize, uint32& _c)
	{
		if(_offset >= _bufSize)
		{
			_c = 0;
			return;
		}
		_c = _buf[_offset++];
		Utf32::reverseByteOrder(_c);
		if(_c > 0x10FFFF)
			GOTHOGRE_EXCEPT("Illegal UTF-32 code point.");
	}

}
