#ifndef GOTHOGRE_MTL_NAME_UTIL_H
#define GOTHOGRE_MTL_NAME_UTIL_H


namespace GothOgre
{
	/** Operations on material names (these function operate 
	on the names only, they do nothing with materials. */
	struct GOTHOGRE_EXPORT MtlNameUtil
	{
		typedef std::pair<String, String> StringPair;

		/** Returns true if the name is started with characters "P:". */
		static bool isPortalName(const String& _mtlName);

		/** Gets names of sectors from a full portal's material's name,
		for example, "P:XARTOWER04_XARTOWER05" -> "XARTOWER04", "XARTOWER05". */
		static StringPair splitPortalName(const String& _fullName);

		/** Combines names of sectors and returns a full portal's material's name,
		for example, "XARTOWER04", "XARTOWER05" -> "P:XARTOWER04_XARTOWER05". */
		static String makePortalName(const StringPair& _sectors);

		/** Combines names of sectors and returns a full portal's material's name,
		for example, "XARTOWER04", "XARTOWER05" -> "P:XARTOWER04_XARTOWER05". */
		static String makePortalName(const String& _firstSector, const String& _secondSector);

		/** Returns true if the name is started with characters "S:". */
		static bool isSectorFloorName(const String& _mtlName);

		/** Gets name of sector and base material's name from a full name of sector's floor's material,
		for example, "S:XARTOWER04_NW_XARDAS_DUNGEONFLOOR_01" -> "XARTOWER04", "NW_XARDAS_DUNGEONFLOOR_01". */
		static StringPair splitSectorFloorName(const String& _fullName);

		/** Combines name of sector and base material's name and returns a full name of sector's floor's material,
		for example, "XARTOWER04", "NW_XARDAS_DUNGEONFLOOR_01" -> "S:XARTOWER04_NW_XARDAS_DUNGEONFLOOR_01". */
		static String makeSectorFloorName(const StringPair& _sectorNameAndMtlName);

		/** Combines name of sector and base material's name and returns a full name of sector's floor's material,
		for example, "XARTOWER04", "NW_XARDAS_DUNGEONFLOOR_01" -> "S:XARTOWER04_NW_XARDAS_DUNGEONFLOOR_01". */
		static String makeSectorFloorName(const String& _sectorName, const String& _mtlName);

		/** Returns true if a name is "GHOSTOCCLUDER". */
		static bool isGhostOccluder(const String& _mtlName);
	};	


} // namespace GothOgre

#endif // GOTHOGRE_MTL_NAME_UTIL_H