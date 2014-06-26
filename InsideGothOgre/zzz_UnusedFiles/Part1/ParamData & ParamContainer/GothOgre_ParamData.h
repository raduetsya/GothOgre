#ifndef GOTHOGRE_PARAM_DATA_H
#define GOTHOGRE_PARAM_DATA_H

#include "GothOgre_ParamDesc.h"

namespace GothOgre
{
	/// This class can hold values of many different types. 
	/// It can be stored in ParamContainer objects.
	class ParamData : public ParamType
	{
	public:
		/// Creates a NIL data with no value
		ParamData();

		/// Copy constructor.
		ParamData(const ParamData& _src);

		/// Constructs a default data for the specified type 
		ParamData(const ParamDesc* _paramDesc);

		// Constructs by the different ways
		ParamData(const ParamDesc* _paramDesc, bool _value);
		ParamData(const ParamDesc* _paramDesc, char _value);
		ParamData(const ParamDesc* _paramDesc, short _value);
		ParamData(const ParamDesc* _paramDesc, long _value);
		ParamData(const ParamDesc* _paramDesc, uchar _value);
		ParamData(const ParamDesc* _paramDesc, ushort _value);
		ParamData(const ParamDesc* _paramDesc, ulong _value);
		ParamData(const ParamDesc* _paramDesc, Real _value);
		ParamData(const ParamDesc* _paramDesc, const String& _value);
		ParamData(const ParamDesc* _paramDesc, const ColourValue& _value);
		ParamData(const ParamDesc* _paramDesc, const Vector3& _value);
		ParamData(const ParamDesc* _paramDesc, const Vector4& _value);
		ParamData(const ParamDesc* _paramDesc, const Matrix3& _value);
		ParamData(const ParamDesc* _paramDesc, const Matrix4& _value);
		ParamData(const ParamDesc* _paramDesc, const char* _array, size_t _size);
		ParamData(const ParamDesc* _paramDesc, const short* _array, size_t _size);
		ParamData(const ParamDesc* _paramDesc, const long* _array, size_t _size);
		ParamData(const ParamDesc* _paramDesc, const uchar* _array, size_t _size);
		ParamData(const ParamDesc* _paramDesc, const ushort* _array, size_t _size);
		ParamData(const ParamDesc* _paramDesc, const ulong* _array, size_t _size);
		ParamData(const ParamDesc* _paramDesc, const Real* _array, size_t _size);
		ParamData(const ParamDesc* _paramDesc, const void* _voidPtr);

		/// Class destructor.
		~ParamData();

		/// Assignment operator.
		const ParamData& operator =(const ParamData& _other);

		/// Equality operator.
		bool operator ==(const ParamData& _other) const;

		/// Inequality operator.
		bool operator !=(const ParamData& _other) const;

		/// Sets the parameter's description
		void setDesc(const ParamDesc* _paramDesc);

		/// Returns the parameter's description
		const ParamDesc* getDesc() const;

		/// Returns the parameter's type
		Type getType() const;

		/// Returns size of the array.
		/// If the data has not an array type, 
		/// the function will throw an exception.
		size_t getArraySize() const;

		// Access to data
		void setBool(bool _value);
		bool getBool() const;

		void setChar(char _value);
		char getChar() const;

		void setShort(short _value);
		short getShort() const;

		void setLong(long _value);
		long getLong() const;

		void setUChar(uchar _value);
		uchar getUChar() const;

		void setUShort(ushort _value);
		ushort getUShort() const;

		void setULong(ulong _value);
		ulong getULong() const;

		void setReal(Real _value);
		Real getReal() const;

		void setEnum(int _value);
		int getEnum() const;

		void setString(const String& _value);
		const String& getString() const;

		void setColor(const ColourValue& _value);
		const ColourValue& getColor() const;

		void setVector3(const Vector3& _value);
		const Vector3& getVector3() const;

		void setVector4(const Vector4& _value);
		const Vector4& getVector4() const;

		void setMatrix3(const Matrix3& _value);
		const Matrix3& getMatrix3() const;

		void setMatrix4(const Matrix4& _value);
		const Matrix4& getMatrix4() const;

		void setCharArray(const char* _array, size_t _size);
		const char* getCharArray() const;

		void setShortArray(const short* _array, size_t _size);
		const short* getShortArray() const;

		void setLongArray(const long* _array, size_t _size);
		const long* getLongArray() const;

		void setUCharArray(const uchar* _array, size_t _size);
		const uchar* getUCharArray() const;

		void setUShortArray(const ushort* _array, size_t _size);
		const ushort* getUShortArray() const;

		void setULongArray(const ulong* _array, size_t _size);
		const ulong* getULongArray() const;

		void setRealArray(const Real* _array, size_t _size);
		const Real* getRealArray() const;

		void setVoidPtr(const void* _value);
		const void* getVoidPtr() const;

	private:
		void init();
		void free();
		void setDesc(const ParamDesc* _paramDesc, bool setDefaultValue);

	private:
		const ParamDesc*	mDesc;
		union
		{
			bool			mBoolValue;
			char            mCharValue;
			short           mShortValue;
			long            mLongValue;
			uchar           mUCharValue;
			ushort          mUShortValue;
			ulong           mULongValue;
			Real			mRealValue;
			int             mEnumValue;
			const void*		mVoidPtr;
			String*			mStringPtr;
			ColourValue*	mColorPtr;
			Vector3*		mVector3Ptr;
			Vector4*		mVector4Ptr;
			Matrix3*		mMatrix3Ptr;
			Matrix4*		mMatrix4Ptr;
			char*           mCharArray;
			short*          mShortArray;
			long*           mLongArray;
			uchar*          mUCharArray;
			ushort*         mUShortArray;
			ulong*          mULongArray;
			Real*           mRealArray;
		};
	};
	//---------------------------------------------------------------------------------------------------
	inline void ParamData::init()
	{
		mDesc = nullptr;
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData()
	{
		init();
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamData& _src)
	{
		init();
		*this = _src;
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc)
	{
		init();
		setDesc(_paramDesc, true);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, bool _value)
	{
		init();
		setDesc(_paramDesc, false);
		setBool(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, char _value)
	{
		init();
		setDesc(_paramDesc, false);
		setChar(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, short _value)
	{
		init();
		setDesc(_paramDesc, false);
		setShort(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, long _value)
	{
		init();
		setDesc(_paramDesc, false);
		setLong(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, uchar _value)
	{
		init();
		setDesc(_paramDesc, false);
		setUChar(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, ushort _value)
	{
		init();
		setDesc(_paramDesc, false);
		setUShort(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, ulong _value)
	{
		init();
		setDesc(_paramDesc, false);
		setULong(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, Real _value)
	{
		init();
		setDesc(_paramDesc, false);
		setReal(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const String& _value)
	{
		init();
		setDesc(_paramDesc, false);
		setString(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const ColourValue& _value)
	{
		init();
		setDesc(_paramDesc, false);
		setColor(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const Vector3& _value)
	{
		init();
		setDesc(_paramDesc, false);
		setVector3(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const Vector4& _value)
	{
		init();
		setDesc(_paramDesc, false);
		setVector4(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const Matrix3& _value)
	{
		init();
		setDesc(_paramDesc, false);
		setMatrix3(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const Matrix4& _value)
	{
		init();
		setDesc(_paramDesc, false);
		setMatrix4(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const char* _array, size_t _size)
	{
		init();
		setDesc(_paramDesc, false);
		setCharArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const short* _array, size_t _size)
	{
		init();
		setDesc(_paramDesc, false);
		setShortArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const long* _array, size_t _size)
	{
		init();
		setDesc(_paramDesc, false);
		setLongArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const uchar* _array, size_t _size)
	{
		init();
		setDesc(_paramDesc, false);
		setUCharArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const ushort* _array, size_t _size)
	{
		init();
		setDesc(_paramDesc, false);
		setUShortArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const ulong* _array, size_t _size)
	{
		init();
		setDesc(_paramDesc, false);
		setULongArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const Real* _array, size_t _size)
	{
		init();
		setDesc(_paramDesc, false);
		setRealArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::ParamData(const ParamDesc* _paramDesc, const void* _voidPtr)
	{
		init();
		setDesc(_paramDesc, false);
		setVoidPtr(_voidPtr);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::~ParamData()
	{
		free();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamData::setDesc(const ParamDesc *_paramDesc)
	{
		setDesc(_paramDesc, true);
	}
	//---------------------------------------------------------------------------------------------------
	inline const ParamDesc* ParamData::getDesc() const
	{
		return mDesc;
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamData::Type ParamData::getType() const
	{
		if(mDesc)
			return mDesc->getType();
		return NIL;
	}
	//---------------------------------------------------------------------------------------------------
	inline size_t ParamData::getArraySize() const
	{
		return mDesc->getArraySize();
	}

} // namespace GothOgre

#endif // GOTHOGRE_PARAM_DATA_H