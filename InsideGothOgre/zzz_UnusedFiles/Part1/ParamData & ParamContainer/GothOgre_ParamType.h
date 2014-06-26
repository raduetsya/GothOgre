#ifndef GOTHOGRE_PARAM_TYPE_H
#define GOTHOGRE_PARAM_TYPE_H

namespace GothOgre
{
	/// This class can hold values of many different types. 
	/// It can be stored in ParamContainer objects.
	struct ParamType
	{
	public:
		enum Type
		{
			NIL,
			BOOL, 
			ENUM,
			CHAR,
			SHORT,
			LONG,
			UCHAR,
			USHORT,
			ULONG,
			REAL,
			STRING,
			COLOR,
			VECTOR3,
			VECTOR4,
			MATRIX3,
			MATRIX4,
			CHAR_ARRAY,
			SHORT_ARRAY,
			LONG_ARRAY,
			UCHAR_ARRAY,
			USHORT_ARRAY,
			ULONG_ARRAY,
			REAL_ARRAY,
			VOID_PTR
		};
	};

} // namespace GothOgre

#endif // GOTHOGRE_PARAM_TYPE_H