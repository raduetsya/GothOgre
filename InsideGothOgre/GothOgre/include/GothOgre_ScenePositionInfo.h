#ifndef GOTHOGRE_SCENE_POSITION_INFO_H
#define GOTHOGRE_SCENE_POSITION_INFO_H


namespace GothOgre
{
	/** Base class. */
	struct ScenePositionInfo
	{
		/** Position in the world space. */
		Vector3 position;
	};


	/** Information about a point under water. */
	struct SceneUnderwaterInfo : ScenePositionInfo
	{
		/** Water material (underwater part). */
		MaterialPtr  waterMaterial;

		/** Immersion depth. If a point isn't underwater, 
		this value is negative. */
		Real         immersionDepth;
	};

	/** Sector information about a point. */
	struct SceneSectorInfo : ScenePositionInfo
	{
		/** The name of the sector. 
		Empty string if the sector isn't found. */
		String  sectorName;
	};

	/** Information about a point relative the floor. */
	struct SceneFloorInfo : ScenePositionInfo
	{
		/** Material used by the floor. */
		MaterialPtr  floorMaterial;

		/** Distance to the floor. 
		This value is negative if a floor isn't found. */
		Real         heightAboveFloor;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SCENE_POSITION_INFO_H