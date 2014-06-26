#ifndef GOTHOGRE_PARAM_CONTAINER_H
#define GOTHOGRE_PARAM_CONTAINER_H

#include "GothOgre_ParamData.h"
#include "GothOgre_Range.h"

namespace GothOgre
{
	/// A container is a collection of individual values. Each value has its own ID and type.
	class ParamContainer : public ParamType
	{
	private:
		typedef map<long, ParamData>::type	IdDataMap;
		IdDataMap	mIdDataMap;

	public:
		/// Creates an empty container 
		ParamContainer();

		/// Copy constructor. Creates new container with all of the values from the old one.
		ParamContainer(const ParamContainer& _src);

		/// Class destructor.
		~ParamContainer();

		/// Clears all values in the container.
		void clear();

		/// Returns the current number of elements in the container
		size_t size() const;

		/// Check if the container is empty
		bool empty() const;

		/// Assignment. Copies all values from _src to *this.
		const ParamContainer& operator =(const ParamContainer& _src);
		
		/// Returns true if the compared containers have the same values, 
		/// and all values are equal, otherwise false.
		bool operator == (const ParamContainer& _other) const;

		/// Returns the inverse of operator ==.
		bool operator != (const ParamContainer& _other) const;
		
		/// Returns the list of all IDs in the container.
		//typedef ConstMapIterator<IdDataMap> Iterator;
		typedef ConstMapIterator<IdDataMap> Iterator;
		Iterator getIterator() const;

		/// Returns a pointer to directly access the data.
		/// If no value exists under the specified ID, the function will return nullptr
		ParamData* getDataPointer(long _id);

		/// Returns a pointer to directly access the data (read-only).
		/// If no value exists under the specified ID, the function will return nullptr
		const ParamData* getDataPointer(long _id) const;

		/// Returns true if the container has a value with the specified ID
		bool hasData(long _id) const;

		/// Returns a pointer to directly access the data.
		/// If no value exists under the specified ID, the function will throw an exception 
		ParamData& getData(long _id);

		/// Returns a pointer to directly access the data (read-only).
		/// If no value exists under the specified ID, the function will throw an exception 
		const ParamData& getData(long _id) const;

		/// Sets an arbitrary data value by the specified ID. 
		/// If a value exists under the same ID, its content will be overwritten.
		void setData(long _id, const ParamData& _paramData);

		/// Inserts an arbitrary data value by the specified ID. 
		/// If a value exists under the same ID, the function will throw an exception.
		void insertData(long _id, const ParamData& _paramData);

		/// Removes a data from the container by the specified ID. 
		void removeData(long _id);

		/// Returns the parameter's description by the specified ID.
		const ParamDesc* getDesc(long _id) const;

		/// Returns the parameter's type by the specified ID.
		Type getType(long _id) const;

		/// Returns size of the array.
		/// If a value with the specified ID has not an array type, 
		/// the function will throw an exception.
		size_t getArraySize(long _id) const;

		// Access to data by the specified ID
		void setBool(long _id, bool _value);
		bool getBool(long _id) const;

		void setChar(long _id, char _value);
		char getChar(long _id) const;

		void setShort(long _id, short _value);
		short getShort(long _id) const;

		void setLong(long _id, long _value);
		long getLong(long _id) const;

		void setUChar(long _id, uchar _value);
		uchar getUChar(long _id) const;

		void setUShort(long _id, ushort _value);
		ushort getUShort(long _id) const;

		void setULong(long _id, ulong _value);
		ulong getULong(long _id) const;

		void setReal(long _id, Real _value);
		Real getReal(long _id) const;

		void setEnum(long _id, int _value);
		int getEnum(long _id) const;

		void setString(long _id, const String& _value);
		const String& getString(long _id) const;

		void setColor(long _id, const ColourValue& _value);
		const ColourValue& getColor(long _id) const;

		void setVector3(long _id, const Vector3& _value);
		const Vector3& getVector3(long _id) const;

		void setVector4(long _id, const Vector4& _value);
		const Vector4& getVector4(long _id) const;

		void setMatrix3(long _id, const Matrix3& _value);
		const Matrix3& getMatrix3(long _id) const;

		void setMatrix4(long _id, const Matrix4& _value);
		const Matrix4& getMatrix4(long _id) const;

		void setCharArray(long _id, const char* _array, size_t _size);
		const char* getCharArray(long _id) const;

		void setShortArray(long _id, const short* _array, size_t _size);
		const short* getShortArray(long _id) const;

		void setLongArray(long _id, const long* _array, size_t _size);
		const long* getLongArray(long _id) const;

		void setUCharArray(long _id, const uchar* _array, size_t _size);
		const uchar* getUCharArray(long _id) const;

		void setUShortArray(long _id, const ushort* _array, size_t _size);
		const ushort* getUShortArray(long _id) const;

		void setULongArray(long _id, const ulong* _array, size_t _size);
		const ulong* getULongArray(long _id) const;

		void setRealArray(long _id, const Real* _array, size_t _size);
		const Real* getRealArray(long _id) const;

		void setVoidPtr(long _id, const void* _value);
		const void* getVoidPtr(long _id) const;
	};
	//---------------------------------------------------------------------------------------------------
	inline ParamContainer::ParamContainer()
	{
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamContainer::ParamContainer(const ParamContainer& _src)
	{
		*this = _src;
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamContainer::~ParamContainer()
	{
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::clear()
	{
		mIdDataMap.clear();
	}
	//---------------------------------------------------------------------------------------------------
	inline size_t ParamContainer::size() const
	{
		return mIdDataMap.size();
	}
	//---------------------------------------------------------------------------------------------------
	inline bool ParamContainer::empty() const
	{
		return mIdDataMap.empty();
	}
	//---------------------------------------------------------------------------------------------------
	inline const ParamContainer& ParamContainer::operator =(const ParamContainer& _src)
	{
		mIdDataMap = _src.mIdDataMap;
		return *this;
	}
	//---------------------------------------------------------------------------------------------------
	inline bool ParamContainer::operator ==(const ParamContainer& _other) const
	{
		return (mIdDataMap == _other.mIdDataMap);
	}
	//---------------------------------------------------------------------------------------------------
	inline bool ParamContainer::operator !=(const ParamContainer& _other) const
	{
		return !(*this == _other);
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamContainer::Iterator ParamContainer::getIterator() const
	{
		return Iterator(mIdDataMap);
	}
	//---------------------------------------------------------------------------------------------------
	inline const ParamData* ParamContainer::getDataPointer(long _id) const
	{
		return const_cast<ParamContainer*>(this)->getDataPointer(_id);
	}
	//---------------------------------------------------------------------------------------------------
	inline bool ParamContainer::hasData(long _id) const
	{
		return getDataPointer(_id) != nullptr;
	}
	//---------------------------------------------------------------------------------------------------
	inline const ParamData& ParamContainer::getData(long _id) const
	{
		return const_cast<ParamContainer*>(this)->getData(_id);
	}
	//---------------------------------------------------------------------------------------------------
	inline const ParamDesc* ParamContainer::getDesc(long _id) const
	{
		return getData(_id).getDesc();
	}
	//---------------------------------------------------------------------------------------------------
	inline ParamType::Type ParamContainer::getType(long _id) const
	{
		return getData(_id).getType();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setBool(long _id, bool _value)
	{
		return getData(_id).setBool(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline bool ParamContainer::getBool(long _id) const
	{
		return getData(_id).getBool();
	}
	//---------------------------------------------------------------------------------------------------
	inline size_t ParamContainer::getArraySize(long _id) const
	{
		return getData(_id).getArraySize();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setChar(long _id, char _value)
	{
		getData(_id).setChar(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline char ParamContainer::getChar(long _id) const
	{
		return getData(_id).getChar();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setShort(long _id, short _value)
	{
		getData(_id).setShort(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline short ParamContainer::getShort(long _id) const
	{
		return getData(_id).getShort();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setLong(long _id, long _value)
	{
		getData(_id).setLong(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline long ParamContainer::getLong(long _id) const
	{
		return getData(_id).getLong();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setUChar(long _id, uchar _value)
	{
		getData(_id).setUChar(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline uchar ParamContainer::getUChar(long _id) const
	{
		return getData(_id).getUChar();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setUShort(long _id, ushort _value)
	{
		getData(_id).setUShort(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ushort ParamContainer::getUShort(long _id) const
	{
		return getData(_id).getUShort();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setULong(long _id, ulong _value)
	{
		getData(_id).setULong(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline ulong ParamContainer::getULong(long _id) const
	{
		return getData(_id).getULong();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setReal(long _id, Real _value)
	{
		getData(_id).setReal(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline Real ParamContainer::getReal(long _id) const
	{
		return getData(_id).getReal();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setEnum(long _id, int _value)
	{
		getData(_id).setEnum(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline int ParamContainer::getEnum(long _id) const
	{
		return getData(_id).getEnum();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setString(long _id, const String& _value)
	{
		getData(_id).setString(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline const String& ParamContainer::getString(long _id) const
	{
		return getData(_id).getString();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setColor(long _id, const ColourValue& _value)
	{
		getData(_id).setColor(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline const ColourValue& ParamContainer::getColor(long _id) const
	{
		return getData(_id).getColor();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setVector3(long _id, const Vector3& _value)
	{
		getData(_id).setVector3(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline const Vector3& ParamContainer::getVector3(long _id) const
	{
		return getData(_id).getVector3();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setVector4(long _id, const Vector4& _value)
	{
		getData(_id).setVector4(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline const Vector4& ParamContainer::getVector4(long _id) const
	{
		return getData(_id).getVector4();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setMatrix3(long _id, const Matrix3& _value)
	{
		getData(_id).setMatrix3(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline const Matrix3& ParamContainer::getMatrix3(long _id) const
	{
		return getData(_id).getMatrix3();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setMatrix4(long _id, const Matrix4& _value)
	{
		getData(_id).setMatrix4(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline const Matrix4& ParamContainer::getMatrix4(long _id) const
	{
		return getData(_id).getMatrix4();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setCharArray(long _id, const char* _array, size_t _size)
	{
		getData(_id).setCharArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline const char* ParamContainer::getCharArray(long _id) const
	{
		return getData(_id).getCharArray();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setShortArray(long _id, const short* _array, size_t _size)
	{
		getData(_id).setShortArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline const short* ParamContainer::getShortArray(long _id) const
	{
		return getData(_id).getShortArray();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setLongArray(long _id, const long* _array, size_t _size)
	{
		getData(_id).setLongArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline const long* ParamContainer::getLongArray(long _id) const
	{
		return getData(_id).getLongArray();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setUCharArray(long _id, const uchar* _array, size_t _size)
	{
		getData(_id).setUCharArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline const uchar* ParamContainer::getUCharArray(long _id) const
	{
		return getData(_id).getUCharArray();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setUShortArray(long _id, const ushort* _array, size_t _size)
	{
		getData(_id).setUShortArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline const ushort* ParamContainer::getUShortArray(long _id) const
	{
		return getData(_id).getUShortArray();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setULongArray(long _id, const ulong* _array, size_t _size)
	{
		getData(_id).setULongArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline const ulong* ParamContainer::getULongArray(long _id) const
	{
		return getData(_id).getULongArray();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setRealArray(long _id, const Real* _array, size_t _size)
	{
		getData(_id).setRealArray(_array, _size);
	}
	//---------------------------------------------------------------------------------------------------
	inline const Real* ParamContainer::getRealArray(long _id) const
	{
		return getData(_id).getRealArray();
	}
	//---------------------------------------------------------------------------------------------------
	inline void ParamContainer::setVoidPtr(long _id, const void* _value)
	{
		getData(_id).setVoidPtr(_value);
	}
	//---------------------------------------------------------------------------------------------------
	inline const void* ParamContainer::getVoidPtr(long _id) const
	{
		return getData(_id).getVoidPtr();
	}

}; // namespace GothOgre


#endif // GOTHOGRE_PARAM_CONTAINER_H