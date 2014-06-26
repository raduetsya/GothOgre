#include "GothOgre_StdHeaders.h"
#include "GothOgre_ParamContainer.h"


namespace GothOgre
{

	ParamData* ParamContainer::getDataPointer(long _id)
	{
		IdDataMap::iterator it = mIdDataMap.find(_id);
		if(it == mIdDataMap.end())
			return nullptr;
		else
			return &(it->second);
	}
	//--------------------------------------------------------------------------------------------
	ParamData& ParamContainer::getData(long _id)
	{
		ParamData* data = getDataPointer(_id);
		GOTHOGRE_ASSERT(data != nullptr, "ParamContainer: Not found data by ID = " << _id);
		return *data;
	}
	//---------------------------------------------------------------------------------------------------
	void ParamContainer::setData(long _id, const ParamData& _paramData)
	{
		getData(_id) = _paramData;
	}
	//--------------------------------------------------------------------------------------------
	void ParamContainer::insertData(long _id, const ParamData& _paramData)
	{
		IdDataMap::iterator it = mIdDataMap.find(_id);
		GOTHOGRE_ASSERT(it == mIdDataMap.end(), "ParamContainer: Data with ID = " << _id << " already exists");
		mIdDataMap.insert(IdDataMap::value_type(_id, _paramData));
	}
	//--------------------------------------------------------------------------------------------
	void ParamContainer::removeData(long _id)
	{
		IdDataMap::iterator it = mIdDataMap.find(_id);
		GOTHOGRE_ASSERT(it != mIdDataMap.end(), "ParamContainer: Not found data by ID = " << _id);
		mIdDataMap.erase(it);
	}

} // namespace GothOgre

