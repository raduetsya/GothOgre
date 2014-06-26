#ifndef GOTHOGRE_ZEN_STRING_INTERFACE2_H
#define GOTHOGRE_ZEN_STRING_INTERFACE2_H

#include "GothOgre_ZenStringInterface.h"
#include "GothOgre_Time.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamInt : public ZenParamDef
	{
	public:
		static const String TYPE; // "Int";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(int);}

		virtual int getValue(const void* _target) = 0;
		virtual void setValue(void* _target, int _value) {}

		virtual int getMinValue() {return std::numeric_limits<int>::min();}
		virtual int getMaxValue() {return std::numeric_limits<int>::max();}

	protected:
		void doGet(const void* _target, void* _value) { *(int*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const int*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamEnum : public ZenParamDef
	{
	public:
		static const String TYPE; // "Enum";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(int);}

		virtual int getValue(const void* _target) = 0;
		virtual void setValue(void* _target, int _value) {}

		typedef vector<int>::type      ValueList;
		typedef map<int, String>::type ValueNameMap;
		typedef map<String, int>::type NameValueMap;

		virtual const ValueList&    getAllValues() = 0;
		virtual const ValueNameMap& getValueNameMap() = 0;
		virtual const NameValueMap& getNameValueMap() = 0;

	protected:
		void doGet(const void* _target, void* _value) { *(int*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const int*)_value);}
	};
	//---------------------------------------------------------------------------
	template<typename EnumType>
	class ZenParamEnumT : public ZenParamEnum
	{
	public:
		const String&       getSubType()      {return EnumType::TYPE_NAME;}
		const ValueList&    getAllValues()    {return EnumType::getAllValues();}
		const ValueNameMap& getValueNameMap() {return EnumType::getValueNameMap();}
		const NameValueMap& getNameValueMap() {return EnumType::getNameValueMap();}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamReal : public ZenParamDef
	{
	public:
		static const String TYPE; // "Real";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(Real);}

		virtual Real getValue(const void* _target) = 0;
		virtual void setValue(void* _target, Real _value) {}

		virtual Real getMinValue() {return std::numeric_limits<Real>::min();}
		virtual Real getMaxValue() {return std::numeric_limits<Real>::max();}

	protected:
		void doGet(const void* _target, void* _value) { *(Real*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const Real*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamBool : public ZenParamDef
	{
	public:
		static const String TYPE; // "Bool";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(bool);}

		virtual bool getValue(const void* _target) = 0;
		virtual void setValue(void* _target, bool _value) {}

	protected:
		void doGet(const void* _target, void* _value) { *(bool*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const bool*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamString : public ZenParamDef
	{
	public:
		static const String TYPE; // "String";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(String);}

		struct GOTHOGRE_EXPORT SubType
		{
			static const String VISUAL_NAME;
			static const String SOUND_NAME;
		};

		virtual String getValue(const void* _target) = 0;
		virtual void setValue(void* _target, const String& _value) {}

	protected:
		void doGet(const void* _target, void* _value) { *(String*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const String*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamColour : public ZenParamDef
	{
	public:
		static const String TYPE; // "Colour";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(ColourValue);}

		virtual ColourValue getValue(const void* _target) = 0;
		virtual void setValue(void* _target, const ColourValue& _value) {}

	protected:
		void doGet(const void* _target, void* _value) { *(ColourValue*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const ColourValue*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamVector3 : public ZenParamDef
	{
	public:
		static const String TYPE; // "Vector3";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(Vector3);}

		virtual Vector3 getValue(const void* _target) = 0;
		virtual void setValue(void* _target, const Vector3& _value) {}

	protected:
		void doGet(const void* _target, void* _value) { *(Vector3*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const Vector3*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamQuaternion : public ZenParamDef
	{
	public:
		static const String TYPE; // "Quaternion";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(Quaternion);}

		virtual Quaternion getValue(const void* _target) = 0;
		virtual void setValue(void* _target, const Quaternion& _value) {}

	protected:
		void doGet(const void* _target, void* _value) { *(Quaternion*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const Quaternion*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamAxisAlignedBox : public ZenParamDef
	{
	public:
		static const String TYPE; // "AxisAlignedBox";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(AxisAlignedBox);}

		virtual AxisAlignedBox getValue(const void* _target) = 0;
		virtual void setValue(void* _target, const AxisAlignedBox& _value) {}

	protected:
		void doGet(const void* _target, void* _value) { *(AxisAlignedBox*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const AxisAlignedBox*)_value);}
	};
	//---------------------------------------------------------------------------
	class GOTHOGRE_EXPORT ZenParamTime : public ZenParamDef
	{
	public:
		static const String TYPE; // "Time";
		const String& getType() {return TYPE;}
		size_t getTypeSize() {return sizeof(Time);}

		virtual Time getValue(const void* _target) = 0;
		virtual void setValue(void* _target, const Time& _value) {}

	protected:
		void doGet(const void* _target, void* _value) { *(Time*)_value = getValue(_target);}
		void doSet(void* _target, const void* _value) {setValue(_target, *(const Time*)_value);}
	};

} // namespace GothOgre

#endif // GOTHOGRE_ZEN_STRING_INTERFACE2_H