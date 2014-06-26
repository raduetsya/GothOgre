#include "GothOgre_Precompiled.h"
#include "GothOgre_StringConv.h"

namespace GothOgre
{
	//----------------------------------------------------------------------------
	// char
	char StringConv::charFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		long l = longFromString(_str, _start, _readcount, _radix);
		if(l > std::numeric_limits<char>::max())
		{
			l = std::numeric_limits<char>::max();
		}
		else if(l < std::numeric_limits<char>::min())
		{
			l = std::numeric_limits<char>::min();
		}
		return (char) l;
	}

	String StringConv::charToString(char _val, size_t _radix) 
	{
		return longToString(_val, _radix);
	}

	//----------------------------------------------------------------------------
	// uchar
	uchar StringConv::ucharFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		ulong l = ulongFromString(_str, _start, _readcount, _radix);
		if(l > std::numeric_limits<uchar>::max())
		{
			l = std::numeric_limits<uchar>::max();
		}
		return (uchar) l;
	}

	String StringConv::ucharToString(uchar _val, size_t _radix) 
	{
		return ulongToString(_val, _radix);
	}

	//----------------------------------------------------------------------------
	// short
	short StringConv::shortFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		long l = longFromString(_str, _start, _readcount, _radix);
		if(l > std::numeric_limits<short>::max())
		{
			l = std::numeric_limits<short>::max();
		}
		else if(l < std::numeric_limits<short>::min())
		{
			l = std::numeric_limits<short>::min();
		}
		return (short) l;
	}

	String StringConv::shortToString(short _val, size_t _radix) 
	{
		return longToString(_val, _radix);
	}

	//----------------------------------------------------------------------------
	// ushort
	ushort StringConv::ushortFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		ulong l = ulongFromString(_str, _start, _readcount, _radix);
		if(l > std::numeric_limits<ushort>::max())
		{
			l = std::numeric_limits<ushort>::max();
		}
		return (ushort) l;
	}

	String StringConv::ushortToString(ushort _val, size_t _radix) 
	{
		return ulongToString(_val, _radix);
	}

	//----------------------------------------------------------------------------
	// int
	int StringConv::intFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		long l = longFromString(_str, _start, _readcount, _radix);
		if(l > std::numeric_limits<int>::max())
		{
			l = std::numeric_limits<int>::max();
		}
		else if(l < std::numeric_limits<int>::min())
		{
			l = std::numeric_limits<int>::min();
		}
		return (int) l;
	}

	String StringConv::intToString(int _val, size_t _radix) 
	{
		return longToString(_val, _radix);
	}

	//----------------------------------------------------------------------------
	// uint
	uint StringConv::uintFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		ulong l = ulongFromString(_str, _start, _readcount, _radix);
		if(l > std::numeric_limits<uint>::max())
		{
			l = std::numeric_limits<uint>::max();
		}
		return (uint) l;
	}

	String StringConv::uintToString(uint _val, size_t _radix) 
	{
		return ulongToString(_val, _radix);
	}

	//----------------------------------------------------------------------------
	// long
	long StringConv::longFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		char* endptr;
		const char* startptr = &_str[_start];
		long ret = strtol(startptr, &endptr, _radix);
		_readcount = endptr - startptr;
		return ret;
	}

	String StringConv::longToString(long _val, size_t _radix) 
	{
		char buf[12];
		_ltoa( _val, buf, _radix );
		return buf;
	}

	//----------------------------------------------------------------------------
	// ulong
	ulong StringConv::ulongFromString(const String& _str, size_t _start, size_t& _readcount, size_t _radix)
	{
		char* endptr;
		const char* startptr = &_str[_start];
		ulong ret = strtoul(startptr, &endptr, _radix);
		_readcount = endptr - startptr;
		return ret;
	}

	String StringConv::ulongToString(ulong _val, size_t _radix) 
	{
		char buf[12];
		_ultoa( _val, buf, _radix );
		return buf;
	}

	//----------------------------------------------------------------------------
	// bool
	bool StringConv::boolFromString(const String& _str, size_t _start, size_t& _readcount)
	{
		const char* startptr = &_str[_start];
		const char* ptr = startptr;
		while(*ptr == ' ') 
			++ptr;

		bool ret;
		if(*ptr == '1')
		{
			++ptr;
			ret = true;
		}
		else if(*ptr == 't' || *ptr == 'T' || *ptr == 'y' || *ptr == 'Y')
		{
			while(isalpha(*ptr))
				++ptr;
			ret = true;
		}
		else if(*ptr == '0')
		{
			++ptr;
			ret = false;
		}
		else if(*ptr == 'f' || *ptr == 'F' || *ptr == 'n' || *ptr == 'N')
		{
			while(isalpha(*ptr))
				++ptr;
			ret = false;
		}
		else
		{
			// fail to convert
			ptr = startptr;
			ret = false;
		}

		_readcount = ptr - startptr;
		return ret;
	}

	String StringConv::boolToString(bool _val)
	{
		if(_val)
			return "1";
		else
			return "0";
	}

	//----------------------------------------------------------------------------
	// float
	float StringConv::floatFromString(const String& _str, size_t _start, size_t& _readcount)
	{
		double d = doubleFromString(_str, _start, _readcount);
		if(d > std::numeric_limits<float>::max())
		{
			d = +std::numeric_limits<float>::infinity();
		}
		else if(d < -std::numeric_limits<float>::max())
		{
			d = -std::numeric_limits<float>::infinity();
		}
		return (float) d;
	}

	String StringConv::floatToString(float _val, size_t _precision)
	{
		return doubleToString(_val, _precision);
	}

	//----------------------------------------------------------------------------
	// double
	static double scanNotFinite(const String& _str, size_t _start, size_t& _readcount)
	{
		const char* startptr = &_str[_start];
		const char* ptr = startptr;
		while(*ptr == ' ') 
			++ptr;

		bool neg = false;
		if(*ptr == '+')
		{
			ptr++;
		}
		else if(*ptr == '-')
		{
			neg = true;
			ptr++;
		}

		double ret;
		if(*ptr == 'i' || *ptr == 'I')
		{
			while(isalpha(*ptr)) 
				++ptr;
			ret = std::numeric_limits<double>::infinity();
			if(neg)
				ret = -ret;
		}
		else
		{
			ptr = startptr;
			ret = 0.0;
		}

		_readcount = ptr - startptr;
		return ret;
	}

	double StringConv::doubleFromString(const String& _str, size_t _start, size_t& _readcount)
	{
		double notfinite = scanNotFinite(_str, _start, _readcount);
		if(notfinite != 0)
		{
			return notfinite;
		}
		else
		{
			char* endptr;
			const char* startptr = &_str[_start];
			double ret = strtod(startptr, &endptr);
			_readcount = endptr - startptr;
			return ret;
		}
	}

	static const char* getPrintfFormat(size_t _precision)
	{
		static const ushort sMaxPrecision = 32;
		static char sFormats[sMaxPrecision + 1][16];
		static bool sInitialised = false;
		if(!sInitialised)
		{
			for(size_t i = 0; i <= sMaxPrecision; ++i)
				sprintf(sFormats[i], "%%.%dG", _precision);
			sInitialised = true;
		}

		if(_precision > sMaxPrecision)
			_precision = sMaxPrecision;
		return sFormats[_precision];
	}

	String StringConv::doubleToString(double _val, size_t _precision)
	{
		if(_val == +std::numeric_limits<double>::infinity())
		{
			return "+infinity";
		}
		else if(_val == -std::numeric_limits<double>::infinity())
		{
			return "-infinity";
		}
		else
		{
			char buf[1024];
			sprintf(buf, getPrintfFormat(_precision), _val);
			return buf;
		}
	}

	//----------------------------------------------------------------------------
	// Vector3
	Vector3 StringConv::vector3FromString(const String& _str, size_t _start, size_t& _readcount)
	{
		Vector3 vec;
		size_t rdc;
		_readcount = 0;

		vec.x = fromString<Real>(_str, _start, rdc);
		if(!rdc)
		{
			_readcount = 0;
			return Vector3::ZERO;
		}
		_readcount += rdc;
		_start += rdc;

		vec.y = fromString<Real>(_str, _start, rdc);
		if(!rdc)
		{
			_readcount = 0;
			return Vector3::ZERO;
		}
		_readcount += rdc;
		_start += rdc;

		vec.z = fromString<Real>(_str, _start, rdc);
		if(!rdc)
		{
			_readcount = 0;
			return Vector3::ZERO;
		}
		_readcount += rdc;
		return vec;
	}

	String StringConv::vector3ToString(const Vector3& _val, size_t _precision)
	{
		String r;
		r += toString(_val.x, _precision);
		r += " ";
		r += toString(_val.y, _precision);
		r += " ";
		r += toString(_val.z, _precision);
		return r;
	}

	//----------------------------------------------------------------------------
	// Quaternion
	Quaternion StringConv::quaternionFromString(const String& _str, size_t _start, size_t& _readcount)
	{
		Vector3 vec = fromString<Vector3>(_str, _start, _readcount);
		if(!_readcount)
			return Quaternion::ZERO;

		Radian xAngle(Degree(vec.x));
		Radian yAngle(Degree(vec.y));
		Radian zAngle(Degree(vec.z));
		Matrix3 mat;
		mat.FromEulerAnglesXYZ(xAngle, yAngle, zAngle);
		Quaternion q(mat);
		return q;
	}

	String StringConv::quaternionToString(const Quaternion& _val, size_t _precision)
	{
		Matrix3 mat;
		_val.ToRotationMatrix(mat);
		Radian xAngle, yAngle, zAngle;
		mat.ToEulerAnglesXYZ(xAngle, yAngle, zAngle);
		Vector3 vec( xAngle.valueDegrees(), yAngle.valueDegrees(), zAngle.valueDegrees());
		return toString(vec, _precision);
	}

	//----------------------------------------------------------------------------
	// AxisAlignedBox
	AxisAlignedBox StringConv::axisAlignedBoxFromString(const String& _str, size_t _start, size_t& _readcount)
	{
		AxisAlignedBox aab;
		size_t rdc;
		_readcount = 0;

		aab.setMinimum( fromString<Vector3>(_str, _start, rdc) );
		if(!rdc)
		{
			_readcount = 0;
			return AxisAlignedBox::BOX_NULL;
		}
		
		_readcount += rdc;
		_start += rdc;

		aab.setMaximum( fromString<Vector3>(_str, _start, rdc) );
		if(!rdc)
		{
			_readcount = 0;
			return AxisAlignedBox::BOX_NULL;
		}

		_readcount += rdc;
		return aab;
	}

	String StringConv::axisAlignedBoxToString(const AxisAlignedBox& _val, size_t _precision)
	{
		String r;
		r += toString(_val.getMinimum(), _precision);
		r += " ";
		r += toString(_val.getMaximum(), _precision);
		return r;
	}

	//----------------------------------------------------------------------------
	// ColourValue
	ColourValue StringConv::colourValueFromString(const String& _str, size_t _start, size_t& _readcount)
	{
		ColourValue clr;
		size_t rdc;
		_readcount = 0;

		clr.r = fromString<int>(_str, _start, rdc) / Real(255.f);
		if(!rdc)
		{
			_readcount = 0;
			return ColourValue::Black;
		}
		_readcount += rdc;
		_start += rdc;		

		clr.g = fromString<Real>(_str, _start, rdc) / Real(255.f);
		if(!rdc)
		{
			_readcount = 0;
			return ColourValue::Black;
		}
		_readcount += rdc;
		_start += rdc;		

		clr.b = fromString<Real>(_str, _start, rdc) / Real(255.f);
		if(!rdc)
		{
			_readcount = 0;
			return ColourValue::Black;
		}
		_readcount += rdc;
		_start += rdc;		

		clr.a = fromString<Real>(_str, _start, rdc) / Real(255.f);
		if(!rdc)
		{
			_readcount = 0;
			return ColourValue::Black;
		}
		_readcount += rdc;

		return clr;
	}
		
	String StringConv::colourValueToString(const ColourValue& _val)
	{
		String r;
		r += toString(Math::IFloor(_val.r * 255 + 0.5f));
		r += " ";
		r += toString(Math::IFloor(_val.g * 255 + 0.5f));
		r += " ";
		r += toString(Math::IFloor(_val.b * 255 + 0.5f));
		r += " ";
		r += toString(Math::IFloor(_val.a * 255 + 0.5f));
		return r;
	}


} // namespace GothOgre