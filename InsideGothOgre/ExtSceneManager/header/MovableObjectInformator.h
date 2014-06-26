#ifndef MOVABLE_OBJECT_INFORMATOR_H
#define MOVABLE_OBJECT_INFORMATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;

	//-------------------------------------------------------------------------
	class MovableObjectInformator
	{
	public:
		MovableObjectInformator(SceneManagerExImpl* _sceneManager);
		~MovableObjectInformator();

		/** Calculates information about an object in 3D space. */
		SceneUnderwaterInfo  getUnderwaterInfo(MovableObject* _obj);
		SceneSectorInfo      getSectorInfo(MovableObject* _obj);
		SceneFloorInfo       getFloorInfo(MovableObject* _obj);

	private:
		enum Flags
		{
			FLAG_UNDERWATER_INFO = 0x01,
			FLAG_SECTOR_INFO     = 0x02,
			FLAG_FLOOR_INFO      = 0x04
		};

		struct Info
		{
			uint32               flags;
			uint32               oldFlags;
			SceneUnderwaterInfo  underwaterInfo;
			SceneSectorInfo      sectorInfo;
			SceneFloorInfo       floorInfo;

			Info() : flags(0), oldFlags(0) {}
		};

		typedef map<MovableObject*, Info>::type  ObservedObjects;

	private:
		const Vector3& getDerivedPosition(MovableObject* _obj);
		Info* getInfo(MovableObject* _obj);

		GOTHOGRE_DECLARE_FRAME_STARTED( MovableObjectInformator );

	private:
		SceneManagerExImpl* mSceneManager;
		ObservedObjects     mObservedObjects;
	};

} // namespace GothOgre

#endif // MOVABLE_OBJECT_INFORMATOR_H