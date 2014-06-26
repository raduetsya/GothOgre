#include "GothOgre_Precompiled.h"
#include "GothOgre_MtlNameUtil.h"
#include "GothOgre_StrUtil.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	bool MtlNameUtil::isPortalName(const String& _mtlName)
	{
		return _mtlName.length() >= 3 && _mtlName[1] == ':' 
			&& toupper(_mtlName[0]) == 'P';
	}
	//------------------------------------------------------------------------
	MtlNameUtil::StringPair MtlNameUtil::splitPortalName(const String& _fullName)
	{
		StringPair result;
		size_t i = _fullName.find('_');
		if(i != String::npos)
		{
			result.first = _fullName.substr(2, i - 2);
			result.second = _fullName.substr(i + 1);
		}
		return result;		
	}
	//------------------------------------------------------------------------
	String MtlNameUtil::makePortalName(const StringPair& _sectors)
	{
		return makePortalName(_sectors.first, _sectors.second);
	}
	//------------------------------------------------------------------------
	String MtlNameUtil::makePortalName(const String& _firstSector, const String& _secondSector)
	{
		String result;
		result += "P:";
		result += _firstSector;
		result += "_";
		result += _secondSector;
		return result;
	}
	//------------------------------------------------------------------------
	bool MtlNameUtil::isSectorFloorName(const String& _mtlName)
	{
		return _mtlName.length() >= 3 && _mtlName[1] == ':' 
			&& toupper(_mtlName[0]) == 'S';
	}
	//------------------------------------------------------------------------
	MtlNameUtil::StringPair MtlNameUtil::splitSectorFloorName(const String& _fullName)
	{
		StringPair result;
		size_t i = _fullName.find('_');
		if(i != String::npos)
		{
			result.first = _fullName.substr(2, i - 2);
			result.second = _fullName.substr(i + 1);
		}
		return result;
	}
	//------------------------------------------------------------------------
	String MtlNameUtil::makeSectorFloorName(const StringPair& _sectorNameAndMtlName)
	{
		return makeSectorFloorName(_sectorNameAndMtlName.first, _sectorNameAndMtlName.second);
	}
	//------------------------------------------------------------------------
	String MtlNameUtil::makeSectorFloorName(const String& _sectorName, const String& _mtlName)
	{
		String result;
		result += "S:";
		result += _sectorName;
		result += "_";
		result += _mtlName;
		return result;
	}
	//------------------------------------------------------------------------
	bool MtlNameUtil::isGhostOccluder(const String& _mtlName)
	{
		return StrUtil::equalsNoCase(_mtlName, "GHOSTOCCLUDER");
	}

} // namespace GothOgre