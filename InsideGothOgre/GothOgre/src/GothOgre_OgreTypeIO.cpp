#include "GothOgre_Precompiled.h"
#include "GothOgre_OgreTypeIO.h"
#include "GothOgre_EnumIO.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	// Vector2
	//---------------------------------------------------------------------------
	StrStream& operator <<(StrStream& _ss, const Vector2& _vec)
	{
		_ss << _vec.x;
		_ss << ' ';
		_ss << _vec.y;
		return _ss;
	}
	//---------------------------------------------------------------------------
	StrStream& operator >>(StrStream& _ss, Vector2& _vec)
	{
		if(_ss.fail())
			return _ss;

		_ss >> _vec.x;
		if(_ss.fail())
			return _ss;

		_ss >> _vec.y;
		return _ss;
	}



	//---------------------------------------------------------------------------
	// Vector3
	//---------------------------------------------------------------------------
	StrStream& operator <<(StrStream& _ss, const Vector3& _vec)
	{
		_ss << _vec.x;
		_ss << ' ';
		_ss << _vec.y;
		_ss << ' ';
		_ss << _vec.z;
		return _ss;
	}
	//---------------------------------------------------------------------------
	StrStream& operator >>(StrStream& _ss, Vector3& _vec)
	{
		if(_ss.fail())
			return _ss;

		_ss >> _vec.x;
		if(_ss.fail())
			return _ss;

		_ss >> _vec.y;
		if(_ss.fail())
			return _ss;

		_ss >> _vec.z;
		return _ss;
	}


	//---------------------------------------------------------------------------
	// Quaternion
	//---------------------------------------------------------------------------
	StrStream& operator <<(StrStream& _ss, const Quaternion& _quat)
	{
		Matrix3 mat;
		_quat.ToRotationMatrix(mat);
		Radian xAngle, yAngle, zAngle;
		mat.ToEulerAnglesXYZ(xAngle, yAngle, zAngle);
		Vector3 vec( xAngle.valueDegrees(), yAngle.valueDegrees(), zAngle.valueDegrees());
		return _ss << vec;
	}
	//---------------------------------------------------------------------------
	StrStream& operator >>(StrStream& _ss, Quaternion& _quat)
	{
		Vector3 vec;
		_ss >> vec;
		if(_ss.fail())
			return _ss;

		Radian xAngle(Degree(vec.x));
		Radian yAngle(Degree(vec.y));
		Radian zAngle(Degree(vec.z));
		Matrix3 mat;
		mat.FromEulerAnglesXYZ(xAngle, yAngle, zAngle);
		_quat = Quaternion(mat);
		return _ss;
	}


	//---------------------------------------------------------------------------
	// AxisAlignedBox
	//---------------------------------------------------------------------------
	StrStream& operator <<(StrStream& _ss, const AxisAlignedBox& _aab)
	{
		_ss << _aab.getMinimum();
		_ss << ' ';
		_ss << _aab.getMaximum();
		return _ss;
	}
	//---------------------------------------------------------------------------
	StrStream& operator >>(StrStream& _ss, AxisAlignedBox& _aab)
	{
		if(_ss.fail())
			return _ss;

		Vector3 mn;
		_ss >> mn;
		if(_ss.fail())
			return _ss;
		_aab.setMinimum(mn);

		Vector3 mx;
		_ss >> mx;
		if(_ss.fail())
			return _ss;
		_aab.setMaximum(mx);

		return _ss;
	}
	
	
	//---------------------------------------------------------------------------
	// ColourValue
	//---------------------------------------------------------------------------
	StrStream& operator <<(StrStream& _ss, const ColourValue& _clr)
	{
		_ss << Math::IFloor(_clr.r * 255);
		_ss << ' ';
		_ss << Math::IFloor(_clr.g * 255);
		_ss << ' ';
		_ss << Math::IFloor(_clr.b * 255);
		_ss << ' ';
		_ss << Math::IFloor(_clr.a * 255);
		return _ss;
	}
	//---------------------------------------------------------------------------
	StrStream& operator >>(StrStream& _ss, ColourValue& _clr)
	{
		int r, g, b, a;
		if(_ss.fail())
			return _ss;

		_ss >> r;
		if(_ss.fail())
			return _ss;

		_ss >> g;
		if(_ss.fail())
			return _ss;

		_ss >> b;
		if(_ss.fail())
			return _ss;

		_ss >> a;
		if(_ss.fail())
			return _ss;

		if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255)
		{
			_ss.seekg(-1); // set error state
			return _ss;
		}

		_clr.r = Real(r) / 255.f;
		_clr.g = Real(g) / 255.f;
		_clr.b = Real(b) / 255.f;
		_clr.a = Real(a) / 255.f;
		return _ss;
	}


	//---------------------------------------------------------------------------
	// PolygonMode
	//---------------------------------------------------------------------------
	class PolygonModeWrapper
	{
	public:
		typedef PolygonMode Enum;
		PolygonModeWrapper(int _value = 0) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
		operator PolygonMode() const {return (PolygonMode) mValue;}
		GOTHOGRE_DECLARE_ENUM_IO(PolygonModeWrapper);

	private:
		Enum mValue;
	};

	GOTHOGRE_IMPLEMENT_ENUM_IO( PolygonModeWrapper )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT( PM_POINTS )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT( PM_WIREFRAME )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT( PM_SOLID )
	GOTHOGRE_IMPLEMENT_ENUM_IO_END
	//---------------------------------------------------------------------------
	StrStream& operator <<(StrStream& _ss, const PolygonMode& _pm)
	{
		PolygonModeWrapper pm2 = _pm;
		return _ss << pm2;
	}
	//---------------------------------------------------------------------------
	StrStream& operator >>(StrStream& _ss, PolygonMode& _pm)
	{
		PolygonModeWrapper pm2;
		_ss >> pm2;
		_pm = pm2;
		return _ss;
	}

} // namespace GothOgre