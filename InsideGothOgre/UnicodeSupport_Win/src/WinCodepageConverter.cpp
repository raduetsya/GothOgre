#include "Precompiled.h"
#include "WinCodepageConverter.h"
#include "WinUtfUtil.h"
#include "Utf8.h"
#include "Utf16.h"
#include "Utf16Endian.h"
#include "Utf32Endian.h"
#include "UtfCodePageId.h"


namespace GothOgre
{
	WinCodepageConverter::WinCodepageConverter(UINT _codepage)
	{
		mCodepage = _codepage;
	}
	//----------------------------------------------------------------------------
	WinCodepageConverter::~WinCodepageConverter()
	{
	}
	//----------------------------------------------------------------------------
	void WinCodepageConverter::unicodeToBuffer(ByteBuffer& _destBuffer, const String& _srcString)
	{
		_unicodeToBuffer<sizeof(String::value_type)> (_destBuffer, _srcString);
	}
	//----------------------------------------------------------------------------
	static void appendDataToBuffer(ByteBuffer& _destBuffer, const uint8* _srcData, size_t _srcSize)
	{
		size_t oldSize = _destBuffer.size();
		size_t newSize = oldSize + _srcSize;
		_destBuffer.resize(newSize);
		memcpy(&_destBuffer[oldSize], _srcData, _srcSize);
	}
	//----------------------------------------------------------------------------
	template<>
	void WinCodepageConverter::_unicodeToBuffer<1>(ByteBuffer& _destBuffer, ConstStringHolder<1> _srcString)
	{
		// An empty string needs special processing.
		if(_srcString.empty())
			return;

		const uint8* data;
		size_t sz;
		ByteBuffer  buf;

		switch(getCodepage())
		{
			case CP_UTF8:
			{
				// UTF-8 -> UTF-8
				data = _srcString.c_str();
				sz = _srcString.size();
				break;
			}
			case CP_UTF16LE:
			{
				// UTF-8 -> UTF-16LE
				utf8_to_utf16le(buf, _srcString.c_str(), _srcString.size());
				data = buf.data();
				sz = buf.size();
				break;
			}
			case CP_UTF16BE:
			{
				// UTF-8 -> UTF-16BE
				utf8_to_utf16be(buf, _srcString.c_str(), _srcString.size());
				data = buf.data();
				sz = buf.size();
				break;
			}
			case CP_UTF32LE:
			{
				// UTF-8 -> UTF-32LE
				utf8_to_utf32le(buf, _srcString.c_str(), _srcString.size());
				data = buf.data();
				sz = buf.size();
				break;
			}
			case CP_UTF32BE:
			{
				// UTF-8 -> UTF-32BE
				utf8_to_utf32be(buf, _srcString.c_str(), _srcString.size());
				data = buf.data();
				sz = buf.size();
				break;
			}
			default:
			{
				// UTF-8 -> UTF-16
				Utf16Buffer buf16;
				utf8_to_utf16(buf16, _srcString.c_str(), _srcString.size());					
				
				// UTF-16 -> destination code page
				size_t maxsz = (buf16.size() + 10) * 4;
				buf.resize(maxsz);
				
				BOOL usedDefaultChar;
				int r = WideCharToMultiByte(getCodepage(), 0, 
					char_cast<LPWSTR>(buf16.data()), buf16.size(), 
					char_cast<LPSTR>(buf.data()), buf.size(), 0, &usedDefaultChar);
				
				sz = r;
				data = buf.data();

				if(r == 0 || usedDefaultChar)
					UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-16", buf16.data(), buf16.size() * 2);
			}
		}
		appendDataToBuffer(_destBuffer, data, sz);
	}
	//----------------------------------------------------------------------------
	template<>
	void WinCodepageConverter::_unicodeToBuffer<2>(ByteBuffer& _destBuffer, ConstStringHolder<2> _srcString)
	{
		// An empty string needs special processing.
		if(_srcString.empty())
			return;

		const uint8* data;
		size_t sz;
		ByteBuffer  buf;

		switch(getCodepage())
		{
			case CP_UTF8:
			{
				// UTF-16 -> UTF-8
				utf16_to_utf8(buf, _srcString.c_str(), _srcString.size());
				data = buf.data();
				sz = buf.size();
				break;
			}
			case CP_UTF16LE:
			{
				// UTF-16 -> UTF-16LE
				if(MACHINE_LITTLE_ENDIAN)
				{
					data = (const uint8*) _srcString.c_str();
					sz = _srcString.size() * 2;
				}
				else
				{
					utf16_to_utf16le(buf, _srcString.c_str(), _srcString.size());
					data = buf.data();
					sz = buf.size();
				}
				break;
			}
			case CP_UTF16BE:
			{
				// UTF-16 -> UTF-16BE
				if(!MACHINE_LITTLE_ENDIAN)
				{
					data = (const uint8*) _srcString.c_str();
					sz = _srcString.size() * 2;
				}
				else
				{
					utf16_to_utf16be(buf, _srcString.c_str(), _srcString.size());
					data = buf.data();
					sz = buf.size();
				}
				break;
			}
			case CP_UTF32LE:
			{
				// UTF-16 -> UTF-32LE
				utf16_to_utf32le(buf, _srcString.c_str(), _srcString.size());
				data = buf.data();
				sz = buf.size();
				break;
			}
			case CP_UTF32BE:
			{
				// UTF-16 -> UTF-32BE
				utf16_to_utf32be(buf, _srcString.c_str(), _srcString.size());
				data = buf.data();
				sz = buf.size();
				break;
			}
			default:
			{
				// UTF-16 -> destination code page
				size_t maxsz = (_srcString.size() + 10) * 4;
				buf.resize(maxsz);
				
				BOOL usedDefaultChar;
				int r = WideCharToMultiByte(getCodepage(), 0, 
					char_cast<LPCWSTR>(_srcString.c_str()), _srcString.size(), 
					char_cast<LPSTR>(buf.data()), buf.size(), 0, &usedDefaultChar);

				sz = r;
				data = buf.data();

				if(r == 0 || usedDefaultChar)
					UnicodeSupport::_throwCouldNotConvert(getName(), "UTF-16", _srcString.c_str(), _srcString.size() * 2);
			}
		}
		appendDataToBuffer(_destBuffer, data, sz);
	}
	//---------------------------------------------------------------------------
	void WinCodepageConverter::bufferToUnicode(String& _destString, const ByteBuffer& _srcBuffer)
	{
		_bufferToUnicode<sizeof(String::value_type)> (_destString, _srcBuffer);
	}
	//---------------------------------------------------------------------------
	template<>
	void WinCodepageConverter::_bufferToUnicode<1>(StringHolder<1> _destString, const ByteBuffer& _srcBuffer)
	{
		// An empty buffer needs special processing.
		if(_srcBuffer.empty())
			return;

		const uint8* data;
		size_t sz;
		Utf8Buffer  buf8;

		switch(getCodepage())
		{
			case CP_UTF8:
			{
				// UTF-8 -> UTF-8
				data = _srcBuffer.data();
				sz = _srcBuffer.size();
				break;
			}
			case CP_UTF16LE:
			{
				// UTF-16LE -> UTF-8
				utf16le_to_utf8(buf8, _srcBuffer.data(), _srcBuffer.size());
				data = buf8.data();
				sz = buf8.size();
				break;
			}
			case CP_UTF16BE:
			{
				// UTF-16BE -> UTF-8
				utf16be_to_utf8(buf8, _srcBuffer.data(), _srcBuffer.size());
				data = buf8.data();
				sz = buf8.size();
				break;
			}
			case CP_UTF32LE:
			{
				// UTF-32LE -> UTF-8
				utf32le_to_utf8(buf8, _srcBuffer.data(), _srcBuffer.size());
				data = buf8.data();
				sz = buf8.size();
				break;
			}
			case CP_UTF32BE:
			{
				// UTF-32BE -> UTF-8
				utf32be_to_utf8(buf8, _srcBuffer.data(), _srcBuffer.size());
				data = buf8.data();
				sz = buf8.size();
				break;
			}
			default:
			{
				// source code page -> UTF-16
				size_t maxsz = _srcBuffer.size() * 2;
				Utf16Buffer buf16(maxsz);
				
				int r = MultiByteToWideChar(getCodepage(), 0, 
					char_cast<LPCSTR>(_srcBuffer.data()), _srcBuffer.size(), 
					char_cast<LPWSTR>(buf16.data()), buf16.size());
				if(r == 0)
				{
					UnicodeSupport::_throwCouldNotConvert("UTF-16", getName(), _srcBuffer.data(), _srcBuffer.size());
					return;
				}

				// UTF-16 -> UTF-8
				utf16_to_utf8_unsafe(buf8, buf16.data(), r);
				data = buf8.data();
				sz = buf8.size();
			}
		}
		_destString.append(data, sz);
	}
	//---------------------------------------------------------------------------
	template<>
	void WinCodepageConverter::_bufferToUnicode<2>(StringHolder<2> _destString, const ByteBuffer& _srcBuffer)
	{
		// An empty buffer needs special processing.
		if(_srcBuffer.empty())
			return;

		const uint16* data;
		size_t sz;
		Utf16Buffer buf16;

		switch(getCodepage())
		{
			case CP_UTF8:
			{
				// UTF-8 -> UTF-16
				utf8_to_utf16(buf16, _srcBuffer.data(), _srcBuffer.size());
				data = buf16.data();
				sz = buf16.size();
				break;
			}
			case CP_UTF16LE:
			{
				// UTF-16LE -> UTF-16
				utf16le_to_utf16(buf16, _srcBuffer.data(), _srcBuffer.size());
				data = buf16.data();
				sz = buf16.size();
				break;
			}
			case CP_UTF16BE:
			{
				// UTF-16BE -> UTF-16
				utf16be_to_utf16(buf16, _srcBuffer.data(), _srcBuffer.size());
				data = buf16.data();
				sz = buf16.size();
				break;
			}
			case CP_UTF32LE:
			{
				// UTF-32LE -> UTF-16
				utf32le_to_utf16(buf16, _srcBuffer.data(), _srcBuffer.size());
				data = buf16.data();
				sz = buf16.size();
				break;
			}
			case CP_UTF32BE:
			{
				// UTF-32BE -> UTF-16
				utf32be_to_utf16(buf16, _srcBuffer.data(), _srcBuffer.size());
				data = buf16.data();
				sz = buf16.size();
				break;
			}
			default:
			{
				// source code page -> UTF-16
				size_t maxsz = _srcBuffer.size() * 2;
				buf16.resize(maxsz);
				
				int r = MultiByteToWideChar(getCodepage(), 0, 
					(LPCSTR) _srcBuffer.data(),	_srcBuffer.size(), 
					(LPWSTR) buf16.data(), buf16.size());
				if(r == 0)
				{
					UnicodeSupport::_throwCouldNotConvert("UTF-16", getName(), _srcBuffer.data(), _srcBuffer.size());
					return;
				}

				data = buf16.data();
				sz = r;
			}
		}
		_destString.append(data, sz);
	}
	//----------------------------------------------------------------------------
	void WinCodepageConverter::singleCharToUnicode(String& _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset)
	{
		_singleCharToUnicode<sizeof(String::value_type)> (_destString, _srcBuffer, _srcOffset);
	}
	//----------------------------------------------------------------------------
	template<>
	void WinCodepageConverter::_singleCharToUnicode<1>(StringHolder<1> _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset)
	{
		// An empty buffer needs special processing.
		if(_srcOffset == _srcBuffer.size())
			return;

		size_t sz;
		const uint8* data;
		uint8  buf8[MAX_UTF8_BYTES];

		switch(getCodepage())
		{
			case CP_UTF8:
			{
				// UTF-8 -> UTF-8
				data = _srcBuffer.data() + _srcOffset;
				sz = Utf8::length(_srcBuffer.data(), _srcOffset, _srcBuffer.size());
				_srcOffset += sz;
				break;
			}
			case CP_UTF16LE:
			{
				// UTF-16LE -> UTF-8
				uint32 c;
				Utf16LE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
				sz = 0;
				if(c != -1)
				{
					Utf8::append_unsafe(buf8, sz, c);
					data = buf8;
				}
				break;
			}
			case CP_UTF16BE:
			{
				// UTF-16BE -> UTF-8
				uint32 c;
				Utf16BE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
				sz = 0;
				if(c != -1)
				{
					Utf8::append_unsafe(buf8, sz, c);
					data = buf8;
				}
				break;
			}
			case CP_UTF32LE:
			{
				// UTF-32LE -> UTF-8
				uint32 c;
				Utf32LE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
				sz = 0;
				if(c != -1)
				{
					Utf8::append_unsafe(buf8, sz, c);
					data = buf8;
				}
				break;
			}
			case CP_UTF32BE:
			{
				// UTF-32BE -> UTF-8
				uint32 c;
				Utf32BE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
				sz = 0;
				if(c != -1)
				{
					Utf8::append_unsafe(buf8, sz, c);
					data = buf8;
				}
				break;
			}
			default:
			{
				// Calculate length of the source sequence describing one character
				size_t cb = 1;
				char leadByte = _srcBuffer[_srcOffset];
				if(::IsDBCSLeadByteEx(getCodepage(), leadByte))
					++cb;

				sz = 0;
				if(_srcOffset + cb <= _srcBuffer.size())
				{
					// source code page -> UTF-16
					uint16 buf16[MAX_UTF16_WORDS];
					int r = MultiByteToWideChar(getCodepage(), 0, 
						char_cast<LPCSTR>(&_srcBuffer[_srcOffset]), cb,
						char_cast<LPWSTR>(buf16), MAX_UTF16_WORDS);
					
					if(r == 0)
					{
						UnicodeSupport::_throwCouldNotConvert("UTF-16", getName(), &_srcBuffer[_srcOffset], cb);
						return;
					}

					_srcOffset += cb;

					// UTF-16 -> UTF-8
					uint32 c;
					size_t ofs16 = 0;
					Utf16::next_unsafe(buf16, ofs16, c);
					Utf8::append_unsafe(buf8, sz, c);
					data = buf8;
				}
				break;
			}
		}
		_destString.append(data, sz);
	}
	//----------------------------------------------------------------------------
	template<>
	void WinCodepageConverter::_singleCharToUnicode<2>(StringHolder<2> _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset)
	{
		// An empty buffer needs special processing.
		if(_srcOffset == _srcBuffer.size())
			return;

		size_t sz;
		const uint16* data;
		uint16 buf16[MAX_UTF8_BYTES];

		switch(getCodepage())
		{
			case CP_UTF8:
			{
				// UTF-8 -> UTF-16
				uint32 c;
				Utf8::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
				sz = 0;
				if(c != -1)
				{
					Utf16::append_unsafe(buf16, sz, c);
					data = buf16;
				}
				break;
			}
			case CP_UTF16LE:
			{
				// UTF-16LE -> UTF-16
				if(MACHINE_LITTLE_ENDIAN)
				{
					data = (const uint16*) (_srcBuffer.data() + _srcOffset);
					sz = Utf16LE::length(_srcBuffer.data(), _srcOffset, _srcBuffer.size());
					_srcOffset += sz;
				}
				else
				{
					uint32 c;
					Utf16LE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
					sz = 0;
					if(c != -1)
					{
						Utf16::append_unsafe(buf16, sz, c);
						data = buf16;
					}
				}
				break;
			}
			case CP_UTF16BE:
			{
				// UTF-16BE -> UTF-8
				if(!MACHINE_LITTLE_ENDIAN)
				{
					data = (const uint16*) (_srcBuffer.data() + _srcOffset);
					sz = Utf16BE::length(_srcBuffer.data(), _srcOffset, _srcBuffer.size());
					_srcOffset += sz;
				}
				else
				{
					uint32 c;
					Utf16BE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
					sz = 0;
					if(c != -1)
					{
						Utf16::append_unsafe(buf16, sz, c);
						data = buf16;
					}
				}
				break;
			}
			case CP_UTF32LE:
			{
				// UTF-32LE -> UTF-8
				uint32 c;
				Utf32LE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
				sz = 0;
				if(c != -1)
				{
					Utf16::append_unsafe(buf16, sz, c);
					data = buf16;
				}
				break;
			}
			case CP_UTF32BE:
			{
				// UTF-32BE -> UTF-8
				uint32 c;
				Utf32BE::next(_srcBuffer.data(), _srcOffset, _srcBuffer.size(), c);
				sz = 0;
				if(c != -1)
				{
					Utf16::append_unsafe(buf16, sz, c);
					data = buf16;
				}
				break;
			}
			default:
			{
				// Calculate length of the source sequence describing one character
				size_t cb = 1;
				char leadByte = _srcBuffer[_srcOffset];
				if(::IsDBCSLeadByteEx(getCodepage(), leadByte))
					++cb;

				sz = 0;
				if(_srcOffset + cb <= _srcBuffer.size())
				{
					// source code page -> UTF-16
					int r = MultiByteToWideChar(getCodepage(), 0, 
						char_cast<LPCSTR>(&_srcBuffer[_srcOffset]), cb,
						char_cast<LPWSTR>(buf16), MAX_UTF16_WORDS);

					if(r == 0)
					{
						UnicodeSupport::_throwCouldNotConvert("UTF-16", getName(), &_srcBuffer[_srcOffset], cb);
						return;
					}

					_srcOffset += cb;
					data = buf16;
					sz = r;
				}
				break;
			}
		}
		_destString.append(data, sz);
	}

}