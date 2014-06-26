#include "GothOgre_StdHeaders.h"
#include "GothOgre_ParamDesc.h"


namespace GothOgre
{
	#define ASSERTCHECK_TYPE(type) \
		GOTHOGRE_ASSERT(getType() == type, \
		"ParamDesc: The operation is allowed for '" #type "' type only")

	#define ASSERTCHECK_TYPE_OR_TYPE(type, arrtype) \
		GOTHOGRE_ASSERT(getType() == type || getType() == arrtype, \
		"ParamDesc: The operation is allowed for '" #type "' and '" #arrtype "' types only")

	#define ASSERTCHECK_ANY_ARRTYPE \
		GOTHOGRE_ASSERT(getType() == CHAR_ARRAY || getType() == SHORT_ARRAY \
			|| getType() == LONG_ARRAY || getType() == UCHAR_ARRAY || getType() == USHORT_ARRAY \
			|| getType() == ULONG_ARRAY || getType() == REAL_ARRAY, \
			"ParamDesc: The operation is allowed for array types only")
	//----------------------------------------------------------------------------------------------	
	struct Internal_Enum
	{
		map<String, int>::type mNameValueMap;
	};

	struct Internal_Array
	{
		Internal_Array() : mSize(0) {}
		size_t mSize;
	};

	template<typename T>
	struct Internal_Numeric
	{
		Internal_Numeric() : mRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max()) {}
		Range<T> mRange;
	};

	template<typename T>
	struct Internal_NumericArray : public Internal_Array, public Internal_Numeric<T>
	{
	};
	//----------------------------------------------------------------------------------------------	
	struct ParamDescInternal
	{
		virtual ~ParamDescInternal() {}
	};

	struct ParamDescInternal_Enum : public Internal_Enum, public ParamDescInternal {};
	struct ParamDescInternal_Array : public Internal_Array, public ParamDescInternal {};

	template<typename T>
	struct ParamDescInternal_Numeric : public Internal_Numeric<T>, public ParamDescInternal {};

	template<typename T>
	struct ParamDescInternal_NumericArray : public Internal_NumericArray<T>, public ParamDescInternal {};
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::init()
	{
		mType = NIL;
		mInternal = nullptr;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::free()
	{
		if(mInternal)
		{
			delete mInternal;
			mInternal = nullptr;
		}
		mType = NIL;
	}
	//----------------------------------------------------------------------------------------------	
	const ParamDesc& ParamDesc::operator =(const ParamDesc& _src)
	{
		setType(_src.getType());
		setName(_src.getName());
		setDescription(_src.getDescription());
		switch(mType)
		{
			case ENUM:         setEnumNames(_src.getEnumNames()); break;
			case CHAR_ARRAY:   setArraySize(_src.getArraySize()); // no break here and it's not an error
			case CHAR:         setCharRange(_src.getCharRange()); break;
			case SHORT_ARRAY:  setArraySize(_src.getArraySize());
			case SHORT:        setShortRange(_src.getShortRange()); break;
			case LONG_ARRAY:   setArraySize(_src.getArraySize());
			case LONG:         setLongRange(_src.getLongRange()); break;
			case UCHAR_ARRAY:  setArraySize(_src.getArraySize());
			case UCHAR:        setUCharRange(_src.getUCharRange()); break;
			case USHORT_ARRAY: setArraySize(_src.getArraySize());
			case USHORT:       setUShortRange(_src.getUShortRange()); break;
			case ULONG_ARRAY:  setArraySize(_src.getArraySize());
			case ULONG:        setULongRange(_src.getULongRange()); break;
			case REAL_ARRAY:   setArraySize(_src.getArraySize());
			case REAL:         setRealRange(_src.getRealRange()); break;
		}
		return *this;
	}
	//----------------------------------------------------------------------------------------------	
	bool ParamDesc::operator ==(const ParamDesc& _other) const
	{
		if(getType() != _other.getType())
			return false;

		if(getName() != _other.getName())
			return false;

		if(getDescription() != _other.getDescription())
			return false;

		switch(mType)
		{
			case ENUM:         return getEnumNames() == _other.getEnumNames();
			case CHAR_ARRAY:   if(getArraySize() != _other.getArraySize()) return false;
			case CHAR:         return getCharRange() == _other.getCharRange();
			case SHORT_ARRAY:  if(getArraySize() != _other.getArraySize()) return false;
			case SHORT:        return getShortRange() == _other.getShortRange();
			case LONG_ARRAY:   if(getArraySize() != _other.getArraySize()) return false;
			case LONG:         return getLongRange() == _other.getLongRange();
			case UCHAR_ARRAY:  if(getArraySize() != _other.getArraySize()) return false;
			case UCHAR:        return getUCharRange() == _other.getUCharRange();
			case USHORT_ARRAY: if(getArraySize() != _other.getArraySize()) return false;
			case USHORT:       return getUShortRange() == _other.getUShortRange();
			case ULONG_ARRAY:  if(getArraySize() != _other.getArraySize()) return false;
			case ULONG:        return getULongRange() == _other.getULongRange();
			case REAL_ARRAY:   if(getArraySize() != _other.getArraySize()) return false;
			case REAL:         return getRealRange() == _other.getRealRange();
			default:           return true;
		}
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setType(Type _type)
	{
		if(mType != _type)
		{
			free();
			mType = _type;
			switch(mType)
			{
				case ENUM:   mInternal = new ParamDescInternal_Enum(); break;
				case CHAR:   mInternal = new ParamDescInternal_Numeric<char>(); break;
				case SHORT:  mInternal = new ParamDescInternal_Numeric<short>(); break;
				case LONG:   mInternal = new ParamDescInternal_Numeric<long>(); break;
				case UCHAR:  mInternal = new ParamDescInternal_Numeric<uchar>(); break;
				case USHORT: mInternal = new ParamDescInternal_Numeric<ushort>(); break;
				case ULONG:  mInternal = new ParamDescInternal_Numeric<ulong>(); break;
				case REAL:   mInternal = new ParamDescInternal_Numeric<Real>(); break;
				case CHAR_ARRAY:   mInternal = new ParamDescInternal_NumericArray<char>(); break;
				case SHORT_ARRAY:  mInternal = new ParamDescInternal_NumericArray<short>(); break;
				case LONG_ARRAY:   mInternal = new ParamDescInternal_NumericArray<long>(); break;
				case UCHAR_ARRAY:  mInternal = new ParamDescInternal_NumericArray<uchar>(); break;
				case USHORT_ARRAY: mInternal = new ParamDescInternal_NumericArray<ushort>(); break;
				case ULONG_ARRAY:  mInternal = new ParamDescInternal_NumericArray<ulong>(); break;
				case REAL_ARRAY:   mInternal = new ParamDescInternal_NumericArray<Real>(); break;
			}
		}
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setCharRange(const Range<char>& _range)
	{
		ASSERTCHECK_TYPE_OR_TYPE(CHAR, CHAR_ARRAY);
		static_cast<ParamDescInternal_Numeric<char>*>(mInternal)->mRange = _range;
	}
	//----------------------------------------------------------------------------------------------	
	Range<char> ParamDesc::getCharRange() const
	{
		ASSERTCHECK_TYPE_OR_TYPE(CHAR, CHAR_ARRAY);
		return static_cast<ParamDescInternal_Numeric<char>*>(mInternal)->mRange;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setShortRange(const Range<short>& _range)
	{
		ASSERTCHECK_TYPE_OR_TYPE(SHORT, SHORT_ARRAY);
		static_cast<ParamDescInternal_Numeric<short>*>(mInternal)->mRange = _range;
	}
	//----------------------------------------------------------------------------------------------	
	Range<short> ParamDesc::getShortRange() const
	{
		ASSERTCHECK_TYPE_OR_TYPE(SHORT, SHORT_ARRAY);
		return static_cast<ParamDescInternal_Numeric<short>*>(mInternal)->mRange;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setLongRange(const Range<long>& _range)
	{
		ASSERTCHECK_TYPE_OR_TYPE(LONG, LONG_ARRAY);
		static_cast<ParamDescInternal_Numeric<long>*>(mInternal)->mRange = _range;
	}
	//----------------------------------------------------------------------------------------------	
	Range<long> ParamDesc::getLongRange() const
	{
		ASSERTCHECK_TYPE_OR_TYPE(LONG, LONG_ARRAY);
		return static_cast<ParamDescInternal_Numeric<long>*>(mInternal)->mRange;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setUCharRange(const Range<uchar>& _range)
	{
		ASSERTCHECK_TYPE_OR_TYPE(UCHAR, UCHAR_ARRAY);
		static_cast<ParamDescInternal_Numeric<uchar>*>(mInternal)->mRange = _range;
	}
	//----------------------------------------------------------------------------------------------	
	Range<uchar> ParamDesc::getUCharRange() const
	{
		ASSERTCHECK_TYPE_OR_TYPE(UCHAR, UCHAR_ARRAY);
		return static_cast<ParamDescInternal_Numeric<uchar>*>(mInternal)->mRange;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setUShortRange(const Range<ushort>& _range)
	{
		ASSERTCHECK_TYPE_OR_TYPE(USHORT, USHORT_ARRAY);
		static_cast<ParamDescInternal_Numeric<ushort>*>(mInternal)->mRange = _range;
	}
	//----------------------------------------------------------------------------------------------	
	Range<ushort> ParamDesc::getUShortRange() const
	{
		ASSERTCHECK_TYPE_OR_TYPE(USHORT, USHORT_ARRAY);
		return static_cast<ParamDescInternal_Numeric<ushort>*>(mInternal)->mRange;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setULongRange(const Range<ulong>& _range)
	{
		ASSERTCHECK_TYPE_OR_TYPE(ULONG, ULONG_ARRAY);
		static_cast<ParamDescInternal_Numeric<ulong>*>(mInternal)->mRange = _range;
	}
	//----------------------------------------------------------------------------------------------	
	Range<ulong> ParamDesc::getULongRange() const
	{
		ASSERTCHECK_TYPE_OR_TYPE(ULONG, ULONG_ARRAY);
		return static_cast<ParamDescInternal_Numeric<ulong>*>(mInternal)->mRange;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setRealRange(const Range<Real>& _range)
	{
		ASSERTCHECK_TYPE_OR_TYPE(REAL, REAL_ARRAY);
		static_cast<ParamDescInternal_Numeric<Real>*>(mInternal)->mRange = _range;
	}
	//----------------------------------------------------------------------------------------------	
	Range<Real> ParamDesc::getRealRange() const
	{
		ASSERTCHECK_TYPE_OR_TYPE(REAL, REAL_ARRAY);
		return static_cast<ParamDescInternal_Numeric<Real>*>(mInternal)->mRange;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setArraySize(size_t _size)
	{
		ASSERTCHECK_ANY_ARRTYPE;
		static_cast<ParamDescInternal_Array*>(mInternal)->mSize = _size;
	}
	//----------------------------------------------------------------------------------------------	
	size_t ParamDesc::getArraySize() const
	{
		ASSERTCHECK_ANY_ARRTYPE;
		return static_cast<ParamDescInternal_Array*>(mInternal)->mSize;
	}
	//----------------------------------------------------------------------------------------------	
	void ParamDesc::setEnumNames(const map<String, int>::type& _nameValueMap)
	{
		ASSERTCHECK_TYPE(ENUM);
		static_cast<ParamDescInternal_Enum*>(mInternal)->mNameValueMap = _nameValueMap;
	}
	//----------------------------------------------------------------------------------------------	
	const map<String, int>::type& ParamDesc::getEnumNames() const
	{
		ASSERTCHECK_TYPE(ENUM);
		return static_cast<ParamDescInternal_Enum*>(mInternal)->mNameValueMap;
	}


} // namespace GothOgre