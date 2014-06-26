/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

namespace attribute
{

	// класс обертка дл€ удалени€ данных из статического вектора
	template <typename Type>
	struct DataHolder
	{
		~DataHolder()
		{
			for (typename Type::iterator item=data.begin(); item!=data.end(); ++item)
				delete (*item).first;
		}

		Type data;
	};

	// интерфейс дл€ обертки пол€
	template <typename OwnerType, typename SetterType>
	struct Field
	{
		virtual void set(OwnerType* _target, typename SetterType::BaseValueType* _value) = 0;
	};

	// шаблон дл€ обертки пол€
	template <typename OwnerType, typename FieldType, typename SetterType>
	struct FieldHolder : public Field<OwnerType, SetterType>
	{
		FieldHolder(FieldType* OwnerType::* offset) : m_offset(offset) {  }
		FieldType* OwnerType::* const m_offset;

		virtual void set(OwnerType* _target, typename SetterType::BaseValueType* _value)
		{
                  _target->*m_offset = SetterType::template convert<FieldType>(_value);
                  //                  _target->*m_offset = (_value == 0 ? 0 : _value->castType<int>::template(false));//SetterType::convert<FieldType>(_value);
		}
	};


	// макрос дл€ инстансировани€ атрибута пол€
#define DECLARE_ATTRIBUTE_FIELD(_name, _type, _setter) \
	template <typename OwnerType, typename ValueType = _type, typename SetterType = _setter> \
	struct _name \
	{ \
		typedef std::pair<Field<OwnerType, SetterType>*, ValueType> BindPair; \
		typedef std::vector<BindPair> VectorBindPair; \
		\
		template <typename FieldType> \
		_name(FieldType* OwnerType::* _offset, const ValueType& _value) \
		{ \
			getData().push_back(BindPair(new FieldHolder<OwnerType, FieldType, SetterType>(_offset), _value)); \
		} \
		\
		static VectorBindPair& getData() \
		{ \
			static DataHolder<VectorBindPair> data; \
			return data.data; \
		} \
	}
	
	// макрос дл€ инстансировани€ экземпл€ра атрибута
#define ATTRIBUTE_FIELD(_attribute, _class, _field, _value) \
	struct _attribute##_##_field \
	{ \
		_attribute##_##_field() \
		{ \
			static attribute::_attribute<_class> bind(&_class::_field, _value); \
		} \
	} _attribute##_##_field


	// шаблон дл€ атрибута класса
	template <typename Type, typename ValueType>
	struct ClassAttribute
	{
		ClassAttribute(const ValueType& _value)
		{
			getData() = _value;
		}
		static ValueType& getData()
		{
			static ValueType data;
			return data;
		}
	};

	// макрос дл€ инстансировани€ атрибута класса
#define DECLARE_ATTRIBUTE_CLASS(_name, _type) \
	template <typename Type, typename ValueType = _type> \
	struct _name : public attribute::ClassAttribute<_name<Type>, ValueType> \
	{ \
		_name(const ValueType& _value) : \
			ClassAttribute<_name<Type>, ValueType>(_value) { } \
	}

	// макрос дл€ инстансировани€ экземпл€ра класса
#define ATTRIBUTE_CLASS(_attribute, _class, _value) \
	class _class; \
	static attribute::_attribute<_class> _attribute##_##_class(_value)
}

#endif // __ATTRIBUTE_H__
