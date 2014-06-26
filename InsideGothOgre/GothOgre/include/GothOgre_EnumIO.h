#ifndef GOTHOGRE_ENUM_IO_H
#define GOTHOGRE_ENUM_IO_H

#include "GothOgre_StrStream.h"


namespace GothOgre
{
	/** Utilities which are used to define intelligent enumerations. 
	You should not use this class directly; it's used by macroses
	GOTHOGRE_DECLARE_ENUM_IO and GOTHOGRE_IMPLEMENT_ENUM_IO. */
	class GOTHOGRE_EXPORT EnumUtil
	{
	public:
		typedef vector<int>::type ValueList;
		typedef map<int, String>::type ValueNameMap;
		typedef map<String, int>::type NameValueMap;

		/** Converts to string. If the current value has not symbolic name,
		an integer number be printed. */
		static void output(StrStream& _ss, int _value, const ValueNameMap& _valueNameMap);

		/** The default implementation of the fromString function.
		This implementation is bad because it doesn't use symbolic names.
		Use macros GOTHOGRE_ENUM_VALUE_DECLARE in derived classes
		to get a more better implementation. */
		static void input(StrStream& _ss, int& _value, const NameValueMap& _nameValueMap);
	};


	//---------------------------------------------------------------------------
	// Macroses to declare and implement enumeration classes
	// (see example later in this file).
	//---------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_ENUM_IO(classname) \
	public: \
		friend StrStream& operator <<(StrStream& _ss, const classname& _val) \
		{ \
			int ival = (int) _val; \
			EnumUtil::output(_ss, ival, getValueNameMap()); \
			return _ss; \
		} \
		\
		friend StrStream& operator >>(StrStream& _ss, classname& _val) \
		{ \
			int ival; \
			EnumUtil::input(_ss, ival, getNameValueMap()); \
			_val = (classname) ival; \
			return _ss; \
		} \
		\
		typedef EnumUtil::ValueList ValueList; \
		typedef EnumUtil::NameValueMap NameValueMap; \
		typedef EnumUtil::ValueNameMap ValueNameMap; \
		\
		static const ValueList& getAllValues() \
		{ \
			return getEnumInfo().allValues; \
		} \
		static const ValueNameMap& getValueNameMap() \
		{ \
			return getEnumInfo().valueNameMap; \
		} \
		static const NameValueMap& getNameValueMap() \
		{ \
			return getEnumInfo().nameValueMap; \
		} \
		\
		static const String TYPE_NAME; \
		\
	private: \
		struct EnumInfo \
		{ \
			NameValueMap nameValueMap; \
			ValueNameMap valueNameMap; \
			ValueList    allValues; \
		}; \
		static const EnumInfo& getEnumInfo();



#	define GOTHOGRE_IMPLEMENT_ENUM_IO(classname) \
	const String classname::TYPE_NAME = #classname; \
	const classname::EnumInfo& classname::getEnumInfo() \
	{ \
		static EnumInfo info; \
		if(info.allValues.empty()) \
		{
	
#	define GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT2(name, username) \
			info.allValues.push_back(name); \
			info.valueNameMap[name] = username; \
			info.nameValueMap[username] = name;

#	define GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(name) \
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT2(name, #name)

#	define GOTHOGRE_IMPLEMENT_ENUM_IO_END \
		} \
		return info; \
	}


	/* Example of using:
	// declaration, .h file:
	class MyEnum
	{
	public:
		enum Enum
		{
			MY_CONSTANT,
			MY_CONSTANT2,
			MY_CONSTANT3,
			DEFAULT
		};
		MyEnum(int _value = DEFAULT) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
		GOTHOGRE_DECLARE_ENUM_IO( MyEnum )
	private:
		Enum mValue;
	};

	// implementation, .cpp file:
	GOTHOGRE_IMPLEMENT_ENUM_IO( MyEnum )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT2( MY_CONSTANT, "MyConstant" )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT ( MY_CONSTANT2 )
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT ( MY_CONSTANT3 )
	GOTHOGRE_IMPLEMENT_ENUM_IO_END

	// using:
	MyEnum t = MyEnum::MY_CONSTANT;
	StrStream ss;
	ss << t; // ss.str() == "MyConstant"
	MyEnum t2;
	ss >> t2; // t2 == MY_CONSTANT
	int tval = (int) t; // tval == 0

	*/

} // namespace GothOgre

#endif // GOTHOGRE_ENUM_IO_H