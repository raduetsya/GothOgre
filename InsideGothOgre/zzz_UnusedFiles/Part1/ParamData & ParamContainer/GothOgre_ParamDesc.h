#ifndef GOTHOGRE_PARAM_DESC_H
#define GOTHOGRE_PARAM_DESC_H

#include "GothOgre_Range.h"
#include "GothOgre_ParamType.h"

namespace GothOgre
{
	struct ParamDescInternal;

	class ParamDesc : public ParamType
	{
	public:
		ParamDesc();
		ParamDesc(Type _type, const String& _name = StringUtil::BLANK);
		ParamDesc(const ParamDesc& _src);
		
		~ParamDesc();

		const ParamDesc& operator =(const ParamDesc& _src);

		bool operator ==(const ParamDesc& _other) const;
		bool operator !=(const ParamDesc& _other) const;

		void setType(Type _type);
		Type getType() const;

		void setName(const String& _name);
		const String& getName() const;

		void setPropertyGroup(const UString& _propertyGroup);
		const UString& getPropertyGroup() const;

		void setDescription(const UString& _description);
		const UString& getDescription() const;
		
		void setCharRange(const Range<char>& _range);
		Range<char> getCharRange() const;

		void setShortRange(const Range<short>& _range);
		Range<short> getShortRange() const;

		void setLongRange(const Range<long>& _range);
		Range<long> getLongRange() const;

		void setUCharRange(const Range<uchar>& _range);
		Range<uchar> getUCharRange() const;

		void setUShortRange(const Range<ushort>& _range);
		Range<ushort> getUShortRange() const;

		void setULongRange(const Range<ulong>& _range);
		Range<ulong> getULongRange() const;

		void setRealRange(const Range<Real>& _range);
		Range<Real> getRealRange() const;

		/// Returns size of the array.
		/// If the data has not an array type, 
		/// the function will throw an exception.
		size_t getArraySize() const;

		/// Sets size of the array
		void setArraySize(size_t _size);

		void setEnumNames(const map<String, int>::type& _nameValueMap);
		const map<String, int>::type& getEnumNames() const;

	private:
		void init();
		void free();

	private:
		Type				mType;
		String				mName;
		UString             mPropertyGroup;
		UString				mDescription;
		ParamDescInternal*  mInternal;
	};
	//--------------------------------------------------------------------------------------
	inline ParamDesc::ParamDesc()
	{
		init();
	}
	//--------------------------------------------------------------------------------------
	inline ParamDesc::ParamDesc(Type _type, const String& _name)
	{
		init();
		setType(_type);
		setName(_name);
	}
	//--------------------------------------------------------------------------------------
	inline ParamDesc::ParamDesc(const ParamDesc& _src)
	{
		init();
		*this = _src;
	}
	//--------------------------------------------------------------------------------------
	inline ParamDesc::~ParamDesc()
	{
		free();
	}
	//--------------------------------------------------------------------------------------
	inline bool ParamDesc::operator !=(const ParamDesc& _other) const
	{
		return !(*this == _other);
	}
	//--------------------------------------------------------------------------------------
	inline ParamDesc::Type ParamDesc::getType() const
	{
		return mType;
	}
	//--------------------------------------------------------------------------------------
	inline void ParamDesc::setName(const String& _name)
	{
		mName = _name;
	}
	//--------------------------------------------------------------------------------------
	inline const String& ParamDesc::getName() const
	{
		return mName;
	}
	//--------------------------------------------------------------------------------------
	inline void ParamDesc::setPropertyGroup(const UString& _propertyGroup)
	{
		mPropertyGroup = _propertyGroup;
	}
	//--------------------------------------------------------------------------------------
	inline const UString& ParamDesc::getPropertyGroup() const
	{
		return mPropertyGroup;
	}
	//--------------------------------------------------------------------------------------
	inline void ParamDesc::setDescription(const UString& _description)
	{
		mDescription = _description;
	}
	//--------------------------------------------------------------------------------------
	inline const UString& ParamDesc::getDescription() const
	{
		return mDescription;
	}

}; // namespace GothOgre

#endif // GOTHOGRE_PARAM_DESC_H