#include "GothOgre_Precompiled.h"
#include "GothOgre_StrStream.h"


namespace GothOgre
{
#if GOTHOGRE_USE_OWN_STRING_STREAM == 1

	//-----------------------------------------------------------------------------------------------
	// StrStreamBuffer
	//-----------------------------------------------------------------------------------------------
	void StrStreamBuffer::str(const String& _str, bool _useCopy)
	{
		if(_useCopy)
		{
			mString = _str;
			mStringPtr = &mString;
		}
		else
		{
			mStringPtr = const_cast<String*>(&_str);
		}

		if(mStringPtr->empty())
		{
			setp(nullptr, nullptr, nullptr);
			setg(nullptr, nullptr, nullptr);
		}
		else
		{
			char_type* beg = &(*mStringPtr)[0];
			char_type* end = beg + mStringPtr->size();
			setp(beg, beg, end);
			setg(beg, beg, end);
		}
	}
	//-----------------------------------------------------------------------------------------------
	StrStreamBuffer::int_type StrStreamBuffer::overflow(int_type _c)
	{
		if(_c != traits_type::eof())
		{
			if(pptr() == epptr())
			{
				mStringPtr->push_back(_c);
				char_type* beg = &(*mStringPtr)[0];
				char_type* end = beg + mStringPtr->size();
				size_t gpos = gptr() - eback();
				setp(beg, end, end);
				setg(beg, beg + gpos, end);
			}
			else
			{
				*pptr() = _c;
				pbump(1);
			}
		}
		return 0;
	}
	//-----------------------------------------------------------------------------------------------
	StrStreamBuffer::int_type StrStreamBuffer::pbackfail( int_type _c )
	{
		if(gptr() == eback())
			return traits_type::eof(); // failure

		gbump(-1);
		if(_c != traits_type::eof())
		{
			*gptr() = _c;
			return _c;
		}
		else
			return 0;
	}
	//-----------------------------------------------------------------------------------------------
	StrStreamBuffer::int_type StrStreamBuffer::underflow()
	{
		if(gptr() == egptr())
		{
			return traits_type::eof();
		}
		else
		{
			int_type c = *gptr();
			gbump(1);
			return c;
		}
	}
	//-----------------------------------------------------------------------------------------------
	StrStreamBuffer::pos_type StrStreamBuffer::seekoff(off_type _off, 
		std::ios_base::seekdir _way, std::ios_base::openmode _which)
	{
		char* new_read_pos = nullptr;
		char* new_write_pos = nullptr;

		if(_which & std::ios_base::in)
		{
			switch(_way)
			{
				case std::ios_base::beg: new_read_pos = eback() + _off; break;
				case std::ios_base::cur: new_read_pos = gptr()  + _off; break;
				case std::ios_base::end: new_read_pos = egptr() + _off; break;
				assert(false);
			}
			if(new_read_pos < eback() || new_read_pos > egptr())
				return -1;
			gbump(new_read_pos - gptr());
		}

		if(_which & std::ios_base::out)
		{
			switch(_way)
			{
				case std::ios_base::beg: new_write_pos = pbase() + _off; break;
				case std::ios_base::cur: new_write_pos = pptr()  + _off; break;
				case std::ios_base::end: new_write_pos = epptr() + _off; break;
				assert(false);
			}
			if(new_write_pos < pbase() || new_write_pos > epptr())
				return -1;
			pbump(new_write_pos - pptr());
		}

		if(new_read_pos)
			return gptr() - eback();
		else
			return pptr() - pbase();
	}
	//-----------------------------------------------------------------------------------------------
	StrStreamBuffer::pos_type StrStreamBuffer::seekpos(pos_type _ptr, 
		std::ios_base::openmode _which)
	{
		return seekoff(_ptr, std::ios_base::beg, _which);
	}

#endif // GOTHOGRE_USE_OWN_STRING_STREAM

} // namespace 
