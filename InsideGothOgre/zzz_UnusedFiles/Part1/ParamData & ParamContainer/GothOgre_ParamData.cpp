#include "GothOgre_StdHeaders.h"
#include "GothOgre_ParamData.h"

namespace GothOgre
{
	#define EXCEPT_UNEXPECTED_TYPE(type) \
		GOTHOGRE_EXCEPT("ParamData: Unexpected ParamType value: " << type)

	#define ASSERTCHECK_TYPE(type) \
		GOTHOGRE_ASSERT(getType() == type, "ParamData: The operation is allowed for '" #type "' type only.")

	#define ASSERTCHECK_ARRAY_TYPE(type, size) \
		ASSERTCHECK_TYPE(type); \
		GOTHOGRE_ASSERT(getArraySize() == size, "ParamData: The array's size does not match the ParamDesc")
	//---------------------------------------------------------------------------------------------------
	const ParamData& ParamData::operator =(const ParamData& _other)
	{
		setDesc(_other.getDesc(), false);
		switch(getType())
		{
			case NIL:     break;
			case BOOL:    setBool(_other.getBool()); break;
			case CHAR:    setChar(_other.getChar()); break;
			case SHORT:   setShort(_other.getShort()); break;
			case LONG:    setLong(_other.getLong()); break;
			case UCHAR:   setUChar(_other.getUChar()); break;
			case USHORT:  setUShort(_other.getUShort()); break;
			case ULONG:   setULong(_other.getULong()); break;
			case REAL:    setReal(_other.getReal()); break;
			case ENUM:    setEnum(_other.getEnum()); break;
			case STRING:  setString(_other.getString()); break;
			case COLOR:   setColor(_other.getColor()); break;
			case VECTOR3: setVector3(_other.getVector3()); break;
			case VECTOR4: setVector4(_other.getVector4()); break;
			case MATRIX3: setMatrix3(_other.getMatrix3()); break;
			case MATRIX4: setMatrix4(_other.getMatrix4()); break;
			case CHAR_ARRAY:   setCharArray(_other.getCharArray(), _other.getArraySize()); break;
			case SHORT_ARRAY:  setShortArray(_other.getShortArray(), _other.getArraySize()); break;
			case LONG_ARRAY:   setLongArray(_other.getLongArray(), _other.getArraySize()); break;
			case UCHAR_ARRAY:  setUCharArray(_other.getUCharArray(), _other.getArraySize()); break;
			case USHORT_ARRAY: setUShortArray(_other.getUShortArray(), _other.getArraySize()); break;
			case ULONG_ARRAY:  setULongArray(_other.getULongArray(), _other.getArraySize()); break;
			case REAL_ARRAY:   setRealArray(_other.getRealArray(), _other.getArraySize()); break;
			case VOID_PTR:     setVoidPtr(_other.getVoidPtr()); break;
			default: EXCEPT_UNEXPECTED_TYPE(getType());
		}
		return *this;
	}
	//---------------------------------------------------------------------------------------------------
	bool ParamData::operator ==(const ParamData& _other) const
	{
		if(*getDesc() != *_other.getDesc())
			return false;

		switch(getType())
		{
			case NIL:    return true;
			case BOOL:   return getBool() == _other.getBool();
			case CHAR:   return getChar() == _other.getChar();
			case SHORT:  return getShort() == _other.getShort();
			case LONG:   return getLong() == _other.getLong();
			case UCHAR:  return getUChar() == _other.getUChar();
			case USHORT: return getUShort() == _other.getUShort();
			case ULONG:  return getULong() == _other.getULong();
			case REAL:   return getReal() == _other.getReal();
			case ENUM:   return getEnum() == _other.getEnum();
			case STRING:  return getString() == _other.getString();
			case COLOR:   return getColor() == _other.getColor();
			case VECTOR3: return getVector3() == _other.getVector3();
			case VECTOR4: return getVector4() == _other.getVector4();
			case MATRIX3: return getMatrix3() == _other.getMatrix3();
			case MATRIX4: return getMatrix4() == _other.getMatrix4();
			case CHAR_ARRAY:   return memcmp(getCharArray(), _other.getCharArray(), getArraySize() * sizeof(char)) == 0;
			case SHORT_ARRAY:  return memcmp(getShortArray(), _other.getShortArray(), getArraySize() * sizeof(short)) == 0;
			case LONG_ARRAY:   return memcmp(getLongArray(), _other.getLongArray(), getArraySize() * sizeof(long)) == 0;
			case UCHAR_ARRAY:  return memcmp(getUCharArray(), _other.getUCharArray(), getArraySize() * sizeof(uchar)) == 0;
			case USHORT_ARRAY: return memcmp(getUShortArray(), _other.getUShortArray(), getArraySize() * sizeof(ushort)) == 0;
			case ULONG_ARRAY:  return memcmp(getULongArray(), _other.getULongArray(), getArraySize() * sizeof(ulong)) == 0;
			case REAL_ARRAY:   return memcmp(getRealArray(), _other.getRealArray(), getArraySize() * sizeof(Real)) == 0;
			case VOID_PTR:     return getVoidPtr() == _other.getVoidPtr();
			default: EXCEPT_UNEXPECTED_TYPE(getType());
		}
	}
	//---------------------------------------------------------------------------------------------------
	bool ParamData::operator !=(const ParamData& _other) const
	{
		return !(*this == _other);
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::free()
	{
		switch(getType())
		{
			case NIL:
			case BOOL:
			case CHAR:
			case SHORT:
			case LONG:
			case UCHAR:
			case USHORT:
			case ULONG:
			case REAL:
			case ENUM: break;
			case STRING:  delete mStringPtr; break;
			case COLOR:   delete mColorPtr; break;
			case VECTOR3: delete mVector3Ptr; break;
			case VECTOR4: delete mVector4Ptr; break;
			case MATRIX3: delete mMatrix3Ptr; break;
			case MATRIX4: delete mMatrix4Ptr; break;
			case CHAR_ARRAY:   delete[] mCharArray; break;
			case SHORT_ARRAY:  delete[] mShortArray; break;
			case LONG_ARRAY:   delete[] mLongArray; break;
			case UCHAR_ARRAY:  delete[] mUCharArray; break;
			case USHORT_ARRAY: delete[] mUShortArray; break;
			case ULONG_ARRAY:  delete[] mULongArray; break;
			case REAL_ARRAY:   delete[] mRealArray; break;
			case VOID_PTR: break;
			default: EXCEPT_UNEXPECTED_TYPE(getType());
		}
		mDesc = nullptr;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setDesc(const ParamDesc* _paramDesc, bool setDefaultValue)
	{
		if(mDesc != _paramDesc)
		{
			free();
			mDesc = _paramDesc;
			switch(getType())
			{
				case NIL:     break;
				case BOOL:    if(setDefaultValue) mBoolValue   = false; break;
				case CHAR:    if(setDefaultValue) mCharValue   = 0; break;
				case SHORT:   if(setDefaultValue) mShortValue  = 0; break;
				case LONG:    if(setDefaultValue) mLongValue   = 0; break;
				case UCHAR:   if(setDefaultValue) mUCharValue  = 0; break;
				case USHORT:  if(setDefaultValue) mUShortValue = 0; break;
				case ULONG:   if(setDefaultValue) mULongValue  = 0; break;
				case REAL:    if(setDefaultValue) mRealValue   = 0; break;
				case ENUM:    if(setDefaultValue) mEnumValue   = 0; break;
				case STRING:  mStringPtr  = new String(); break;
				case COLOR:   mColorPtr   = new ColourValue(); break;
				case VECTOR3: mVector3Ptr = new Vector3(); if(setDefaultValue) *mVector3Ptr = Vector3::ZERO; break;
				case VECTOR4: mVector4Ptr = new Vector4(); if(setDefaultValue) *mVector4Ptr = Vector4::ZERO; break;
				case MATRIX3: mMatrix3Ptr = new Matrix3(); if(setDefaultValue) *mMatrix3Ptr = Matrix3::ZERO; break;
				case MATRIX4: mMatrix4Ptr = new Matrix4(); if(setDefaultValue) *mMatrix4Ptr = Matrix4::ZERO; break;
				case CHAR_ARRAY:   mCharArray   = new char  [getArraySize()]; if(setDefaultValue) memset(mCharArray,   0, getArraySize() * sizeof(char)); break;
				case SHORT_ARRAY:  mShortArray  = new short [getArraySize()]; if(setDefaultValue) memset(mShortArray,  0, getArraySize() * sizeof(short)); break;
				case LONG_ARRAY:   mLongArray   = new long  [getArraySize()]; if(setDefaultValue) memset(mLongArray,   0, getArraySize() * sizeof(long)); break;
				case UCHAR_ARRAY:  mUCharArray  = new uchar [getArraySize()]; if(setDefaultValue) memset(mUCharArray,  0, getArraySize() * sizeof(uchar)); break;
				case USHORT_ARRAY: mUShortArray = new ushort[getArraySize()]; if(setDefaultValue) memset(mUShortArray, 0, getArraySize() * sizeof(ushort)); break;
				case ULONG_ARRAY:  mULongArray  = new ulong [getArraySize()]; if(setDefaultValue) memset(mULongArray,  0, getArraySize() * sizeof(ulong)); break;
				case REAL_ARRAY:   mRealArray   = new Real  [getArraySize()]; if(setDefaultValue) memset(mRealArray,   0, getArraySize() * sizeof(Real)); break;
				case VOID_PTR: if(setDefaultValue) setVoidPtr(nullptr); break;
				default: EXCEPT_UNEXPECTED_TYPE(getType());
			}
		}
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setBool(bool _value)
	{
		ASSERTCHECK_TYPE(BOOL);
		mBoolValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	bool ParamData::getBool() const
	{
		ASSERTCHECK_TYPE(BOOL);
		return mBoolValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setChar(char _value)
	{
		ASSERTCHECK_TYPE(CHAR);
		mCharValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	char ParamData::getChar() const
	{
		ASSERTCHECK_TYPE(CHAR);
		return mCharValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setShort(short _value)
	{
		ASSERTCHECK_TYPE(SHORT);
		mShortValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	short ParamData::getShort() const
	{
		ASSERTCHECK_TYPE(SHORT);
		return mShortValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setLong(long _value)
	{
		ASSERTCHECK_TYPE(LONG);
		mLongValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	long ParamData::getLong() const
	{
		ASSERTCHECK_TYPE(LONG);
		return mLongValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setUChar(uchar _value)
	{
		ASSERTCHECK_TYPE(UCHAR);
		mUCharValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	uchar ParamData::getUChar() const
	{
		ASSERTCHECK_TYPE(UCHAR);
		return mUCharValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setUShort(ushort _value)
	{
		ASSERTCHECK_TYPE(USHORT);
		mUShortValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	ushort ParamData::getUShort() const
	{
		ASSERTCHECK_TYPE(USHORT);
		return mUShortValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setULong(ulong _value)
	{
		ASSERTCHECK_TYPE(ULONG);
		mULongValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	ulong ParamData::getULong() const
	{
		ASSERTCHECK_TYPE(ULONG);
		return mULongValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setReal(Real _value)
	{
		ASSERTCHECK_TYPE(REAL);
		mRealValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	Real ParamData::getReal() const
	{
		ASSERTCHECK_TYPE(REAL);
		return mRealValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setEnum(int _value)
	{
		ASSERTCHECK_TYPE(ENUM);
		mEnumValue = _value;
	}
	//---------------------------------------------------------------------------------------------------
	int ParamData::getEnum() const
	{
		ASSERTCHECK_TYPE(ENUM);
		return mEnumValue;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setString(const String& _value)
	{
		ASSERTCHECK_TYPE(STRING);
		*mStringPtr = _value;
	}
	//---------------------------------------------------------------------------------------------------
	const String& ParamData::getString() const
	{
		ASSERTCHECK_TYPE(STRING);
		return *mStringPtr;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setColor(const ColourValue& _value)
	{
		ASSERTCHECK_TYPE(COLOR);
		*mColorPtr = _value;
	}
	//---------------------------------------------------------------------------------------------------
	const ColourValue& ParamData::getColor() const
	{
		ASSERTCHECK_TYPE(COLOR);
		return *mColorPtr;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setVector3(const Vector3& _value)
	{
		ASSERTCHECK_TYPE(VECTOR3);
		*mVector3Ptr = _value;
	}
	//---------------------------------------------------------------------------------------------------
	const Vector3& ParamData::getVector3() const
	{
		ASSERTCHECK_TYPE(VECTOR3);
		return *mVector3Ptr;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setVector4(const Vector4& _value)
	{
		ASSERTCHECK_TYPE(VECTOR4);
		*mVector4Ptr = _value;
	}
	//---------------------------------------------------------------------------------------------------
	const Vector4& ParamData::getVector4() const
	{
		ASSERTCHECK_TYPE(VECTOR4);
		return *mVector4Ptr;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setMatrix3(const Matrix3& _value)
	{
		ASSERTCHECK_TYPE(MATRIX3);
		*mMatrix3Ptr = _value;
	}
	//---------------------------------------------------------------------------------------------------
	const Matrix3& ParamData::getMatrix3() const
	{
		ASSERTCHECK_TYPE(MATRIX3);
		return *mMatrix3Ptr;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setMatrix4(const Matrix4& _value)
	{
		ASSERTCHECK_TYPE(MATRIX4);
		*mMatrix4Ptr = _value;
	}
	//---------------------------------------------------------------------------------------------------
	const Matrix4& ParamData::getMatrix4() const
	{
		ASSERTCHECK_TYPE(MATRIX4);
		return *mMatrix4Ptr;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setCharArray(const char* _array, size_t _size)
	{
		ASSERTCHECK_ARRAY_TYPE(CHAR_ARRAY, _size);
		memcpy(mCharArray, _array, _size * sizeof(char));
	}
	//---------------------------------------------------------------------------------------------------
	const char* ParamData::getCharArray() const
	{
		ASSERTCHECK_TYPE(CHAR_ARRAY);
		return mCharArray;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setShortArray(const short* _array, size_t _size)
	{
		ASSERTCHECK_ARRAY_TYPE(SHORT_ARRAY, _size);
		memcpy(mShortArray, _array, _size * sizeof(short));
	}
	//---------------------------------------------------------------------------------------------------
	const short* ParamData::getShortArray() const
	{
		ASSERTCHECK_TYPE(SHORT_ARRAY);
		return mShortArray;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setLongArray(const long* _array, size_t _size)
	{
		ASSERTCHECK_ARRAY_TYPE(LONG_ARRAY, _size);
		memcpy(mLongArray, _array, _size * sizeof(long));
	}
	//---------------------------------------------------------------------------------------------------
	const long* ParamData::getLongArray() const
	{
		ASSERTCHECK_TYPE(LONG_ARRAY);
		return mLongArray;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setUCharArray(const uchar* _array, size_t _size)
	{
		ASSERTCHECK_ARRAY_TYPE(UCHAR_ARRAY, _size);
		memcpy(mUCharArray, _array, _size * sizeof(uchar));
	}

	//---------------------------------------------------------------------------------------------------
	const uchar* ParamData::getUCharArray() const
	{
		ASSERTCHECK_TYPE(UCHAR_ARRAY);
		return mUCharArray;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setUShortArray(const ushort* _array, size_t _size)
	{
		ASSERTCHECK_ARRAY_TYPE(USHORT_ARRAY, _size);
		memcpy(mUShortArray, _array, _size * sizeof(ushort));
	}
	//---------------------------------------------------------------------------------------------------
	const ushort* ParamData::getUShortArray() const
	{
		ASSERTCHECK_TYPE(USHORT_ARRAY);
		return mUShortArray;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setULongArray(const ulong* _array, size_t _size)
	{
		ASSERTCHECK_ARRAY_TYPE(ULONG_ARRAY, _size);
		memcpy(mULongArray, _array, _size * sizeof(ulong));
	}
	//---------------------------------------------------------------------------------------------------
	const ulong* ParamData::getULongArray() const
	{
		ASSERTCHECK_TYPE(ULONG_ARRAY);
		return mULongArray;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setRealArray(const Real* _array, size_t _size)
	{
		ASSERTCHECK_ARRAY_TYPE(REAL_ARRAY, _size);
		memcpy(mRealArray, _array, _size * sizeof(Real));
	}
	//---------------------------------------------------------------------------------------------------
	const Real* ParamData::getRealArray() const
	{
		ASSERTCHECK_TYPE(REAL_ARRAY);
		return mRealArray;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamData::setVoidPtr(const void* _value)
	{
		ASSERTCHECK_TYPE(VOID_PTR);
		mVoidPtr = _value;
	}
	//---------------------------------------------------------------------------------------------------
	const void* ParamData::getVoidPtr() const
	{
		ASSERTCHECK_TYPE(VOID_PTR);
		return mVoidPtr;
	}

} // namespace GothOgre
